#include "SocketConnection.h"

SocketConnection::SocketConnection(Step state, ConnectionError error) : SocketPacket()
{
    this->state = state;
    this->error = error;
}

SocketConnection *SocketConnection::operator =(const SocketConnection &sktCnct)
{
    state = sktCnct.state;
    error = sktCnct.error;
    password = sktCnct.password;
    return this;
}


QDataStream &operator<<(QDataStream &out, const SocketConnection &sktCnct)
{
    out << sktCnct.getState() << sktCnct.getError() << sktCnct.getPasswords();
    return out;
}

QDataStream &operator>>(QDataStream &in, SocketConnection &sktCnct)
{
    SocketConnection::Step state;
    in >> state;
    SocketConnection::ConnectionError error;
    in >> error;
    QMap<QString, Password> passwords;
    in >> passwords;
    sktCnct = SocketConnection(state, error);
    sktCnct.setPasswords(passwords);
    return in;
}

QDebug operator<<(QDebug debug, const SocketConnection &sktCnct)
{
    debug << sktCnct.getState() << sktCnct.getError() << sktCnct.getPasswords().size();
    return debug;
}
