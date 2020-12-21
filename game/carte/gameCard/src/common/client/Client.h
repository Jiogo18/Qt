#ifndef CLIENT_H
#define CLIENT_H

#include "ClientAuth.h"
#include "ClientInfo.h"
#include "src/common/connection/Socket.h"
#include "src/common/LogNav.h"

class Client : public ClientInfo, public ClientAuth
{
    Q_OBJECT
public:
    Client();
    Client(const QString &login);
    ~Client();

    virtual bool isConnected() const override { return socket ? socket->isConnected() : false; }
    void setSocket(Socket *socket);

    const Socket *getSocket() const { return socket; }
    Socket *getSocket() { return socket; }

private slots:
    void onConnect() const;
    void onDisconnect() const;

private:
    Socket *socket = nullptr;
};

#endif // CLIENT_H
