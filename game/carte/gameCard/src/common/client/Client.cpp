#include "Client.h"


Client::Client() : ClientInfo(), ClientAuth()
{}

Client::Client(const QString &login) : ClientInfo(login), ClientAuth(login)
{

}

Client::~Client()
{
    if(socket) { delete socket; socket = nullptr; }
}



void Client::setSocket(Socket *socket)
{
    if(this->socket) {
        QObject::disconnect(this->socket, &Socket::connected, this, &Client::onConnect);
        QObject::disconnect(this->socket, &Socket::disconnected, this, &Client::onDisconnect);
    }
    this->socket = socket;
    QObject::connect(socket, &Socket::connected, this, &Client::onConnect);
    QObject::connect(socket, &Socket::disconnected, this, &Client::onDisconnect);
}

void Client::onConnect() const
{
    LogNav::addLog("Client", ClientAuth::getLogin()+tr(" is now connected"), Chat::Info);
}

void Client::onDisconnect() const
{
    LogNav::addLog("Client", ClientAuth::getLogin()+tr(" is now disconnected"), Chat::Info);
}

