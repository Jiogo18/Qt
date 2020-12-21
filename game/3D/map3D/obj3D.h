#ifndef OBJ3D_H
#define OBJ3D_H

#include <QWidget>
#include <QList>
#include "QPoint3D.h"
#include <QtMath>

class obj3D : public QPoint3D//la pos principalle de l'obj3D
{
public:
    obj3D();
    obj3D(double x, double y, double z, int rX, int rY);
    obj3D(QList<QPoint3D> points, double x=0, double y=0, double z=0, int rX=0, int rY=0);
    obj3D(const obj3D &obj);
    int rX() const;
    int rY() const;
    void setRX(int rX);
    void setRY(int rY);
    QList<QPoint3D> getPoint() const;
    void append(QPoint3D point);
    const QPoint3D &at(quint64 i) const;
    QPoint3D &operator[](quint64 i);
    QPoint3D &mergePoints(quint64 i);
    void operator =(const obj3D &obj);
    void operator =(const QPoint3D &obj);
    void calculRotation();
    static double rotation1(double x, double y, int r);
    static double rotation2(double x, double y, int r);
    void addTexture(QList<quint64> pts, QString fileName);
    void setTexture(const QMap<QList<quint64>, QString> &texture);
    QMap<QList<quint64>, QString> getTexture() const;
    QList<quint64> getTextureAt(quint64 at) const;//trie les points en fonction de z
    QList<QList<quint64> > getTextureSortZ() const;
    QPoint3D getMoyenneTexture(QList<quint64> face) const;

    void debug() const;

private:
    void calcRotation();
    QPoint3D posObjTemp;
    QList<QPoint3D> pointsP;//pos relatives de l'objet
    QList<QPoint3D> pointsRP;//pose tournée de l'objet
    int rXP=0;
    int rYP=0;
    QMap<QList<quint64>, QString> textures;//on peut pas avoir plusieurs fois la meme texture
};

#endif // OBJ3D_H
