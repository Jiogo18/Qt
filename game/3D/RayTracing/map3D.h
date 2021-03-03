#ifndef MAP3D_H
#define MAP3D_H

#include "Object3D.h"
#include "Transfo.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QThread>

class map3D : public QObject
{
    Q_OBJECT

public:
    map3D();
    ~map3D();
    const Entity *getClient() const { return client; }
    const World *getWorld() const { return world; }


    void moveUp() { client->moveUp(); }
    void moveDown() { client->moveDown(); }
    void moveFront() { client->moveFront(); }
    void moveBack() { client->moveBack(); }
    void moveLeft() { client->moveLeft(); }
    void moveRight() { client->moveRight(); }
    void moveRX(doubli rX) { client->moveRX(rX); }
    void moveRZ(doubli rZ) { client->moveRZ(rZ); }
    void moveClient(const Pos3D &pos) { client->moveTo(pos); }

    int fillBlock(const Point3D &posMin, const Point3D &posMax, BLOCK::Type blockType, BLOCK::Material blockMaterial);


signals:
    void clientChanged(const Entity *client);

private:
    Entity *client = nullptr;
    World *world = nullptr;
};

#endif // MAP3D_H
