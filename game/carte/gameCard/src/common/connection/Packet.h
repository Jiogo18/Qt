#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QObject>

namespace Packet
{
    enum Version {
        V1 = 1
    };
    const Version version = V1;
    const int timeToAuth = 60000;//TODO: default <10s
    typedef qint64 Size;

    enum PacketType {
        PacketMessage,
        PacketConnection,
    };

    QByteArray makePkt(PacketType type, std::function<void(QDataStream*)> func);

    namespace Connection {
        enum RejectedReason {
            Timeout,
            WrongServPassword,
            WrongLogin,//pas autorisé (ou sur whitelist)
            AlreadyConnected,
            WrongPassword,//pas bon ou pas autorisé
            SkippedSteps,
            OtherReason,
        };
        QString toString(const RejectedReason &reason, bool complete = false);
        enum Step {
            GiveIdentity,//by Client
            VerifServPass,//by Server
            GiveServPass,//by Client
            VerifClientPass,//by Server
            RegisterClient,//byServer
            GiveClientPass,//by Client
            ConnectionAutorized,//by Server and Client (confirmation)
            ConnectionRejected,//by Server or Client
        };
    }
};


#endif // PACKET_H
