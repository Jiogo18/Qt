#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include <QDataStream>
#include <QByteArray>
#include "Socket.h"
#include "src/common/client/Password.h"
#include "Packet.h"
#include "src/common/client/ClientAuth.h"

namespace PacketManager
{
    void onDataReceived(Socket *socket, Packet::PacketType type, QDataStream &in);

    namespace Message {
        QByteArray make(const ClientInfo *client, const QString &message);
        void onReceived(Socket *socket, QDataStream &in);
        //void (*handlerMessage)(Socket *socket, QString message);
        static std::function<void(Socket*, const ClientInfo*, QString&)> handlerMessage;
    }

    namespace Connection {
        using namespace Packet::Connection;
        QByteArray makeIdentity(const QString &login);
        QByteArray makeWithPass(const Step &step, const Password &password);
        QByteArray makeAutorized();
        QByteArray makeRejected(const RejectedReason &reason);
        void onReceived(Socket *socket, QDataStream &in);
        void onReceived(QDataStream &in,
                        std::function<void(QString&)> funcIdentity,
                        std::function<void(Step&,Password&)> funcPassword,
                        std::function<void()> funcAuthorize,
                        std::function<void(RejectedReason&)> funcReject);
    }
};


#endif // PACKETMANAGER_H
