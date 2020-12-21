#include "PacketManager.h"

void PacketManager::onDataReceived(Socket *socket, Packet::PacketType type, QDataStream &in)
{
    switch (type) {
        case Packet::PacketMessage:
            Message::onReceived(socket, in);
            break;
        case Packet::PacketConnection:
            Connection::onReceived(socket, in);
            break;
    }
}



QByteArray PacketManager::Message::make(const ClientInfo *client, const QString &message)
{
    return Packet::makePkt(Packet::PacketMessage, [&](QDataStream *out) {
        *out << *client;
        *out << message;
    });
}

void PacketManager::Message::onReceived(Socket *socket, QDataStream &in)
{
    ClientInfo client;
    in >> client;
    QString message;
    in >> message;
    qDebug() << "PacketManager::Message::onReceived from" << socket->peerAddressNamed() << message;
    if(Message::handlerMessage)
        Message::handlerMessage(socket, &client, message);
}



QByteArray PacketManager::Connection::makeIdentity(const QString &login)
{
    return makePkt(Packet::PacketConnection, [&](QDataStream *out) {
        *out << Step::GiveIdentity;
        *out << login;
    });
}
QByteArray PacketManager::Connection::makeWithPass(const Packet::Connection::Step &step, const Password &password)
{
    return makePkt(Packet::PacketConnection, [&](QDataStream *out) {
        *out << step;
        *out << password;
    });
}
QByteArray PacketManager::Connection::makeAutorized()
{
    return makePkt(Packet::PacketConnection, [&](QDataStream *out) {
        *out << Step::ConnectionAutorized;
    });
}
QByteArray PacketManager::Connection::makeRejected(const RejectedReason &reason)
{
    return makePkt(Packet::PacketConnection, [&](QDataStream *out) {
        *out << Step::ConnectionRejected;
        *out << reason;
    });
}

void PacketManager::Connection::onReceived(Socket *socket, QDataStream &in)
{
    Q_UNUSED(socket);
    Step step;
    in >> step;
    qDebug() << "PacketManager/Connection/onReceived from onDataReceived (should not be used) :" << step;
}

void PacketManager::Connection::onReceived(QDataStream &in,
                                           std::function<void(QString&)> funcIdentity,
                                           std::function<void(Step &, Password &)> funcPassword,
                                           std::function<void()> funcAuthorize,
                                           std::function<void(RejectedReason &)> funcReject)
{
    Step step;
    in >> step;
    //qDebug() << "Socket/onDataReceived CONNECTION:" << step;
    switch (step) {
        case Step::GiveIdentity: {
            QString login;
            in >> login;
            if(funcIdentity) funcIdentity(login);
            break;
        }
        case Step::VerifServPass:
        case Step::GiveServPass:
        case Step::VerifClientPass:
        case Step::RegisterClient:
        case Step::GiveClientPass: {
            Password password;
            in >> password;
            if(funcPassword) funcPassword(step, password);
            break;
        }
        case Step::ConnectionAutorized: {
            if(funcAuthorize) funcAuthorize();
            break;
        }
        case Step::ConnectionRejected: {
            RejectedReason reason;
            in >> reason;
            if(funcReject) funcReject(reason);
            break;
        }
    }
}
