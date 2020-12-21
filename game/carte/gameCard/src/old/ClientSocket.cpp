#include "ClientSocket.h"

ClientSocket::ClientSocket(QTcpSocket *socket, Password servPassword) : QObject()
{
    this->servPassword = servPassword;
    this->socket = new Socket(socket);
    connect(&timerAuth, &QTimer::timeout, this, &ClientSocket::onTimeout);
    timerAuth.setSingleShot(true);
    timerAuth.start(10000);//10s

    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnect);

    nextStep();
}

ClientSocket::~ClientSocket()
{
    if(socket) { delete socket; socket = nullptr; }
    if(client) { delete client; client = nullptr; }
}

bool ClientSocket::sendConnection(const SocketConnection &pktCnct)
{
    if(socket->state() != QTcpSocket::ConnectedState)
        return false;
    QByteArray *packet = new QByteArray;
    QDataStream *out = new QDataStream(packet, QIODevice::WriteOnly);
    out->setVersion(socket->version);
    *out << (quint32)0;//taille du paquet

    *out << pktCnct;

    socket->sendData(packet, out);
    return true;
}


Client *ClientSocket::confirmPassword()
{
    if(!isAutorized())
        return nullptr;
    sendConnection(SocketConnection(SocketConnection::Step::StepAutorized));
    LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Succefully Authentified !"), Chat::LogLevel::Info);
    client->setSocket(socket);
    socket = nullptr;//not deleted in ~ClientSocket
    Client *client2 = client;//tempory pointer
    client = nullptr;
    deleteLater();
    return client2;
}

void ClientSocket::discardPassword()
{
    sendConnection(SocketConnection(SocketConnection::Step::StepNotAutorized, SocketConnection::ErrorWrongLogin));
    LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Already registered with another password"), Chat::LogLevel::Warning);
    socket->disconnectFromHost();
    deleteLater();
}

void ClientSocket::onTimeout()
{
    sendConnection(SocketConnection(SocketConnection::StepNotAutorized, SocketConnection::ErrorTimeout));
    LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("You did not authenticate in time"), Chat::LogLevel::Warning);
    socket->disconnectFromHost();
    deleteLater();
}


void ClientSocket::onPktConnection(SocketConnection pktCnct)
{
    switch (pktCnct.getState()) {
    case SocketConnection::Step::Stopped:
    case SocketConnection::Step::StepNotAutorized:
        LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Client has Stopped the connection"), Chat::LogLevel::Error);
        deleteLater();
        break;
    case SocketConnection::Step::StepSentence:
    case SocketConnection::Step::StepAutorized:
        LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Wrong Step: ")+"Sentence", Chat::LogLevel::Error);
        deleteLater();
        break;
    case SocketConnection::Step::StepPassword:
        //we have:
        //servPassword.toNumber() == key
        //lastSentPkt.servPassword == sentence ^ keyReverse
        //pktCnct.secureData == password ^ sentence

        Password sentence = pktCnct.getPassword("servPassword");
        sentence.chiffre(servPassword.getReverse());//dechiffre with keyReverse
        if(lastSentSentence != sentence) {
            sendConnection(SocketConnection(SocketConnection::StepNotAutorized, SocketConnection::ErrorWrongServerPassword));
            LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Wrong Server Password"), Chat::LogLevel::Warning);
            deleteLater();
            return;
        }

        Password password = pktCnct.getPassword("clientPassword");
        password.chiffre(sentence);//dechiffre with sentence

        if(client) {
            LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Client has already created => old has deleted"), Chat::LogLevel::Error);
            delete client; client = nullptr;
        }
        client = new Client(pktCnct.getClientInfo(), password);
        state = SocketConnection::Step::StepAutorized;
        emit passwordToConfirm(this, client);
        break;
    }
}

void ClientSocket::nextStep()
{
    switch (state) {
    case SocketConnection::Step::Stopped: {
        lastSentSentence = QRandomGenerator::securelySeeded().generate64();
        SocketConnection lastSentPkt = SocketConnection(SocketConnection::StepSentence);
        lastSentPkt.addPassword("servPassword", servPassword, lastSentSentence);
        state = SocketConnection::Step::StepSentence;
        sendConnection(lastSentPkt);
        break;
    }
    case SocketConnection::Step::StepSentence:
    case SocketConnection::Step::StepPassword:
        LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Don't ask this function until an answer"), Chat::LogLevel::Info);
        break;
    case SocketConnection::Step::StepNotAutorized:
        LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Not authorized to connect"), Chat::LogLevel::Info);
        deleteLater();
        break;
    case SocketConnection::Step::StepAutorized:
        LogNav::addLog("Server/ClientSocket/"+socket->peerAddress().toString(), tr("Authorized, you can finish with getClient()"), Chat::LogLevel::Info);
        break;
    }
}
