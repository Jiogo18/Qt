#include "GameServer.h"

GameServer::GameServer(Server *server, QWidget *parent) : QGroupBox(parent)
{
    setAccessibleName("GameServer");
    this->server = server;

    view = new QTableView();
    tabPlayer = new QStandardItemModel(0, 3);
    tabPlayer->setHorizontalHeaderLabels({"Connected", "Pseudo", "Partie"});

    setTitle("Clients");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);

    connect(server, &Server::clientConnected, this, &GameServer::playerJoined);
    connect(server, &Server::clientDisconnected, this, &GameServer::playerLeft);
}

void GameServer::actuTabPlayer()
{
    tabPlayer->setRowCount(players.size());

    for(int i=0; i<players.size(); i++)
    {
        QString connected;

        tabPlayer->setItem(i, 0, new QStandardItem(connected));
        tabPlayer->setItem(i, 1, new QStandardItem(players.at(i)->getClient()->getPseudo()));
        tabPlayer->setItem(i, 2, new QStandardItem(players.at(i)->getPartie()));
    }
}

void GameServer::playerJoined(const ClientInfo *client)
{
    for(int i=0; i<players.size(); i++) {
        if(players.at(i)->getClient() == client) {
            actuTabPlayer();
            return;
        }
    }
    Player *player = new Player(client);
    players.append(player);
    connect(player, &Player::onPlayerClientChanged, this, &GameServer::actuTabPlayer);
    actuTabPlayer();
}

void GameServer::playerLeft(const ClientInfo *client)
{
    for(int i=0; i<players.size(); i++) {
        if(players.at(i)->getClient() == client) {
            actuTabPlayer();
            return;
        }
    }
}
