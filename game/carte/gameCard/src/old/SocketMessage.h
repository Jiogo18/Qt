#ifndef SOCKETMESSAGE_H
#define SOCKETMESSAGE_H

#include "SocketReceiver.h"
#include <QString>
#include <QDebug>
#include <QDataStream>

class SocketMessage : public SocketPacket
{
    Q_OBJECT
public:
    enum Type {
        UNKNOWN,
        INFO,
        CHAT
    };
    SocketMessage();
    SocketMessage(Type type, QString message);
    SocketMessage(const SocketMessage &sktMsg);
    const SocketMessage *operator =(const SocketMessage &sktMsg);

    Type getType() const { return type; }
    QString getMessage() const { return message; }

private:
    Type type = UNKNOWN;
    QString message = "";
};

QDataStream &operator<<(QDataStream &out, const SocketMessage &sktMsg);
QDataStream &operator>>(QDataStream &in, SocketMessage &sktMsg);
QDebug operator<<(QDebug debug, const SocketMessage &sktMsg);

#endif // SOCKETMESSAGE_H
