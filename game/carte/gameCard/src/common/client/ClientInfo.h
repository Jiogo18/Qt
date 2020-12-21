#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>
#include "UUID.h"

class ClientInfo : public QObject
{
public:
    enum ClientType {
        TypeClient,
        TypeBot,
        TypeServer
    };
    ClientInfo(const QString &pseudo = "", const ClientType type = TypeClient);
    ClientInfo(const ClientInfo &info);

    virtual bool isConnected() const { return (type==TypeBot || type==TypeServer) ? true : false; }
    QString getPseudo() const { return pseudo; }
    QString getStatus() const;
    ClientType getType() const { return type; }
    const UUID &getId() const { return id; }

    void setPseudo(QString pseudo) { this->pseudo = pseudo; }

private:
    QString pseudo;
    UUID id;
    ClientType type;
    const QString charConnected = "✔";
    const QString charNotConnected = "❌";
    friend QDataStream &operator>>(QDataStream &in, ClientInfo &t);
};


QDataStream &operator<<(QDataStream &out, const ClientInfo &t);
QDataStream &operator>>(QDataStream &in, ClientInfo &t);

#endif // CLIENTINFO_H
