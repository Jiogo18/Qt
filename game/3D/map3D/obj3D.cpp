#include "obj3D.h"
#include <QDebug>


obj3D::obj3D() : QPoint3D(0, 0, 0)
{
    rXP=0;
    rYP=0;
    pointsP.clear();//donc pas besoin de calcul rotation
    textures.clear();
}
obj3D::obj3D(double x, double y, double z, int rX, int rY) : QPoint3D(x, y, z)
{
    rXP=rX;
    rYP=rY;
    pointsP.clear();//donc pas besoin de calcul rotation
    textures.clear();
}
obj3D::obj3D(QList<QPoint3D> points, double x, double y, double z, int rX, int rY) : QPoint3D(x, y, z)
{
    rXP=rX;
    rYP=rY;
    pointsP=points;
    calculRotation();
    textures.clear();
}
obj3D::obj3D(const obj3D &obj) : QPoint3D(obj.x(), obj.y(), obj.z())
{
    rXP=obj.rX();
    rYP=obj.rY();
    pointsP=obj.getPoint();
    calculRotation();
    textures=obj.getTexture();
}

int obj3D::rX() const {return rXP;}
int obj3D::rY() const {return rYP;}
void obj3D::setRX(int rX) {rXP=rX;}
void obj3D::setRY(int rY) {rYP=rY;}

QList<QPoint3D> obj3D::getPoint() const {return pointsP;}

void obj3D::append(QPoint3D point) {pointsP.append(point); calculRotation();}

const QPoint3D &obj3D::at(quint64 i) const
{
    if(i>=(quint64)pointsRP.size())//calculRotation(); a faire avant
    {
        QPoint3D retourNul=QPoint3D(x(), y(), z());
        return retourNul;
    }
    return pointsRP.at(i);
}

QPoint3D &obj3D::operator[](quint64 i)
{
    calculRotation();//calculRotation(); fait car on peut modif, auttant en profiter
    if(i>=(quint64)pointsRP.size())
    {
        QPoint3D retourNul=QPoint3D(x(), y(), z());
        return retourNul;
    }
    return pointsRP[i];
}

QPoint3D &obj3D::mergePoints(quint64 i)
{
    if(i>=(quint64)pointsP.size())
    {
        QPoint3D retourNul=QPoint3D(x(), y(), z());
        return retourNul;
    }
    return pointsP[i];
}

void obj3D::operator =(const obj3D &obj)
{
    setX(obj.x());
    setY(obj.y());
    setZ(obj.z());
    rXP=obj.rX();
    rYP=obj.rY();
}

void obj3D::operator =(const QPoint3D &obj)
{setX(obj.x()); setY(obj.y()); setZ(obj.z());}

void obj3D::calculRotation()
{
    pointsRP=pointsP;//on le set pour la taille
    /*désactivé pour le début, le réactiver si vous voulez tourner un objet
    for(int i=0; i<pointsP.size(); i++) //liste, pas le centre
    {
        double x1=rotation1(pointsP.at(i).x(), pointsP.at(i).z(), rXP);
        pointsRP[i].setZ(rotation2(pointsP.at(i).x(), pointsP.at(i).z(), rXP));
        pointsRP[i].setX(x1);//rotation x-z
        double y1=rotation2(pointsRP.at(i).z(), pointsP.at(i).y(), rYP-90);
        pointsRP[i].setZ(rotation2(pointsRP.at(i).z(), pointsP.at(i).y(), rYP-90));
        pointsRP[i].setY(y1);//rotation z-y
    }*/
}

/* fait avec https://www.geogebra.org/classic/ntmgqtxr : (utilisé la version calculette)
 * f1(x,y,r)=x*cos(r)-y*sin(r);
 * f2(x,y,r)=x*sin(r)+y*cos(r)
 * fx(x,y,z)=f1(x,y,r1)zoom;
 * fy(x,y,z)=f1(f2(x,y,r1),z,r2)zoom
 * fz(x,y,z)=f2(f2(x,y,r1),z,r2)zoom;*/

double obj3D::rotation1(double x, double y, int r)//en degré
{
    double sR=qSin(qDegreesToRadians((double)r));//obligé de mettre *1000 car sinon aime pas les doubles
    double cR=qCos(qDegreesToRadians((double)r));
    return x*cR-y*sR;
}

double obj3D::rotation2(double x, double y, int r)//en degré
{
    double sR=qSin(qDegreesToRadians((double)r));//obligé de mettre *1000 car sinon aime pas les doubles
    double cR=qCos(qDegreesToRadians((double)r));
    return x*sR+y*cR;
}

void obj3D::addTexture(QList<quint64> pts, QString fileName)
{//p4 doit faire un parallèlogramme, ça change rien
    if(textures.contains(pts))//marcheras pas bc, les faces peuvent se mélanger à volontée
        return;
    textures.insert(pts, fileName);
}
void obj3D::setTexture(const QMap<QList<quint64>, QString> &texture) {textures=texture;}
QMap<QList<quint64>, QString> obj3D::getTexture() const {return textures;}

QList<quint64> obj3D::getTextureAt(quint64 at) const
{return getTextureSortZ().at(at);}
QList<QList<quint64>> obj3D::getTextureSortZ() const
{//on veut trier la QMap en mettant les sommets ds un certains ordre
    /*QList<QList<quint64>> listeTextures;
    QList<double> listeTexturesZ;//des faces on la meme moyenne donc pas de map possible
    //on va les trier selon le z moyens de chaque face
    QMapIterator<QList<quint64>, QString> i(textures);
    while(i.hasNext())
    {
        i.next();
        if(i.key().isEmpty())
            continue;
        double zMoyen=0;
        for(int i2=0; i2<i.key().size(); i2++)//get la moyenne de z
            zMoyen+=pointsRP.at(i.key().at(i2)).z();
        zMoyen/=i.key().size();
        listeTextures.append(i.key());//pk le i.key n'est pas ajouté par le listeTextures (inaccessible)?
        listeTexturesZ.append(zMoyen);//on classe dans 2 listes
    }

    QList<QList<quint64>> listeTextures2;
    QList<double> listeTexturesZ2;//on trie*/
    /*for(int i2=0; i2<listeTextures.size(); i2++)
    {
        listeTextures2.append({0});
        listeTexturesZ2.append(0);
    }*/

    /*for(int i2=0; i2<listeTextures.size(); i2++)
    {
        int iT=0;
        for(iT=0; iT<listeTextures.size()-1; iT++)
        {
            if(listeTexturesZ2.size()<=iT)//si il y a pas asser de cases, on le met à la fin
                break;
            if(listeTexturesZ2[iT] > listeTexturesZ[i2])//si le prochain est plus grand, on s'arrete la
                break;
        }
        listeTextures2.insert(iT, listeTextures.at(i2));
        listeTexturesZ2.insert(iT, listeTexturesZ.at(i2));
    }
    return listeTextures2;*/
    QMultiMap<double, QList<quint64>> listeTextures;
    //on va les trier selon le z moyens de chaque face
    QMapIterator<QList<quint64>, QString> i(textures);
    while(i.hasNext())
    {
        i.next();
        if(i.key().isEmpty())
            continue;


        QList<double> zMoyen;
        for(int i2=0; i2<i.key().size(); i2++)//get la moyenne de z
            zMoyen.append(pointsRP.at(i.key().at(i2)).z());
        //zMoyen/=i.key().size();
        listeTextures.insert(QPoint3D::moyenne(zMoyen), i.key());
    }
    return listeTextures.values();
}

QPoint3D obj3D::getMoyenneTexture(QList<quint64> face) const
{
    QPoint3D moyenne;
    for(int i=0; i<face.size(); i++)
    {
        moyenne.setX(moyenne.x()+at(face.at(i)).xPerspective());
        moyenne.setY(moyenne.y()+at(face.at(i)).yPerspective());
        moyenne.setZ(moyenne.z()+at(face.at(i)).zPerspective());
    }
    moyenne.setX(moyenne.x()/face.size());
    moyenne.setY(moyenne.y()/face.size());
    moyenne.setZ(moyenne.z()/face.size());
    return moyenne;
}

void obj3D::debug() const
{
    for(int i=0; i<pointsP.size(); i++)
        qDebug() << "point" << i << "x=" << pointsP.at(i).x() << "y=" << pointsP.at(i).y() << "z=" << pointsP.at(i).z();
}
