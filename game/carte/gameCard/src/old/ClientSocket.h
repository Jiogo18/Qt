#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "src/common/connection/Socket.h"
#include "src/common/client/Client.h"
#include <QTimer>
#include <QHostAddress>
#include "src/common/connection/SocketConnection.h"
#include "src/common/LogNav.h"
#include <QRandomGenerator>

/* Etapes :
 * 1) Le serv envoie une phrase chiffrée par servPassword
 * 2) Le client déchiffre avec son servPassword et renvoie:
 *   - Son mdp chiffrée avec la phrase
 *   - La phrase chiffrée avec servPassword INVERSE (10010 => 01001)
 * 3) Le serv déchiffre
 *   - la phrase chiffrée avec servPassword Inverse (peut refuser si phrase pas bonne)
 *   - le mdp du Client (peut refuser si pas bon)
 * 4) récup le client avec getClient() (avant le timeout)
*/

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    ClientSocket(QTcpSocket *socket, Password servPassword);
    ~ClientSocket();

    bool sendConnection(const SocketConnection &pktCnct);
    const Socket *getSocket() const { return socket; }

    bool isAutorized() { return state == SocketConnection::StepAutorized; }
    Client *confirmPassword();//Warning: this delete ClientSocket (last step)
    void discardPassword();

private slots:
    void onTimeout();
    void onPktConnection(SocketConnection pktCnct);
    void onDisconnect() { emit disconnected(this); }

signals:
    void disconnected(ClientSocket *socket);
    void passwordToConfirm(ClientSocket *clientSkt, const Client *client);

private:
    void nextStep();
    Socket *socket = nullptr;
    Client *client = nullptr;
    Password servPassword;//constant à la création de ClientSocket
    QTimer timerAuth;
    SocketConnection::Step state = SocketConnection::Stopped;
    Password lastSentSentence;
};

#endif // CLIENTSOCKET_H
