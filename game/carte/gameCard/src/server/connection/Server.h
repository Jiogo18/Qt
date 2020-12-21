#ifndef GAMECARD_SERVER
#define GAMECARD_SERVER

#include <QTcpServer>
#include <QTcpSocket>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QFormLayout>
#include <QMessageBox>
#include "Blacklist.h"
#include "src/common/client/Client.h"
#include "src/common/connection/PacketManager.h"
#include "src/common/connection/Socket.h"

class Server: public QWidget
{
    Q_OBJECT
public:
    const int version = 1;
    Blacklist blacklist;
    Server(QWidget *parent = nullptr);
    ~Server();
    void createServ();
    void stopServ();
    int sendToEveryone(const QByteArray &data);

public slots:
    void onConsoleReturned(QString line);

private slots:
    void onBCreate();
    void onNewServPassword();
    void onNewConnection();
    void onServerError(QString error);
    void onClientConnect(Socket *socket);
    void onClientDisconnect(Socket *socket);
    void onClientToAuth(Socket *socket, const QString &login);
    void onMessageReceived(const Socket *socket, const ClientInfo *sender, const QString &message);
    void onClientRejected(Packet::Connection::RejectedReason reason);

signals:
    void clientConnected(Client*);
    void clientDisconnected(Client*);
    void unconfirmedConnection(QTcpSocket*);
    void stopped();

private:
    void setState(QString state);
    Client *getClient(const Socket *socket);
    const Client *getClient(const Socket *socket) const;
    bool isAutorized(QTcpSocket *socket) const;
    QTcpServer *server = nullptr;
    QLabel *state = nullptr;
    QSpinBox *port = nullptr;
    QLineEdit *servPassword = nullptr;
    Password *servPass = nullptr;
    QPushButton *bCreate = nullptr;

    QList<Socket*> connections;//toutes les connexions actuelles (même celles non confirmées)
    QList<Client*> clients;
    ClientInfo *servInfo = nullptr;

    QMap<QHostAddress, int> failIsblacklist;//Rejected counter
};

#endif // GAMECARD_SERVER
