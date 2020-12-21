#ifndef WIDGETCONNECTION_H
#define WIDGETCONNECTION_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QAbstractSocket>
#include <QMessageBox>
#include "src/common/connection/PacketManager.h"
#include "src/common/connection/Socket.h"
#include "src/common/client/Client.h"

class WidgetConnection : public QDialog
{
    Q_OBJECT
public:
    WidgetConnection(Client *client, QWidget *parent = nullptr);
    ~WidgetConnection();
    //TODO: info sur la connexion actuelle
    void show() { QWidget::show(); setFocus(Qt::PopupFocusReason); }
    const Socket *getSocket() { return socket; }
    bool isConnected() const { return socket->isConnected(); }
    bool sendMessage(QString msg) { return socket->write(PacketManager::Message::make(client, msg)); }

public slots:
    void onError(QAbstractSocket::SocketError error);
    void connectToHost();
    void disconnectFromHost();
    void showInfo() const;

private slots:
    void onBConnect();
    void onConnect();
    void onDisconnect();

private:
    QLineEdit *ip = nullptr;
    QSpinBox *port = nullptr;
    QLineEdit *servPassword = nullptr;
    QLineEdit *pseudo = nullptr;
    QLineEdit *password = nullptr;
    QPushButton *bConnect = nullptr;
    QPushButton *bInfo = nullptr;
    Socket *socket = nullptr;
    Client *client = nullptr;
    Password *servPass = nullptr;

    void actuButtons();
};

#endif // WIDGETCONNECTION_H
