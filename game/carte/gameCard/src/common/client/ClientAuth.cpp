#include "ClientAuth.h"

ClientAuth::ClientAuth(const QString &login)
{
    this->login = login;
}

/*ClientAuth::ClientAuth(const ClientAuth &client) : QObject(client.parent())
{
    login = client.login;
    password = client.password;
    uuid = client.uuid;
}*/
