#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QStandardItemModel>
#include "src/server/connection/Server.h"
#include "src/common/game/Player.h"


class GameServer : public QGroupBox
{
public:
    GameServer(Server *server, QWidget *parent = nullptr);

    void actuTabPlayer();

    void playerJoined(const ClientInfo *client);
    void playerLeft(const ClientInfo *client);

private:
    Server *server = nullptr;
    QTableView *view = nullptr;
    QStandardItemModel *tabPlayer = nullptr;

    QList<Player*> players;

};

#endif // GAMESERVER_H
