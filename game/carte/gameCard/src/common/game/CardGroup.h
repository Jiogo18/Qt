#ifndef CARDGROUP_H
#define CARDGROUP_H

#include <QObject>
#include "Card.h"
#include <QList>

class CardGroup : public QObject
{
    Q_OBJECT
public:
    CardGroup();

private:
    QList<Card> cards;
};

#endif // CARDGROUP_H
