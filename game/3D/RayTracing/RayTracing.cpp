#include "RayTracing.h"

int RAYTRACING::functionForDebugLOL() { return 0; }


Plan::Plan() {}
Plan::Plan(const Point3D &pA, const Point3D &pB, const Point3D &pC)
{
    this->pA = pA;
    this->pB = pB;
    this->pC = pC;
    calcEquation();
}
Plan::Plan(const Rect3D &rect)
{
    this->pA = rect.getPointMin();
    //les points intermédiaires (B et C) sont à mis distance de A et de D sur Z
    //TODO: sauf si rotation...
    this->pB = Point3D(rect.getPointMin().getX(),
                       (rect.getPointMin().getY()+rect.getPointMax().getY())/2,
                       (rect.getPointMin().getZ()+rect.getPointMax().getZ())/2);
    this->pC = Point3D((rect.getPointMin().getX()+rect.getPointMax().getX())/2,
                       rect.getPointMin().getY(),
                       (rect.getPointMin().getZ()+rect.getPointMax().getZ())/2);
    calcEquation();
}
Plan::Plan(const Plan &plan) { operator=(plan); }
void Plan::calcEquation()
{
    doubli xAB = round(pB.getX() - pA.getX()),
            yAB = round(pB.getY() - pA.getY()),
            zAB = round(pB.getZ() - pA.getZ()),
            xAC = round(pC.getX() - pA.getX()),
            yAC = round(pC.getY() - pA.getY()),
            zAC = round(pC.getZ() - pA.getZ());
    //voir #EquationPlan
    a = 0; b = 0; c = 0; d = 0;
    if((xAB == 0.0L && yAB == 0.0L && zAB == 0.0L) || (xAC == 0.0L && yAC == 0.0L && zAC == 0.0L)) {
        //qDebug() << "ERROR Plan vecteur nul";
        return;
    }
    doubli nOfb=round(yAB/yAC), nOfc=round(zAB/zAC);
    if(zAB == 0.0L && zAC == 0.0L) nOfc = 0.0L;
    if(yAB == 0.0L && yAC == 0.0L) nOfb = 0.0L;
    //avec nOfb le coef qui permet à b d'être nul et nOfc pour qvoir c nul

    //bug équation du plan avec
    //A(0,0,0) B(0,2,0) C(1,0,0) => AB(0,2,0) AC(1,0,0)
    //P : z=0 => c=1 et le reste=0
    //si les 2 vecteurs sont sur le plan déssiné par 2 axes
    if(zAB == 0.0L && zAC == 0.0L) {
        c = 1;//z - d = 0
    }
    else if(yAB == 0.0L && yAC == 0.0L) {
        b = 1;//y - d = 0
    }
    else if(xAB == 0.0L && xAC == 0.0L) {
        a = 1;//x - d = 0
    }
    //si AB est un vecteur dir des axes
    else if(xAB == 0.0L && yAB == 0.0L) {
        //zAC ne sert à rien puisqu'il dessine une droite // à AB
        a = zAB*yAC;
        b = -zAB*xAC;
        c = 0;
    }
    else if(xAB == 0.0L && zAB == 0.0L) {
        a = yAB*zAC;
        b = 0;
        c = -yAB*xAC;
    }
    else if(yAB == 0.0L && zAB == 0.0L) {
        a = 0;
        b = xAB*zAC;
        c = -xAB*yAC;
    }
    //si AC est un vecteur directeur d'un des axes
    else if(xAC == 0.0L && yAC == 0.0L) {
        a = zAC*yAB;
        b = -zAC*xAB;
        c = 0;
    }
    else if(xAC == 0.0L && zAC == 0.0L) {
        a = yAC*zAB;
        b = 0;
        c = -yAC*xAB;
    }
    else if(yAC == 0.0L && zAC == 0.0L) {
        a = 0;
        b = xAC*zAB;
        c = -xAC*yAB;
    }
    //sinon
    else {
        a = 1;
        if(yAC == 0.0L)
            c = (xAC)*a/(-zAC);//nOfb == inf donc on retire et fait *0 pr l'autre
        else
            c = (xAC*nOfb-xAB)*a/(zAB-zAC*nOfb);
        if(zAC == 0.0L)
            b = (xAC)*a/(-yAC);//nOfc == inf donc on retire et fait *0 pr l'autre
        else
            b = (xAC*nOfc-xAB)*a/(yAB-yAC*nOfc);
    }
    a = round(a); b = round(b); c = round(c);
    d = - a*pA.getX() - b*pA.getY() - c*pA.getZ();
    d = round(d);
    //si il y a 2 x, y ou z ==0 pr chaque vecteur alors vecteurs colinéaires => pas de plan
}

bool Plan::paralleleDroite(const Point3D &pA, const Point3D &pB) const
{
    //voir #Orthogonal
    doubli scalaireAvecVectNormal = round(a*(pB.getX()-pA.getX())+
                                          b*(pB.getY()-pA.getY())+
                                          c*(pB.getZ()-pA.getZ()));
    if(scalaireAvecVectNormal == 0.0L)
        return true;
    double precisionLog = log10(static_cast<double>(std::abs(scalaireAvecVectNormal)));
    if(precisionLog < -10) {
        //normaement n'arrive plus avec les doubli
        //if(precisionLog > - 200)
            qDebug() << "paralleleDroite avec precision de:" << precisionLog;
        return true;
    }
    return false;
}

Point3D Plan::intersection(const Point3D &pA, const Point3D &pB) const
{
    if(paralleleDroite(pA, pB))
        return Point3D();
    //Voir #Intersection
    doubli ABx = pB.getX()-pA.getX(),
           ABy = pB.getY()-pA.getY(),
           ABz = pB.getZ()-pA.getZ();
    doubli t = -(a*pA.getX() + b*pA.getY() + c*pA.getZ() + d)
               /(a*ABx + b*ABy + c*ABz);
    if(t < 0)//derriere
        return Point3D();
    return Point3D(pA.getX() + t * ABx,
                   pA.getY() + t * ABy,
                   pA.getZ() + t * ABz);
}

QPointF Plan::projeteOrtho(const Point3D &pA) const
{
    //donne un point de position relative au plan (à la texture...)
    //un vecteur normal: m(a, b, c) donne la direction du vecteur MM'
    //un vecteur normal donne la rotation du Plan: sin(rY) = b / distance, sin(rX) = z / distance
    doubli distance = sqrt(a*a + b*b + c*c);
    Point3D pR = Pos3D(this->pA, asin(b/distance), asin(c/distance)).changeRef(pA);
    //on prend direct pA car si c'est pas un point du plan alors pR est son projeté dans l'autreref
    //(si x != 0 alors pA n'est pas sur le plan)


    return QPointF(static_cast<double>(pR.getY()), static_cast<double>(pR.getZ()));
    //ne prend pas en compte la rotation... ! (relative au point A)
}

Plan *Plan::operator =(const Plan &plan)
{
    pA = plan.pA; pB = plan.pB; pC = plan.pC;
    a = plan.a; b = plan.b; c = plan.c; d = plan.d;
    return this;
}



Ray::Ray(Pos3D pos, QMap<const Face*, const Plan*> *facesPlan, QMap<QString, const QImage*> *facesImg, DebugTime *dt)
{
    this->dt = dt;
    origin = pos;
    distParcouru = 0;
    moveTo(pos);
    distParcouru = 0;
    this->facesPlan = facesPlan;
    this->facesImg = facesImg;
}

ColorLight Ray::getColor() const
{
    if(colors.isEmpty())
        return ColorLight(0, 0, 0, 255, 0);
    ColorLight retour = colors.last();
    for(int i=colors.size()-2; i>=0; i--)
        retour = colors.at(i) + retour;
    return retour;
}

void Ray::process(const World *world)
{
    while(distParcouru < viewDistance) {//pas exactement view distance mais pas loins (2*..?)
        qint64 start = dt->getCurrent();
        Point3D pInter;
        const Face *face = getFirstIntersection(world, &pInter);
        dt->addValue("Ray::process_face", dt->getCurrent()-start);
        if(face == nullptr || !face->isValid())
            break;//c'est normal si on va dans le vide

        lastMoved = !(pInter == pos.getPoint());
        lastFace = face;

        const Plan *plan(getPlan(face));
        QPointF pTexture = plan->projeteOrtho(pInter);
        //TODO pas opti (on perd 7% à refaire le calcul...)
        QPointF pC = plan->projeteOrtho(face->getMaxGeometry().getPointMax()) - QPointF(1,1);
        pTexture -= pC;
        colors.append(face->getColor(pTexture, getTexture(face->getTexture())));

        if(face->getMaterial() == BLOCK::Material::mirror) {
            moveTo(Pos3D(pInter, face->boundRotX(pos.getRX()), face->boundRotZ(pos.getRZ())));
        }
        else {
            moveTo(Pos3D(pInter, pos.getRX(), pos.getRZ()));//TODO calculer la rotation (et si miroir ou transparant)
        }

        int alpha = colors.last().getColorA().alpha();
        opacity += alpha;
        if(opacity >= 255)
            break;//on s'arrete la
        functionForDebugLOL();
        //break;//TODO empecher de calculer après pour les tests (avoir que la première face)
        //Tips: transparant il faut penser à passer sur la face de l'autre coté... trop complexe :'(
    }
}

void Ray::moveTo(const Pos3D &pos)
{
    distParcouru += Point3D::distance(this->pos, pos);
    this->pos = pos;
    posNextPoint = pos.getNextPoint();//pour pas le recalculer pour toutes les faces
}

const Face *Ray::getFirstIntersection(const World *world, Point3D *pInter) const
{
    const Face *faceMin = nullptr;
    Point3D pInterMin;
    doubli distanceInterMin = 0;//seront set car !faceMin.isValid() au début
    doubli distanceBlockMin = 0;
    for(int i=0; i<world->getChunks().size(); i++) {
        Chunk *chunk = world->getChunks().at(i);
        if(Point3D::distanceMax(chunk->getPoint(), pos) > viewDistance)
            continue;
        qint64 start1 = dt->getCurrent();
        for(int i2=0; i2<chunk->getBlocks()->size(); i2++) {
            Block *block = chunk->getBlocks()->at(i2);
            if(Point3D::distanceMax(block->getPoint(), pos) > viewDistance)
                continue;
            for(int i3=0; i3<block->getFaces()->size(); i3++) {
                const Face *face = block->getFaces()->at(i3);
                if(face == lastFace)
                    continue;
                const Plan *plan = getPlan(face);

                Point3D pInter = plan->intersection(pos, posNextPoint);

                if(!pInter.isValid() || !face->getMaxGeometry().contains(pInter))
                    continue;
                if(!enter && pInter == pos.getPoint() && !lastMoved)
                    continue;//on viens de sortir (ou par défaut) et on se retrouve sur le même point
                doubli distanceInter = round(Point3D::distance(pos, pInter)),
                        distanceBlock = 0;
                if(faceMin != nullptr && faceMin->isValid()) {
                    if(distanceInter > distanceInterMin)
                        continue;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
                    distanceBlock = round(Point3D::distance(pos, block->getMiddleGeometry()));
                    if(distanceInter == distanceInterMin) {
                        if(distanceBlock >= distanceBlockMin)
                            continue;
#pragma clang diagnostic pop
                    }
                }
                else {
                    distanceBlock = round(Point3D::distance(pos, block->getMiddleGeometry()));
                }
                //si il n'y a pas encore de face
                //sinon si le pt d'intersection est plus proche
                //sinon si c'est le même et si la face est plus proche
                //sinon si c'est la même et si le block est plus proche
                //alors on prend
                faceMin = face;
                pInterMin = pInter;
                distanceInterMin = distanceInter;
                distanceBlockMin = distanceBlock;
                //il est possible qu'un même distance soit pour le meme block, pour des faces diff (de meme distance)
                //mais c'est pas grave car ça arrive quand c'est VRAIMENT indéterminant
                //(on peu prendre l'un ou l'autre)
            }

        }
        dt->addValue("Ray::firstIntersection_1", dt->getCurrent()-start1);

    }
    *pInter = pInterMin;
    return faceMin;
}

const Plan *Ray::getPlan(const Face *face) const
{
    if(!facesPlan->contains(face))
        facesPlan->insert(face, new Plan(face->getMaxGeometry()));
    return facesPlan->value(face);
}
const QImage *Ray::getTexture(QString file) const
{
    if(!facesImg->contains(file)) {
        QImage img = OBJECT3D::getTexture(file);
        if(img.isNull()) {
            facesImg->insert(file, nullptr);
        }
        else
            facesImg->insert(file, new QImage(img));

    }
    return facesImg->value(file);
}




RayTracing::RayTracing(const World *world, const Entity *client) : QObject()
{
    this->world = world;
    this->client = client;
    connect(world, &World::changed, this, &RayTracing::worldChanged);
    facesPlan = new QMap<const Face*, const Plan*>;
    facesImg = new QMap<QString, const QImage*>;
}

RayTracing::~RayTracing()
{
    if(facesPlan != nullptr) {
        QMapIterator<const Face*, const Plan*> i(*facesPlan);
        while(i.hasNext()) {
            i.next();
            delete i.value();
        }
        delete facesPlan;
    }
    if(facesImg != nullptr) {
        QMapIterator<QString, const QImage*> i(*facesImg);
        while(i.hasNext()) {
            i.next();
            delete i.value();
        }
        delete facesImg;
    }
}
void RayTracing::draw(const QSizeF &sceneSize)
{
    dt.clear();
    const Pos3D pos = client->getPos();
    QPixmap pixmap(qFloor(sceneSize.width()), qFloor(sceneSize.height()));
    emit start(pixmap.width());
    qDebug() << "RayTracing::draw #start";

    QVector<QVector<ColorLight>> colors(pixmap.width());
    double lightTotal = 0;
    for(int x=0; x<pixmap.width(); x++) {
        //pos en % de pixmap.width/2
        doubli xPos = round(static_cast<doubli>(2.0L*x/pixmap.width() - 1));
        emit step(x);
        for(int y=0; y<pixmap.height(); y++) {
            //pos en % de pixmap.height/2
            doubli yPos = round(static_cast<doubli>((1 - 2.0L*y / pixmap.height())));
            qint64 start = dt.getCurrent();
            colors[x].insert(y, determineColor(pos, xPos, yPos));
            lightTotal += colors.at(x).at(y).getLight();
            dt.addValue("RayTracing::draw_insert", dt.getCurrent()-start);
        }

    }

    qDebug() << "RayTracing::draw #drawcolor";
    QPainter painter;
    painter.begin(&pixmap);
    lightTotal /= (pixmap.width() * pixmap.height());//moyenne
    if(lightTotal < 1.0) lightTotal=1;
    qDebug() << "light" << lightTotal;

    for(int x=0; x<colors.size(); x++) {
        emit step(x);
        for(int y=0; y<colors.at(x).size(); y++) {
            QColor color = colors.at(x).at(y).getColorReduced(lightTotal);
            painter.setPen(QPen(QBrush(color), 1));
            painter.drawPoint(x, y);
        }
    }
    painter.end();
    qDebug() << "RayTracing::draw #end" << dt;
    emit finished(pixmap);
}

void RayTracing::worldChanged(const WorldChange &change) {
    switch (change.type()) {
    case WorldChange::Type::block:
    case WorldChange::Type::chunk: {
            QMapIterator<const Face*, const Plan*> i(*facesPlan);
            while(i.hasNext()) {
                i.next();
                QPointer<const Face> pointer(i.key());
                if(pointer.isNull())//if it's destroyed or undefined
                    facesPlan->remove(i.key());
            }
        }
        break;
    case WorldChange::Type::entity:
        break;
    }
}

ColorLight RayTracing::determineColor(const Pos3D &pos, doubli xPos, doubli yPos)
{
    int red = 0, blue = 0, green = 0;
    int light = 0;//une sorte de moyenne pour privilégier le poids des très lumineux
    //en théorie il faudrait que les rayons soient random (pour le miroir et la transparence) mais pas grave
    doubli xPos2 = xPos * xMax;
    doubli yPos2 = yPos * yMax;
    for(int ix = 0; ix < pppH; ix++) {
        doubli xPos3 = xPos2 + ix/pppH * xMax;
        doubli angleH = atan(xPos3);
        for(int iy = 0; iy < pppV; iy++) {
            doubli yPos3 = yPos2 + iy/pppV * yMax;
            doubli d = sqrt(1 + sqr(xPos3) + sqr(yPos3));
            doubli angleV = asin(yPos3 / d);

            qint64 start = dt.getCurrent();
            Ray ray(pos.getChildRot(angleH, angleV), facesPlan, facesImg, &dt);
            dt.addValue("RayTracing::determineColor_1", dt.getCurrent()-start);
            start = dt.getCurrent();
            ray.process(world);
            dt.addValue("RayTracing::determineColor_2", dt.getCurrent()-start);
            ColorLight colorL = ray.getColor();
            QColor colorA = colorL.getColorA();
            red += colorA.red() * colorA.alpha()/255;
            green += colorA.green() * colorA.alpha()/255;
            blue += colorA.blue() * colorA.alpha()/255;
            light += colorL.getLight();
        }
    }
    /*if(light == 0)//aucune source de lumiere
        return ColorLight(0, 0, 0, 255, 0);*/
    if(light < 1)
        light = ppp;//TODO retirer ça et remettre celui avant

    return ColorLight(red/ppp, green/ppp, blue/ppp, 255, light/ppp);
}

