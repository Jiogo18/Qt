#include "Socket.h"
#include "PacketManager.h"

Socket::Socket(const Password *servPass, QObject *parent) : Socket(new QTcpSocket(), servPass, parent) {}

Socket::Socket(QTcpSocket *socket, const Password *servPass, QObject *parent) : QObject(parent)
{
    this->socket = socket;
    this->servPass = servPass;
    connect(socket, &QTcpSocket::readyRead, this, &Socket::onDataReceived);
    connect(socket, &QTcpSocket::connected, this, &Socket::onConnect);
    connect(socket, &QTcpSocket::disconnected, this, &Socket::onDisconnect);
    connect(socket, &QTcpSocket::errorOccurred, this, &Socket::onError);
    authTimer.setInterval(Packet::timeToAuth);
    authTimer.setSingleShot(true);
    connect(&authTimer, &QTimer::timeout, this, &Socket::onAuthTimeout);
}

Socket::~Socket()
{
    if(socket) {
        qDebug() << "~Socket" << socket->state();
        if(socket->state() != QTcpSocket::UnconnectedState)
            socket->abort();
        delete socket; socket = nullptr;
    }
}



void Socket::connectToHost(const QString &hostName, quint16 port, ClientAuth *auth, Password *servPass)
{
    if(!isDisconnected()) {
        qDebug() << "Socket::connectToHost already connected, please disconnect before";
        return;
    }
    this->servPass = servPass;
    this->auth = auth;
    LogNav::addLog("Socket", tr("Connecting to ")+"["+hostName+"]", Chat::Info);
    socket->connectToHost(hostName, port);
}

qint64 Socket::write(const QByteArray &data) const
{
    if(state != AuthState::Authentified)
        return -1;
    return socket->write(data);
}

void Socket::onConnect()
{
    tailleMessage = 0;
    state = AuthState::ReadyToAuth;
    authTimer.start();
    //envoyer quand on est connecté (auth n'est défini que à connectToHost et retiré à la déco)
    //=> executé uniquement pas le Client
    if(auth)
        socket->write(PacketManager::Connection::makeIdentity(auth->getLogin()));
}

void Socket::onDisconnect()
{
    LogNav::addLog("Socket", tr("Disconnected from ")+peerAddressNamed(), Chat::Info);
    resetIdentity();
    emit disconnected(this);
}



void Socket::onError(QAbstractSocket::SocketError error)
{ state = AuthState::NotAuthentified; emit Socket::error(error); }


void Socket::onDataReceived()
{
    QDataStream in(socket);
    while(true)
    {
        if(tailleMessage == 0)
        {
            if(socket->bytesAvailable() < (qint64)sizeof(Packet::Size))
                break;//pas encore complet

            in >> tailleMessage;
        }

        if (socket->bytesAvailable() < tailleMessage)
            break;
        tailleMessage = 0;

        //Message entier
        Packet::Version version;
        in >> version;
        Packet::PacketType type;
        in >> type;
        switch (type) {
            case Packet::PacketConnection: {
                PacketManager::Connection::onReceived(in,
                                                      [this](QString &login)
                                                        { this->onConnectionReceivedIdentity(login); },
                                                      [this](Packet::Connection::Step &step, Password &password)
                                                        { this->onConnectionReceivedPassword(step, password); },
                                                      [this]()
                                                        { this->onConnectionReceivedAutorize(); },
                                                      [this](Packet::Connection::RejectedReason &reason)
                                                        { this->onConnectionReceivedReject(reason); });
                break;
            }
            default:
                if(isConnected())
                    PacketManager::onDataReceived(this, type, in);
                else
                    rejectConnection(Packet::Connection::RejectedReason::SkippedSteps);
                break;
        }

        if(socket->bytesAvailable() > 0) {
            qDebug() << "Socket/onDataReceived/ERROR Data remaining in socket, with" << type << tr("(%n bytes)", "", socket->bytesAvailable());
            emit Socket::error(QAbstractSocket::DatagramTooLargeError);
        }
    }
}

void Socket::onAuthTimeout()
{
    rejectConnection(Packet::Connection::RejectedReason::Timeout);
}

void Socket::onConnectionReceivedIdentity(const QString &login)
{
    if(state != AuthState::NotAuthentified) {
        rejectConnection(Packet::Connection::RejectedReason::SkippedSteps);
        return;
    }
    state = AuthState::ReadyToAuth;
    sentence = Password::generateSentence();
    emit askingAuth(this, login);
    socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::VerifServPass,
                                                          servPass->chiffre(sentence)));
}
void Socket::onConnectionReceivedPassword(Packet::Connection::Step step, const Password &password)
{
    if(((step == Packet::Connection::Step::VerifServPass || step == Packet::Connection::Step::GiveServPass)
            && state != AuthState::ReadyToAuth)
    || ((step == Packet::Connection::Step::VerifClientPass
         || step == Packet::Connection::Step::RegisterClient
         || step == Packet::Connection::Step::GiveClientPass)
            && state != AuthState::VerifiedServPass)) {
        rejectConnection(Packet::Connection::RejectedReason::SkippedSteps);
        return;
    }
    qDebug() << "onConnectionReceivedPassword" << step << state;

    switch (step) {
    case Packet::Connection::Step::VerifServPass:
        //Now: password == sentence ^ servPass
        state = AuthState::VerifiedServPass;
        sentence = password.dechiffre(*servPass);
        socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::GiveServPass,
                                                      servPass->chiffre(sentence.reverse())));
        //Now: password == sentenceRevert ^ servPass
        break;

    case Packet::Connection::Step::GiveServPass: {
        //Now: password == sentenceRevert ^ servPass
        state = AuthState::VerifiedServPass;
        const Password &receivedServPass = password.dechiffre(sentence.reverse());
        if(receivedServPass != *servPass) {
            qDebug() << "WrongServPassword" << servPass->getData() << receivedServPass.getData();
            rejectConnection(Packet::Connection::RejectedReason::WrongServPassword);
            return;
        }
        sentence = Password::generateSentence();
        if(auth && auth->getPassword().isStored()) {
            socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::VerifClientPass,
                                                          auth->getPassword().chiffre(sentence)));
            //Now: password == sentence ^ clientPass
        }
        else {
            socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::RegisterClient,
                                                          servPass->chiffre(sentence)));
            //Now: password == sentence ^ servPass
        }
        break;
    }

    case Packet::Connection::Step::VerifClientPass://like VerifServPass but with clientPass
        state = AuthState::VerifiedClientPass;
        sentence = password.dechiffre(auth->getPassword());
        socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::GiveClientPass,
                                                      auth->getPassword().chiffre(sentence.reverse())));
        break;
    case Packet::Connection::Step::RegisterClient:
        state = AuthState::VerifiedClientPass;
        sentence = password.dechiffre(*servPass);
        socket->write(PacketManager::Connection::makeWithPass(Packet::Connection::Step::GiveClientPass,
                                                      auth->getPassword().chiffre(sentence.reverse())));
        break;

    case Packet::Connection::Step::GiveClientPass: {//like GiveServPass
        state = AuthState::VerifiedClientPass;
        Password clientPass = password.dechiffre(sentence.reverse());
        if(!clientPass.isAcceptable()) {
            rejectConnection(Packet::Connection::RejectedReason::WrongPassword);
            return;
        }
        else if(auth == nullptr) {
            qDebug() << "Socket on GiveClientPass error: skipped steps or auth not set";
            rejectConnection(Packet::Connection::RejectedReason::WrongLogin);
            return;//error login not found
        }
        else if(!auth->getPassword().isStored()) {//not created => new client
            auth->setPassword(clientPass);
        }
        else if(clientPass != auth->getPassword()) {
            rejectConnection(Packet::Connection::RejectedReason::WrongPassword);
            return;
        }
        socket->write(PacketManager::Connection::makeAutorized());//wait the answer to be autorized
        break;
    }

    default:break;
    }
}

void Socket::onConnectionReceivedAutorize()
{
    if(auth && auth->isConnected()) {
        rejectConnection(Packet::Connection::RejectedReason::AlreadyConnected);
        return;
    }
    if(state == AuthState::Authentified)
        return;
    if(state != AuthState::VerifiedClientPass) {
        rejectConnection(Packet::Connection::RejectedReason::SkippedSteps);
        return;
    }
    authTimer.stop();
    state = AuthState::Authentified;
    LogNav::addLog("Socket", tr("Connected to ")+peerAddressNamed(), Chat::Info);
    emit connected(this);
}

void Socket::onConnectionReceivedReject(Packet::Connection::RejectedReason reason)
{
    state = AuthState::Rejected;
    emit authRejected(reason);
    LogNav::addLog("Socket", tr("Reject ")+peerAddressNamed()+tr(" because of: ")+Packet::Connection::toString(reason), Chat::Warning);

    abort();
}


void Socket::resetIdentity()
{
    tailleMessage = 0;
    state = AuthState::NotAuthentified;
    auth = nullptr;
    authTimer.stop();
}

void Socket::rejectConnection(Packet::Connection::RejectedReason reason)
{
    socket->write(PacketManager::Connection::makeRejected(reason));
    emit authRejected(reason);
    LogNav::addLog("Socket", tr("Reject ")+peerAddressNamed()+tr(" because of: ")+Packet::Connection::toString(reason), Chat::Warning);
    disconnectFromHost();//to send the RejectedReason
    qDebug() << "rejectConnection by this app because of:" << Packet::Connection::toString(reason);
}

