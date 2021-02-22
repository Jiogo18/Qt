#ifndef POINT3D_H
#define POINT3D_H


#include <QDebug>
#include <QtMath>
#include "DebugTime.h"


typedef long double doubli;//un nombre pour gérer 6 décimales max (arrondit)
static doubli round(const doubli &d) { return std::floor(d*1000000+0.5L)/1000000; }
doubli sqr(const doubli &d);//easy
doubli degreesToRadians(const doubli &deg);
doubli radiansToDegrees(const doubli &rad);
doubli sqrt(const doubli &d);
doubli cos(const doubli &deg);
doubli sin(const doubli &deg);
doubli tan(const doubli &deg);
doubli acos(const doubli &d);
doubli asin(const doubli &d);
doubli atan(const doubli &d);
QDebug operator <<(QDebug debug, const doubli &d);

class Point3D
{
public:
    Point3D();
    Point3D(doubli x, doubli y, doubli z);
    Point3D(const Point3D &point);
    Point3D *operator =(const Point3D &point);

    doubli getX() const { return x; }
    doubli getY() const { return y; }
    doubli getZ() const { return z; }
    Point3D getPoint() const { return *this; }
    void setX(doubli x) { this->x = round(x); }
    void setY(doubli y) { this->y = round(y); }
    void setZ(doubli z) { this->z = round(z); }
    void setPoint(const Point3D &point) { operator=(point); }
    void addX(doubli x) { this->x = round(this->x + x); }
    void addY(doubli y) { this->y = round(this->y + y); }
    void addZ(doubli z) { this->z = round(this->z + z); }

    doubli distanceAxeZ() const { return std::sqrt(x*x + y*y); }
    bool operator <(const Point3D &point) const;
    bool operator >(const Point3D &point) const;
    bool operator ==(const Point3D &point) const;
    bool isNull() const { return x==0.0L && y==0.0L && z==0.0L; }
    bool isValid() const { return defined; }

    Point3D operator +(const Point3D &point) const;
    Point3D operator -(const Point3D &point) const;
    Point3D operator *(const Point3D &point) const;
    Point3D operator *(doubli n) const;
    Point3D operator /(const Point3D &point) const;
    Point3D operator /(doubli n) const;

    static doubli distance(const Point3D &pA, const Point3D &pB);
    static doubli distanceMax(const Point3D &pA, const Point3D &pB);

private:
    doubli x;
    doubli y;
    doubli z;
    bool defined = false;
};
QDebug operator << (QDebug debug, const Point3D &point);
Point3D qFloor(const Point3D &point);
Point3D qCeil(const Point3D &point);


class Pos3D : public Point3D {
public:
    Pos3D();
    Pos3D(doubli x, doubli y, doubli z, doubli rX, doubli rZ);
    Pos3D(const Point3D &point, doubli rX, doubli rZ);
    Pos3D(const Pos3D &pos);
    Pos3D *operator=(const Pos3D &pos);

    doubli getRX() const { return rX; }//en degré
    doubli getRZ() const { return rZ; }//en degré
    Pos3D getPos() const { return *this; }
    void setRX(doubli rX) { this->rX = round(rX); }
    void setRZ(doubli rZ) { this->rZ = round(rZ); }
    void setPos(const Pos3D &pos) { operator=(pos); }
    void addRX(doubli rX) { this->rX = round(this->rX + rX); }
    void addRZ(doubli rZ) { this->rZ = round(this->rZ + rZ); }

    void moveWithRot(doubli speed, doubli rot);
    static Point3D pointFromRot(doubli d, doubli rX, doubli rZ);
    Point3D getNextPointRelatif() const;
    Point3D getNextPoint() const;
    Pos3D getChildRot(doubli rXRelatif, doubli rZRelatif) const;
    Point3D changeRef(const Point3D &point) const;
private:
    doubli rX;//en degré
    doubli rZ;//en degré

    static Point3D rotation(Point3D point, doubli rX, doubli rZ);
    static doubli rotation1(doubli x, doubli y, doubli sR, doubli cR);
    static doubli rotation2(doubli x, doubli y, doubli sR, doubli cR);
    static Pos3D getRotAsVect(const Point3D &p1, const Point3D &p2);
    static Pos3D getRotAsPoint(const Point3D &p);

};
QDebug operator << (QDebug debug, const Pos3D &pos);

class Size3D
{
public:
    Size3D();
    Size3D(doubli width, doubli height, doubli depth);
    Size3D(const Size3D &size);
    Size3D(const Point3D &pA, const Point3D &pB);
    doubli getWidth() const { return width; }
    doubli getHeight() const { return height; }
    doubli getDepth() const { return depth; }
    Size3D getSize() const { return *this; }
    void setWidth(doubli width) { this->width = round(width); }
    void setHeight(doubli height) { this->height = round(height); }
    void setDepth(doubli depth) { this->depth = round(depth); }
    Size3D operator *(doubli scale) const { return Size3D(width*scale, height*scale, depth*scale); }
    Size3D operator /(doubli scale) const { return Size3D(width/scale, height/scale, depth/scale); }
protected:
    doubli width;//X
    doubli height;//Y
    doubli depth;//Z
};
Point3D operator -(const Point3D &p, const Size3D &s);
Point3D operator +(const Point3D &p, const Size3D &s);

class Rect3D
{
public:
    Rect3D();
    Rect3D(const Point3D &pointA, const Point3D &pointB);
    Rect3D(const Point3D &pointA, const Size3D &size);
    Rect3D(const Rect3D &rect);
    Size3D getSize() const { return Size3D(pointMin, pointMax); }
    Point3D getPointMin() const { return pointMin; }
    Point3D getPointMax() const { return pointMax; }
    Point3D getMiddle() const { return (pointMin + pointMax) / 2; }

    //void setPointMin(const Point3D &pointMin) { this->pointMin = pointMin; }//pas recommandé
    //void setPointMax(const Point3D &pointMax) { this->pointMax = pointMax; }//pas recommandé
    void scale(double scale);
    Rect3D operator +(const Point3D &pointMinAdd) const { return Rect3D(pointMin+pointMinAdd, getSize()); }
    Rect3D *operator =(const Rect3D &rect) { pointMin=rect.pointMin; pointMax=rect.pointMax; return this; }
    bool operator ==(const Rect3D &rect) const { return pointMin == rect.pointMin && pointMax == rect.pointMax; }

    bool contains(const Point3D &point) const;

private:
    Point3D pointMin;
    Point3D pointMax;
};
QDebug operator << (QDebug debug, const Rect3D &rect);



#endif // POINT3D_H
