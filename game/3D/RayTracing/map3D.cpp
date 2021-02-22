#include "map3D.h"

map3D::map3D(QGraphicsScene *scene) : QObject()
{
    this->scene = scene;
    pixmap = scene->addPixmap(QPixmap(qCeil(scene->width()), qCeil(scene->height())));
    world = new World();
    //client = new Entity(Pos3D(-0.5L, 1.5L, 1.5L, -30, -20), ENTITY::Type::ghost);
    //client = new Entity(Pos3D(1.8L, 2, 1.9L, -140, -30), ENTITY::Type::ghost);//belle vue
    //client = new Entity(Pos3D(1.5L, 1.1L, 0.9L, -140, -30), ENTITY::Type::ghost);//vue green_glass

    //client = new Entity(Pos3D(0.5L, 1.5, 2.5L, -90, -30), ENTITY::Type::ghost);//vue miroir verre blanc
    //client = new Entity(Pos3D(1.2L, 0.5, 2.5L, -180, -30), ENTITY::Type::ghost);//vue miroir verre vert
    //client = new Entity(Pos3D(0.5L, 4, 1.2L, -90, -10), ENTITY::Type::ghost);//vue miroir glowstone
    client = new Entity(Pos3D(1, 3, 1.4L, -70, -10), ENTITY::Type::ghost);//belle vue mirroir

    world->addEntity(client);
    world->setBlock(new Block(Pos3D(0, 0, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::oak_log));
    world->setBlock(new Block(Pos3D(0, 1, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::birch_log));
    world->setBlock(new Block(Pos3D(0, 0, 1, 0, 0), BLOCK::Type::cube, BLOCK::Material::stone));
    world->setBlock(new Block(Pos3D(0, 1, 1, 0, 0), BLOCK::Type::cube, BLOCK::Material::glass));
    world->setBlock(new Block(Pos3D(1, 0, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::green_glass));
    world->setBlock(new Block(Pos3D(0, 2, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::glowstone));
    world->setBlock(new Block(Pos3D(0, 0, 2, 0, 0), BLOCK::Type::cube, BLOCK::Material::mirror));

    for(int i=0; i<3; i++) {//mur de miroir entouré de stone
        world->setBlock(new Block(Pos3D(2, 0, i, 0, 0), BLOCK::Type::cube, BLOCK::Material::stone));
        world->setBlock(new Block(Pos3D(2, 1, i, 0, 0), BLOCK::Type::cube, BLOCK::Material::mirror));
        world->setBlock(new Block(Pos3D(2, 2, i, 0, 0), BLOCK::Type::cube, BLOCK::Material::stone));
    }
    world->setBlock(new Block(Pos3D(0, 3, 0, 0, 0), BLOCK::Type::cube, BLOCK::Material::mirror));

    rayt = new RayTracing(world, client);
    rayt->moveToThread(&rayThread);
    connect(&rayThread, &QThread::finished, rayt, &QObject::deleteLater);
    connect(this, &map3D::clientChanged, rayt, &RayTracing::setClient);
    connect(this, &map3D::draw, rayt, &RayTracing::draw);
    connect(rayt, &RayTracing::finished, this, &map3D::rayFinished);
    connect(rayt, &RayTracing::start, this, &map3D::rayStarted);
    connect(rayt, &RayTracing::step, this, &map3D::rayStep);
    rayThread.start();
    lastPixmap = QPixmap(scene->sceneRect().size().toSize());
    lastPixmap.fill(Qt::black);
}

map3D::~map3D()
{
    rayThread.quit();
    rayThread.wait();
    if(world != nullptr) delete world;
    //client est delete par le world donc c'est bon
    if(pixmap != nullptr) delete pixmap;
}

void map3D::refresh() {
    if(rayOnCalc)
        return;
    rayOnCalc = true;
    emit draw(scene->sceneRect().size());
}
void map3D::resizeEvent(const QSize &size) {
    lastSize = size;
    if(lastSize.isValid())
        pixmap->setPixmap(lastPixmap.scaled(lastSize));
}
void map3D::rayFinished(const QPixmap &pixmap) {
    this->pixmap->setPixmap(pixmap);
    lastPixmap=pixmap;
    rayOnCalc = false;
    if(lastPixmap.size() != lastSize)
        resizeEvent(lastSize);
    emit finished();
}


/*void map3D::refresh()
{*/
/*World *worldClient = new World(Transfo3D::changeRef(world, (Pos3D)client));
    Transfo3D::addPerspective(worldClient, 1, 1);

    QList<Block*> blocks = worldClient->getBlocks().values();
    std::sort(blocks.begin(), blocks.end(), BlockLessThan);

    qDebug() << "worldClient" << worldClient->getBlocks();

    for(int i=0; i<blocks.length(); i++) {
        qDebug() << "calc i=" << i;
        qDebug() << blocks.at(i)->getTimestamp();
        qDebug() << "fin test i=" << i;
        QGraphicsPixmapItem *current = nullptr;
        if(graphicsItems.contains(blocks.at(i)->getTimestamp())) {
            qDebug() << "get item" << blocks.at(i)->getTimestamp();
            current = graphicsItems.value(blocks.at(i)->getTimestamp());
            current->setPixmap(blocks.at(i)->getPixmap());

        }
        else {
            qDebug() << "nouvel item" << blocks.at(i)->getTimestamp();
            current = scene->addPixmap(blocks.at(i)->getPixmap());
            graphicsItems.insert(blocks.at(i)->getTimestamp(), current);
        }
        //qDebug() << "update item" << blocks.at(i)->getTimestamp();
        current->setPos(blocks.at(i)->getPoint().getX(), blocks.at(i)->getPoint().getY());
        current->setZValue(blocks.at(i)->getPoint().getZ());
    }
    qDebug() << "calc" << graphicsItems;


    delete worldClient;*/
    /*emit draw(client, scene, pixmap);
}*/

