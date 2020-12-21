#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include "src/common/client/Client.h"
#include "src/common/connection/Socket.h"
#include "src/common/connection/PacketManager.h"
#include "src/common/widget/Chat.h"
#include "Plateau.h"
#include "connection/WidgetConnection.h"
#include <QList>
#include <QSplitter>
#include <QMenuBar>
#include <QMenu>

class fenetre : public QMainWindow
{
    Q_OBJECT
public:
    const int version = 1;

    fenetre(QWidget *parent=nullptr);
    ~fenetre();
    void closeEvent(QCloseEvent *event);
    void show();

public slots:
    void addChat(const Socket *socket, const ClientInfo *sender, QString message);
    void sendMsg(QString msg);

private:
    Client *client = nullptr;
    Plateau *plateau = nullptr;
    Chat *chat = nullptr;
    WidgetConnection *connection;
};

#endif // FENETRE_H
