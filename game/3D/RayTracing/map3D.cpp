#include "map3D.h"

map3D::map3D() : QObject()
{
    world = new World();
    client = new Entity(Pos3D(-0.5L, 1.5L, 1.5L, -30, -20), ENTITY::Type::ghost);
    world->addEntity(client);
    //client->moveTo(Pos3D(1.8L, 2, 1.9L, -140, -30));//belle vue
    //client->moveTo(Pos3D(1.5L, 1.1L, 0.9L, -140, -30));//vue green_glass

    //client->moveTo(Pos3D(0.5L, 1.5, 2.5L, -90, -30));//vue miroir verre blanc
    //client->moveTo(Pos3D(1.2L, 0.5, 2.5L, -180, -30));//vue miroir verre vert
    //client->moveTo(Pos3D(0.5L, 4, 1.2L, -90, -10));//vue miroir glowstone
    client->moveTo(Pos3D(1, 3, 1.4L, -70, -10));//belle vue mirroir


    world->setBlock(new Block(Pos3D(0, 0, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::oak_log));
    world->setBlock(new Block(Pos3D(0, 1, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::birch_log));
    world->setBlock(new Block(Pos3D(0, 0, 1, 0, 0), BLOCK::Type::cube, BLOCK::Material::stone));
    world->setBlock(new Block(Pos3D(0, 1, 1, 0, 0), BLOCK::Type::cube, BLOCK::Material::glass));
    world->setBlock(new Block(Pos3D(1, 0, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::green_glass));
    world->setBlock(new Block(Pos3D(0, 2, 0, 0, 0), BLOCK::Type::slab, BLOCK::Material::glowstone));
    world->setBlock(new Block(Pos3D(0, 0, 2, 0, 0), BLOCK::Type::cube, BLOCK::Material::mirror));

    //mur de miroir entouré de stone
    fillBlock(Point3D(2, 0, 0), Point3D(2, 2, 3), BLOCK::Type::cube, BLOCK::Material::stone);
    fillBlock(Point3D(2, 1, 0), Point3D(2, 1, 3), BLOCK::Type::cube, BLOCK::Material::mirror);

    world->setBlock(new Block(Pos3D(0, 3, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::mirror));

}

map3D::~map3D()
{
    if(world != nullptr) delete world;
    //client est delete par le world donc c'est bon
}


int map3D::fillBlock(const Point3D &posMin, const Point3D &posMax, BLOCK::Type blockType, BLOCK::Material blockMaterial)
{
    doubli xMin = std::min(posMin.getX(), posMax.getX()), xMax = std::max(posMin.getX(), posMax.getX());
    doubli yMin = std::min(posMin.getY(), posMax.getY()), yMax = std::max(posMin.getY(), posMax.getY());
    doubli zMin = std::min(posMin.getZ(), posMax.getZ()), zMax = std::max(posMin.getZ(), posMax.getZ());

    int blockPlaced = 0;
    for(int x = xMin; x <= xMax; x++) {
        for(int y = yMin; y <= yMax; y++) {
            for(int z = zMin; z <= zMax; z++) {
                if(world->setBlock(new Block(Pos3D(x, y, z, 0, 0), blockType, blockMaterial)))
                    blockPlaced++;
            }
        }
    }
    return blockPlaced;
}

