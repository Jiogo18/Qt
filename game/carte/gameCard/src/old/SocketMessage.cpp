#include "SocketMessage.h"

SocketMessage::SocketMessage() : SocketPacket(SocketPacket::MESSAGE)
{
    type = SocketMessage::UNKNOWN;
    message = "";
}

SocketMessage::SocketMessage(Type type, QString message) : SocketPacket(SocketPacket::MESSAGE)
{
    this->type = type;
    this->message = message;
}

SocketMessage::SocketMessage(const SocketMessage &sktMsg) : SocketPacket(SocketPacket::MESSAGE) { operator =(sktMsg); }

const SocketMessage *SocketMessage::operator =(const SocketMessage &sktMsg)
{
    SocketPacket::operator=(sktMsg);
    type = sktMsg.type;
    message = sktMsg.message;
    return this;
}




QDataStream &operator<<(QDataStream &out, const SocketMessage &sktMsg)
{
    out << (SocketPacket)sktMsg << sktMsg.getType() << sktMsg.getMessage();
    return out;
}
QDataStream &operator>>(QDataStream &in, SocketMessage &sktMsg)
{
    SocketMessage::Type type;
    QString message;
    in >> type >> message;
    sktMsg = SocketMessage(type, message);
    return in;
}
QDebug operator<<(QDebug debug, const SocketMessage &sktMsg)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << sktMsg.getType() << ", " << sktMsg.getMessage() << ')';
    return debug;
}
