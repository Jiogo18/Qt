#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QSplitter>
#include "src/common/widget/Chat.h"
#include "connection/Server.h"
#include "game/GameServer.h"



class fenetre : public QMainWindow
{
    Q_OBJECT

public:
    const int version = 1;
    fenetre(QWidget *parent = nullptr);
    ~fenetre();


private slots:
    void onMessageFromChat(QString line);

private:
    Chat *chat = nullptr;
    Server *server = nullptr;
    GameServer *game = nullptr;
};

#endif // FENETRE_H
