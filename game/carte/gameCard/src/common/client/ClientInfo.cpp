#include "ClientInfo.h"

ClientInfo::ClientInfo(const QString &pseudo, const ClientType type) : QObject()
{
    this->pseudo = pseudo;
    this->type = type;
}

ClientInfo::ClientInfo(const ClientInfo &info) : QObject()
{
    pseudo = info.pseudo;
    type = info.type;
}

QString ClientInfo::getStatus() const
{
    return QString((isConnected() ? charConnected : charNotConnected))
            +(type==TypeBot ? "-bot" : "");
}



QDataStream &operator<<(QDataStream &out, const ClientInfo &t)
{
    out << t.getPseudo() << t.getType() << t.getId();
    return out;
}
QDataStream &operator>>(QDataStream &in, ClientInfo &t)
{
    in >> t.pseudo;
    in >> t.type;
    in >> t.id;
    return in;
}
