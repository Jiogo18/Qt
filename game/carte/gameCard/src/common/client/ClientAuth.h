#ifndef CLIENTAUTH_H
#define CLIENTAUTH_H

#include <QDateTime>
#include <QString>
#include "UUID.h"
#include "Password.h"

class ClientAuth
{
public:
    ClientAuth(const QString &login = "");
    //ClientAuth(const ClientAuth &client);
    bool operator==(const ClientAuth &client) const { return login == client.login; }

    QString getLogin() const { return login; }
    virtual bool isConnected() const { return false; }
    const Password &getPassword() const { return password; }

    void setPassword(Password password) { this->password = password; }

private:
    QString login = "";
    Password password;
    UUID uuid;
};

#endif // CLIENTAUTH_H
