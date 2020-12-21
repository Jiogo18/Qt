#include "Server.h"

#include "src/common/LogNav.h"

Server::Server(QWidget *parent) : QWidget(parent)
{
    setAccessibleName("Server");
    server = new QTcpServer;
    servPass = new Password;
    servInfo = new ClientInfo(tr("Server"), ClientInfo::TypeServer);

    state = new QLabel(tr("Stopped"));
    port = new QSpinBox;
    port->setRange(0, 65535);
    port->setValue(25567);
    servPassword = new QLineEdit;
    servPassword->setPlaceholderText(tr("Server Password"));
    servPassword->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    bCreate = new QPushButton(tr("Start"));

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow(tr("status"), state);
    layout->addRow(tr("port"), port);
    layout->addWidget(servPassword);
    layout->addWidget(bCreate);

    QObject::connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    QObject::connect(bCreate, &QPushButton::clicked, this, &Server::onBCreate);
    QObject::connect(servPassword, &QLineEdit::editingFinished, this, &Server::onNewServPassword);
    PacketManager::Message::handlerMessage = [this](Socket *socket, const ClientInfo *sender, QString msg) { this->onMessageReceived(socket, sender, msg); };
}

Server::~Server()
{
    stopServ();
    if(server) { delete server; server = nullptr; }
    if(state) { delete state; state = nullptr; }
    if(port) { delete port; port = nullptr; }
    if(servPassword) { delete servPassword; servPassword = nullptr; }
    if(bCreate) { delete bCreate; bCreate = nullptr; }
    if(servPass) { delete servPass; servPass = nullptr; }
    if(servInfo) { delete servInfo; servInfo = nullptr; }
}

void Server::createServ()
{
    if(server->listen(QHostAddress::Any, port->value())) {
        bCreate->setText(tr("Stop"));
        port->setDisabled(true);
        setState(tr("Started on port %n", "", port->value()));
    }
    else {
        onServerError(tr("Server can't start on port %n", "", port->value()) + "\n" +
                server->errorString());
    }
}

void Server::stopServ()
{
    server->close();
    bCreate->setText(tr("Start"));
    port->setEnabled(true);
    setState(tr("Stopped"));
    emit stopped();
}

int Server::sendToEveryone(const QByteArray &data)
{
    int nb=0;
    for(int i=0; i<clients.size(); i++) {
        if(!clients.at(i) || !clients.at(i)->isConnected())
            continue;
        clients.at(i)->getSocket()->write(data);
        nb++;
    }
    return nb;
}

void Server::onConsoleReturned(QString line)
{
    sendToEveryone(PacketManager::Message::make(servInfo, line));
}

void Server::onNewConnection()
{
    QTcpSocket *newConnection(server->nextPendingConnection());
    if(!isAutorized(newConnection)) {
        newConnection->disconnectFromHost();
        return;
    }
    bool alreadyRegistered = false;
    for(int i=0; i<connections.size(); i++) {
        if(*connections.at(i) == newConnection) {
            alreadyRegistered = true;
            break;
        }
    }
    if(!alreadyRegistered) {
        Socket *newSocket = new Socket(newConnection, servPass);
        connections.append(newSocket);
        connect(newSocket, &Socket::connected, this, &Server::onClientConnect);
        connect(newSocket, &Socket::disconnected, this, &Server::onClientDisconnect);
        connect(newSocket, &Socket::askingAuth, this, &Server::onClientToAuth);
        connect(newSocket, &Socket::authRejected, this, &Server::onClientRejected);

        LogNav::addLog("Server", tr("New connection from ") + newSocket->peerAddressNamed(), Chat::Info);
    }
    emit unconfirmedConnection(newConnection);
}

void Server::onServerError(QString error)
{
    QMessageBox::warning(this, tr("Server Creation"), tr("ERROR: ") + error);
    LogNav::addLog("Server", error, Chat::Error);
}

void Server::onClientConnect(Socket *socket)
{
    Client *client = getClient(socket);
    LogNav::addLog("Server", tr("Connected as ")+"\""+client->getLogin()+"\"", Chat::Info);
    emit clientConnected(client);
}

void Server::onClientDisconnect(Socket *socket)
{
    connections.removeAll(socket);
    emit clientDisconnected(getClient(socket));
}

void Server::onClientToAuth(Socket *socket, const QString &login)
{
    if(login == "") {
        socket->rejectLogin();
        return;
    }
    for(int i=0; i<clients.size(); i++) {
        Client *client = clients.at(i);
        if(!client) continue;
        if(client->getLogin() == login) {
            socket->setClientAuth(client);
            return;
        }
    }
    Client *client = new Client(login);
    clients.append(client);
    socket->setClientAuth(client);
}

void Server::onMessageReceived(const Socket *socket, const ClientInfo *sender, const QString &message)
{
    if(!socket->isConnected())
        return;
    Chat *chat = LogNav::findConsole();
    if(chat)
        chat->addMessage(sender, message);
}

void Server::onClientRejected(Packet::Connection::RejectedReason reason)
{
    Q_UNUSED(reason);
    Socket *socket = (Socket*)sender();
    if(!failIsblacklist.contains(socket->peerAddress()))
        failIsblacklist.insert(socket->peerAddress(), 0);
    failIsblacklist[socket->peerAddress()]++;
    if(failIsblacklist.value(socket->peerAddress()) > 10) {//après 10 erreurs on refuse l'accès
        //TODO
        blacklist.addBlacklist(socket->peerAddress(), Blacklist::AutoBlacklist | Blacklist::TooManyConnection);
        LogNav::addLog("Server", socket->peerAddressNamed() + " has made too many mistakes, it goes in blacklist", Chat::Warning);
        failIsblacklist[socket->peerAddress()] = 0;
    }
}



void Server::setState(QString state)
{
    this->state->setText(state);
    LogNav::addLog("Server", state, Chat::Info);
}

Client *Server::getClient(const Socket *socket)
{
    for(int i=0; i<clients.size(); i++) {
        if(!clients.at(i)) continue;
        if(clients.at(i)->getSocket() == socket)
            return clients[i];
    }
    return nullptr;
}
const Client *Server::getClient(const Socket *socket) const
{
    for(int i=0; i<clients.size(); i++) {
        if(!clients.at(i)) continue;
        if(clients.at(i)->getSocket() == socket)
            return clients.at(i);
    }
    return nullptr;
}

bool Server::isAutorized(QTcpSocket *socket) const
{
    return blacklist.isBlacklisted(socket->peerAddress()) == Blacklist::NotBlacklisted;
}

void Server::onBCreate()
{
    if(server->isListening())
        stopServ();
    else
        createServ();
}

void Server::onNewServPassword()
{
    if(servPassword->hasFocus()) {//return emit also editingFinished (but doesn't clear the focus
        QObject::disconnect(servPassword, &QLineEdit::editingFinished, this, &Server::onNewServPassword);
        servPassword->clearFocus();
        QObject::connect(servPassword, &QLineEdit::editingFinished, this, &Server::onNewServPassword);
    }
    *servPass = servPassword->text();
    LogNav::addLog("Server", tr("The server password has been updated"), Chat::Info);
}
