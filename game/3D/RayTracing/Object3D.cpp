#include "Object3D.h"


doubli ENTITY::baseSpeed(ENTITY::Type type, ENTITY::Direction dir)
{
    doubli speed = 0;
    switch (type) {
    case ENTITY::Type::none:
        speed = 0;
        break;
    case ENTITY::Type::player:
    case ENTITY::Type::ghost:
        speed = 0.1L;
        break;
    }
    switch (type) {
    case ENTITY::Type::player:
        switch (dir) {
        case ENTITY::Direction::left:
        case ENTITY::Direction::right:
        case ENTITY::Direction::back:
            speed /= 2;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return round(speed);
}

QString OBJECT3D::getFileTexture(BLOCK::Material material, QList<BLOCK::Variation> variations)
{
    QString texture = "";
    switch (material) {
    case BLOCK::Material::none: texture = "blocks/none"; break;
    case BLOCK::Material::air: texture = "blocks/air"; break;
    case BLOCK::Material::oak_log: texture = "blocks/oak_log"; break;
    case BLOCK::Material::birch_log: texture = "blocks/birch_log"; break;
    case BLOCK::Material::stone: texture = "blocks/stone"; break;
    case BLOCK::Material::glowstone: texture = "blocks/glowstone"; break;
    case BLOCK::Material::glass: texture = "blocks/glass"; break;
    case BLOCK::Material::green_glass: texture = "blocks/green_stained_glass"; break;
    case BLOCK::Material::mirror: texture ="blocks/mirror"; break;
    }
    switch (material) {
    case BLOCK::Material::oak_log:
    case BLOCK::Material::birch_log:
        if(variations.contains(BLOCK::Variation::top) || variations.contains(BLOCK::Variation::bottom))
            texture += "_top";
        break;
    default:
        break;
    }

    return texture;
}
QImage OBJECT3D::getTexture(QString file)
{
    QImage img;
    if(img.load(":/ressourcepacks/default/textures/"+file+".png")) {
        if(img.format() != QImage::Format_RGBA64)
            img = img.convertToFormat(QImage::Format_RGBA64);
        return img;
    }
    qWarning() << "[OBJECT3D::getTexture] can't load this texture:" << file;
    /*if(img.load(":/ressourcepacks/default/textures/blocks/block_nul.png"))
        return img;*/
    img = QImage(2,2,QImage::Format_RGBA64);
    img.fill(QColor(0, 0, 0, 100));
    img.setPixelColor(0, 0, QColor(255, 0, 255, 100));
    img.setPixelColor(1, 1, QColor(255, 0, 255, 100));

    return img;
}
int OBJECT3D::getLight(BLOCK::Material material, QList<BLOCK::Variation> variations)
{
    Q_UNUSED(variations)
    switch (material) {
    case BLOCK::glowstone:
        return 5;
    default:
        return 0;
    }
}


ColorLight::ColorLight()
{
    this->light = 0;
}
ColorLight::ColorLight(int r, int g, int b, int a, int light)
{
    setRed(r); setGreen(g); setBlue(b); setAlpha(a);
    setLight(light);
}

ColorLight::ColorLight(const ColorLight &color)
{
    r = color.r; g = color.g; b = color.b; a = color.a;
    light = color.light;
}

QColor ColorLight::getColorReduced(double reduce) const
{
    int r = qFloor(this->r*light/reduce), g = qFloor(this->g*light/reduce), b = qFloor(this->b*light/reduce);
    if(r >= 255) r = 255;
    if(g >= 255) g = 255;
    if(b >= 255) b = 255;
    return QColor(r, g, b);
}

ColorLight ColorLight::operator +(const ColorLight &color) const
{
    //si color a une luminositée importante (plus que l'actuelle) alors color est +importante
    //si this a un alpha plus faible (transparent/miroir) alors color est +important
    if(a == 0)//this n'est absolument pas visible
        return color;
    double poids = (color.light - light + 1.0) * 255 / a;

    return ColorLight(qFloor((r + color.r*poids)/(1+poids)),
                      qFloor((g + color.g*poids)/(1+poids)),
                      qFloor((b + color.b*poids)/(1+poids)),
                      a + color.a,
                      light + color.light);
}





Face::Face() : Object(Pos3D(0, 0, 0, 0, 0))
{
    rect = Rect3D(Point3D(0, 0, 0), Point3D(0, 0, 0));
    material = BLOCK::Material::none;
    variations.clear();
    variations.append(BLOCK::Variation::front);
    calcFace();
}
Face::Face(const Point3D &point, const Rect3D &rect, BLOCK::Material material, QList<BLOCK::Variation> variations) : Object(Pos3D(point, 0, 0))
{
    this->rect = rect;
    this->material = material;
    this->variations = variations;
    calcFace();
}
Face::Face(const Face &face) : Object(face.getPos())
{
    rect = face.rect;
    material = face.material;
    variations = face.variations;
    maxGeometry = face.maxGeometry;
    middleGeometry = face.middleGeometry;
    texture = face.texture;
    RX = face.RX; RZ = face.RZ;
}
ColorLight Face::getColor(const QPointF &point, const QImage *img) const
{
    if(img != nullptr && !img->isNull()) {
        if(0 <= point.x() && point.x() <= 1 && 0 <= point.y() && point.y() <= 1) {
            //switch selon les positions ? => via fichier + peut etre
            int x = qFloor(point.x() * img->width());
            int y = qFloor(point.y() * img->height());
            if(img->width() <= x)
                x = img->width() - 1;
            if(img->height() <= y)
                y = img->height() - 1;
            QColor color = img->pixelColor(x, y);
            return ColorLight(color.red(), color.green(), color.blue(),
                              color.alpha(), OBJECT3D::getLight(material,
                                                                variations));
        }
        else
            qDebug() << "Face::getColor point pas dans le cadre:" << point;
    }

    /*int casesDone = 0;
    for(int i=0; i<variations.size(); i++) {
        switch (variations.at(i)) {
        case BLOCK::Variation::top:
            if(casesDone & 1) continue;
            casesDone = casesDone | 1;
            return ColorLight(100, 100, 200, 255, 1);
        case BLOCK::Variation::bottom:
            if(casesDone & 2) continue;
            casesDone = casesDone | 2;
            return ColorLight(200, 200, 100, 255, 1);
        case BLOCK::Variation::front:
            if(casesDone & 4) continue;
            casesDone = casesDone | 4;
            return ColorLight(200, 100, 100, 255, 1);
        case BLOCK::Variation::back:
            if(casesDone & 8) continue;
            casesDone = casesDone | 8;
            return ColorLight(100, 200, 200, 255, 1);
        case BLOCK::Variation::left:
            if(casesDone & 16) continue;
            casesDone = casesDone | 16;
            return ColorLight(100, 200, 100, 255, 1);
        case BLOCK::Variation::right:
            if(casesDone & 32) continue;
            casesDone = casesDone | 32;
            return ColorLight(200, 100, 200, 255, 1);
        }
    }*/
    return ColorLight(0, 0, 0, 255, 0);
}


doubli Face::boundRotX(const doubli &posRX) const
{
    //petit fix pour les faces du dessus mais il faudrait faire un autre calcul pour celles penchées
    if(variations.first() == BLOCK::Variation::top || variations.first() == BLOCK::Variation::top) {
        return RX*4 + posRX;//4+0+
    }
    return RX * 4 + 180 - posRX;//4+180-
}
doubli Face::boundRotZ(const doubli &posRZ) const
{
    return RZ * 4 - 180 + posRZ;//*4-180 marche pour left et pour top
}

void Face::calcFace()
{
    maxGeometry = rect + getPoint();
    middleGeometry = maxGeometry.getMiddle();
    texture = OBJECT3D::getFileTexture(material, variations);


    doubli deltaX = rect.getPointMax().getX() - rect.getPointMin().getX();
    doubli deltaY = rect.getPointMax().getY() - rect.getPointMin().getY();
    doubli deltaZ = rect.getPointMax().getZ() - rect.getPointMin().getZ();

    if(-0.5 < deltaX && deltaX < 0.5) {
        doubli deltaXZ = sqrt(deltaX * deltaX + deltaZ * deltaZ);
        RX = atan(deltaY / deltaXZ) + 45;
    }
    else {
        doubli deltaYZ = sqrt(deltaY * deltaY + deltaZ * deltaZ);
        RX = atan(deltaX / deltaYZ);
    }
    doubli deltaXY = sqrt(deltaX * deltaX + deltaY * deltaY);
    if(-0.5 < deltaXY && deltaXY < 0.5) {
        qDebug() << "alerte deltaZ trop faible" << deltaXY << deltaZ;
    }
    RZ = atan(deltaXY / deltaZ);
}



Object::Object(Pos3D pos) : Pos3D(pos), QObject() {}
Object::Object(const Object &obj) : Pos3D(obj.getPos()), QObject() { operator=(obj); }
Object::~Object() {}
Object *Object::operator =(const Object &obj) { setPos(obj.getPos()); return this; }



Block::Block(Pos3D pos, BLOCK::Type type, BLOCK::Material material) : Object(pos)
{
    this->type = type;
    this->material = material;
    calcFaces();
}
Block::~Block()
{
    while(!faces.isEmpty())
        deleteFace(faces.first());
}
Rect3D Block::getBlockGeometry(BLOCK::Type type)
{
    switch (type) {
    case BLOCK::Type::cube:
        return Rect3D(Point3D(0, 0, 0), Point3D(1, 1, 1));
    case BLOCK::Type::slab:
        return Rect3D(Point3D(0, 0, 0), Point3D(1, 1, 0.5L));
    }
    return Rect3D();
}

void Block::deleteFace(Face *face)
{
    faces.removeAll(face);
    if(face != nullptr)
        delete face;
}
void Block::calcFaces()
{
    QList<Face> faces = getFaces(this->getPoint(), type, material);
    while(!this->faces.isEmpty())
        deleteFace(this->faces.first());
    for(int i=0; i<faces.size(); i++)
        this->faces.append(new Face(faces.at(i)));
    maxGeometry = getBlockGeometry(type) + getPoint();
    middleMinGeometry = maxGeometry.getMiddle();
}
QList<Face> Block::getFaces(Point3D posBlock, BLOCK::Type type, BLOCK::Material material)
{
    switch (type) {
    case BLOCK::Type::cube:
        return {
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(1, 1, 0)), material, {BLOCK::Variation::bottom}),//BOTTOM
            Face(posBlock, Rect3D(Point3D(0, 0, 1), Point3D(1, 1, 1)), material, {BLOCK::Variation::top}),//TOP
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(0, 1, 1)), material, {BLOCK::Variation::back}),//SOUTH
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(1, 0, 1)), material, {BLOCK::Variation::right}),//EAST
            Face(posBlock, Rect3D(Point3D(1, 0, 0), Point3D(1, 1, 1)), material, {BLOCK::Variation::front}),//NORTH
            Face(posBlock, Rect3D(Point3D(0, 1, 0), Point3D(1, 1, 1)), material, {BLOCK::Variation::left})//WEST
        };
    case BLOCK::Type::slab:
        return {
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(1, 1, 0)), material, {BLOCK::Variation::bottom}),//BOTTOM
            Face(posBlock, Rect3D(Point3D(0, 0, 0.5L), Point3D(1, 1, 0.5L)), material, {BLOCK::Variation::top}),//TOP
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(0, 1, 0.5L)), material, {BLOCK::Variation::back}),//SOUTH
            Face(posBlock, Rect3D(Point3D(0, 0, 0), Point3D(1, 0, 0.5L)), material, {BLOCK::Variation::right}),//EAST
            Face(posBlock, Rect3D(Point3D(1, 0, 0), Point3D(1, 1, 0.5L)), material, {BLOCK::Variation::front}),//NORTH
            Face(posBlock, Rect3D(Point3D(0, 1, 0), Point3D(1, 1, 0.5L)), material, {BLOCK::Variation::left})//WEST
        };
    }
    return {};
}





Chunk::Chunk(Point3D pos) : Object(Pos3D(pos*chunkSize, 0, 0))
{
    calcMinMaxPoint();
}
Chunk::~Chunk()
{
    while(blocks.isEmpty())
        deleteBlock(blocks.first());
}

bool Chunk::setBlock(Block *block)
{
    if(!contains(block->getPoint()))
        return false;
    if(haveBlock(block->getPoint())) {
        removeBlock(block->getPoint());
    }
    blocks.append(block);
    calcMinMaxPoint();
    return true;
}
bool Chunk::haveBlock(const Point3D &pos) const
{ return getBlock(pos) != nullptr; }
Block *Chunk::getBlock(const Point3D &pos) const
{
    for(int i=0; i<blocks.size(); i++)
        if(blocks.at(i)->getPoint() == pos)
            return blocks.at(i);
    return nullptr;
}

bool Chunk::removeBlock(const Point3D &pos)
{
    Block *block = getBlock(pos);
    if(!block) return false;
    blocks.removeAll(block);
    delete block;
    return true;
}

bool Chunk::contains(const Point3D &pos) const { return chunkOfPos(pos) == getPoint(); }

void Chunk::calcMinMaxPoint()
{
    if(blocks.size() == 0) {
        maxGeometry = Rect3D(getPoint(), getPoint());
        return;
    }
    maxGeometry = blocks.first()->getMaxGeometry();
    doubli minX = maxGeometry.getPointMin().getX();
    doubli minY = maxGeometry.getPointMin().getY();
    doubli minZ = maxGeometry.getPointMin().getZ();
    doubli maxX = maxGeometry.getPointMax().getX();
    doubli maxY = maxGeometry.getPointMax().getY();
    doubli maxZ = maxGeometry.getPointMax().getZ();

    for(int i=0; i<blocks.size(); i++) {
        Rect3D currentMaxGeometry = blocks.at(i)->getMaxGeometry();
        if(currentMaxGeometry.getPointMin().getX() < minX)
            minX = currentMaxGeometry.getPointMin().getX();
        if(currentMaxGeometry.getPointMin().getY() < minY)
            minY = currentMaxGeometry.getPointMin().getY();
        if(currentMaxGeometry.getPointMin().getZ() < minZ)
            minZ = currentMaxGeometry.getPointMin().getZ();
        if(maxX < currentMaxGeometry.getPointMax().getX())
            maxX = currentMaxGeometry.getPointMax().getX();
        if(maxY < currentMaxGeometry.getPointMax().getY())
            maxY = currentMaxGeometry.getPointMax().getY();
        if(maxZ < currentMaxGeometry.getPointMax().getZ())
            maxZ = currentMaxGeometry.getPointMax().getZ();
    }
    maxGeometry = Rect3D(Point3D(minX, minY, minZ), Point3D(maxX, maxY, maxZ));
    //middleMinGeometry = Point3D((minX+maxX)/2, (minY+maxY)/2, minZ);
    middleMinGeometry = maxGeometry.getMiddle();
}

bool Chunk::deleteBlock(Block *block)
{
    if(!blocks.contains(block))
        return false;
    blocks.removeAll(block);
    if(block != nullptr)
        delete block;
    return true;
}



EntityAttribute::EntityAttribute(ENTITY::Type type) { this->type = type; }
EntityAttribute::EntityAttribute(const EntityAttribute &obj) { type = obj.type; speed = obj.speed; }
doubli EntityAttribute::getSpeed(ENTITY::Direction dir) const
{
    if(speed.contains(dir))
        return speed.value(dir);
    else
        return baseSpeed(type, dir);
}

Entity::Entity(const Pos3D &pos, ENTITY::Type type) : Object(pos)
{
    attribute.setType(type);
}




WorldChange::WorldChange(WorldChange::Type type, Action action) { t = type; act = action; }
WorldChange::WorldChange(const WorldChange &change)
{
    t=change.t;
    tchunk=change.tchunk;
    tblock=change.tblock;
    tentity=change.tentity;
}


World::World() {}
World::~World()
{
    while(!chunks.isEmpty())
        deleteChunk(chunks.first());
    while(!entities.isEmpty())
        deleteEntity(entities.first());
}

bool World::setBlock(Block *block)
{
    Point3D posChunk = Chunk::chunkOfPos(block->getPoint());
    if(!haveChunk(posChunk))
        if(!createChunk(posChunk)) {
            qDebug() << "ERROR Chunk::setBlock can't create a chunk";
            return false;
        }
    bool success = getChunk(posChunk)->setBlock(block);
    if(success) {
        WorldChange change(WorldChange::Type::block, WorldChange::Action::created);
        change.setBlock(block);
        change.setChunk(getChunk(posChunk));
        emit changed(change);
    }
    return success;
}
void World::addEntity(Entity *entity)
{
    if(entities.contains(entity))
        return;
    entities.append(entity);
    WorldChange change(WorldChange::Type::entity, WorldChange::Action::created);
    change.setEntity(entity);
    emit changed(change);
}

bool World::createChunk(Point3D pos)
{
    if(haveChunk(Chunk::chunkOfPos(pos)))
        return false;
    chunks.append(new Chunk(pos));
    WorldChange change(WorldChange::Type::chunk, WorldChange::Action::created);
    change.setChunk(getChunk(pos));
    emit changed(change);

    return haveChunk(Chunk::chunkOfPos(pos));
}
void World::deleteChunk(Chunk *chunk)
{
    chunks.removeAll(chunk);
    if(chunk != nullptr)
        delete chunk;
    WorldChange change(WorldChange::Type::chunk, WorldChange::Action::removed);
    change.setChunk(chunk);
    emit changed(change);
}
Chunk *World::getChunk(Point3D pos) const
{
    for(int i=0; i<chunks.size(); i++)
        if(chunks.at(i)->getPoint() == pos)
            return chunks.at(i);
    return nullptr;
}
bool World::haveChunk(Point3D pos) const
{
    return getChunk(pos) != nullptr;
}

bool World::deleteEntity(Entity *entity)
{
    if(!entities.contains(entity))
        return false;
    entities.removeAll(entity);
    if(entity != nullptr)
        delete entity;
    WorldChange change(WorldChange::Type::entity, WorldChange::Action::removed);
    change.setEntity(entity);
    emit changed(change);
    return true;
}
