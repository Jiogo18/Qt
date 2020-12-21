#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "CardGroup.h"
#include "src/common/client/ClientAuth.h"
#include "src/common/client/ClientInfo.h"

class Player : public QObject
{
    Q_OBJECT

public:
    Player(const ClientInfo *client);

    void setClient(const ClientInfo *client) { this->client = client; emit onPlayerClientChanged(); }
    const ClientInfo *getClient() const { return client; }

    bool canPlay();
    ClientInfo::ClientType getType() const { return client->getType(); }
    QString getPseudo() const { return client->getPseudo(); }
    void setPartie(int partie) { dansPartie = partie; }
    int getPartie() const { return dansPartie; }


    CardGroup *getCardGroup() { return cards.first(); }
    CardGroup *getCardGroup(int i) { return 0<=i && i<cards.size() ? cards[i] : getCardGroup(); }
    CardGroup *removeCardGroup(CardGroup *cg) { cards.removeAll(cg); return cg; }//doesn't delete !
    CardGroup *addCardGroup(CardGroup *cg) { cards.append(cg); return cg; }
    CardGroup *addCardGroup(CardGroup *cg, int i) { cards.insert(i, cg); return cg; }


signals:
    void onPlayerClientChanged();
    void onNameChanged(const Player *player);

private:
    QList<CardGroup*> cards;

    const ClientInfo *client;
    int dansPartie = 0;
};

#endif // PLAYER_H
