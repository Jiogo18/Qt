#include "Transfo.h"

/*
World Transfo3D::changeRef(const World *world, Pos3D pos)
{
    World retour;
    QMapIterator<Timestamp, Block*> iB(world->getBlocks());
    while(iB.hasNext()) {
        iB.next();
        Block *current = new Block(*iB.value());
        current->setPoint(rotation(iB.value()->getPoint() + pos.getPoint(), pos.getRX(), pos.getRY()));
        retour.addBlock(current);
    }
    QMapIterator<Timestamp, Entity*> iE(world->getEntities());
    while(iE.hasNext()) {
        iE.next();
        Entity *current = new Entity(*iE.value());
        current->setPoint(rotation(iE.value()->getPoint() + pos.getPoint(), pos.getRX(), pos.getRY()));
        retour.addBlock(current);
    }
    return *world;
}*/




/*void Transfo3D::addPerspective(World *world, double persp, double zoom)
{

}
Point3D Transfo3D::pointPerspective(Point3D point, double perspective, double zoom)
{
    return point;
}*/

