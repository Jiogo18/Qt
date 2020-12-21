#include "Player.h"

Player::Player(const ClientInfo *client)
{
    this->client = client;
}


bool Player::canPlay()//qui jouent lors d'un tour (server ne compte pas comme un joueur
{
    return (client->getType() == ClientInfo::TypeClient || client->getType() == ClientInfo::TypeBot)
            && client->isConnected();
}
