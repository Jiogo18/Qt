#include "QPoint3D.h"
#include <QDebug>


QPoint3D::QPoint3D()
{xP=0; yP=0; zP=0;
widthP=0;heightP=0; perspectiveP=0; zoomP=0;}

QPoint3D::QPoint3D(const QPoint3D &point)
{
    xP=point.x(); yP=point.y(); zP=point.z();
    widthP=point.widthP;
    heightP=point.heightP;
    perspectiveP=point.perspectiveP;
    zoomP=point.zoomP;
}

QPoint3D::QPoint3D(double xPos, double yPos, double zPos)
{xP=xPos; yP=yPos; zP=zPos;
widthP=0;heightP=0; perspectiveP=0; zoomP=0;}

double QPoint3D::x() const {return xP;}
double QPoint3D::y() const {return yP;}
double QPoint3D::z() const {return zP;}

double QPoint3D::perspective() const
{
    //return zoomP;//plus de perspective
    double d=-(perspectiveP*heightP)/z()/2*zoomP;//calculé avec mc (google sheet : Minecraft perspective)
    if(d<0)
        d=-d;
    return d;
}

double QPoint3D::xPerspective() const
{return x()*perspective() + widthP/2;}//x par le zoom et la perspective +au milieu
double QPoint3D::yPerspective() const
{return y()*perspective() + heightP/2;}//pareil pour y
double QPoint3D::zPerspective() const {return z()*zoomP;}//z juste pour perspective


void QPoint3D::setX(double x) {xP=x;}
void QPoint3D::setY(double y) {yP=y;}
void QPoint3D::setZ(double z) {zP=z;}
void QPoint3D::setPerspective(double width, double height, double perspective, double zoom)
{
    widthP=width;
    heightP=height;
    perspectiveP=perspective;
    zoomP=zoom;
}

double QPoint3D::moyenne(QList<double> l)
{
    double retour=0;
    for(int i=0; i<l.size(); i++)
        retour += l.at(i);
    return retour/l.size();
}

void QPoint3D::debug() const
{qDebug()<<"x="<<x()<<"y="<<y()<<"z="<<z();}

/*bool QPoint3D::operator <(const QPoint3D &pt2) const
{//on fait pas de calcul, pas important (sinon regarder la distance depuis 0)
    //qDebug()<<"<"<<pt2.z() - (int)(pt2.z()/2)*2;
    //return pt2.z() - (int)(pt2.z()/2)*2 > 0;//1 sur 2
    return false;
}*/

QString QPoint3D::getDebug() const
{
    return QString("(x="+QString::number(x())+
                   " y="+QString::number(y())+
                   " z="+QString::number(z())+
                   ")");
}
