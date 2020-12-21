#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include <QDataStream>
#include "src/common/client/Password.h"
#include <QHostAddress>
#include <QTimer>
#include "Packet.h"
#include "src/common/client/ClientAuth.h"
#include "src/common/client/ClientInfo.h"
#include "src/common/LogNav.h"


class Socket : public QObject
{
    Q_OBJECT
public:
    const int version = 1;
    enum AuthState {
        NotAuthentified,
        ReadyToAuth,
        VerifiedServPass,
        VerifiedClientPass,
        Authentified,
        Rejected
    };

    Socket(const Password *servPass, QObject *parent = nullptr);
    Socket(QTcpSocket *socket, const Password *servPass, QObject *parent = nullptr);
    ~Socket();
    bool operator==(const Socket *socket) const { return (QTcpSocket*)this == (QTcpSocket*)socket; }
    bool operator==(const QTcpSocket *socket) const { return (QTcpSocket*)this == socket; }
    void setClientAuth(ClientAuth *auth) { this->auth = auth; }
    void rejectLogin() { rejectConnection(Packet::Connection::RejectedReason::WrongLogin); }

    void connectToHost(const QString &hostName, quint16 port, ClientAuth *auth, Password *servPass);
    void abort() { resetIdentity(); socket->abort(); }
    void disconnectFromHost() { resetIdentity(); socket->disconnectFromHost(); }
    qint64 write(const QByteArray &data) const;

    AuthState getAuthState() const { return state; }
    bool isConnected() const { return state == AuthState::Authentified; }
    bool isDisconnected() const { return state == AuthState::NotAuthentified; }
    QTcpSocket::SocketState getState() const { return socket->state(); }
    QString errorString() const { return socket->errorString(); }
    QString peerName() const { return socket->peerName(); }
    qint64 peerPort() const { return socket->peerPort(); }
    QHostAddress peerAddress() const { return socket->peerAddress(); }
    QString peerAddressNamed() const { return "["+socket->peerAddress().toString()+"]"; }
    qint64 bytesAvailable() const { return socket->bytesAvailable(); }

private slots:
    void onConnect();
    void onDisconnect();
    void onError(QAbstractSocket::SocketError error);
    void onDataReceived();
    void onAuthTimeout();
    void onConnectionReceivedIdentity(const QString &login);
    void onConnectionReceivedPassword(Packet::Connection::Step step, const Password &password);
    void onConnectionReceivedAutorize();
    void onConnectionReceivedReject(Packet::Connection::RejectedReason reason);

signals:
    void connected(Socket*);//après autorisation
    void disconnected(Socket*);
    void error(QAbstractSocket::SocketError);
    void packetReceived(Socket*, Packet::PacketType, QByteArray&);
    void authRejected(Packet::Connection::RejectedReason);
    void askingAuth(Socket*, QString login);

private:
    void resetIdentity();
    void rejectConnection(Packet::Connection::RejectedReason reason);
    Packet::Size tailleMessage = 0;
    QTcpSocket *socket = nullptr;

    AuthState state = AuthState::NotAuthentified;
    const Password *servPass = nullptr;
    ClientAuth *auth = nullptr;
    QTimer authTimer;
    Password sentence;
};

#endif // SOCKET_H
