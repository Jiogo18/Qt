#ifndef GRAPHICSCENE3D_H
#define GRAPHICSCENE3D_H

#include <QtWidgets>
#include <QImage>
#include "obj3D.h"
#include "QPoint3D.h"

class GraphicScene3D
{
public:
    GraphicScene3D();
    void addObj(obj3D &obj);
    void addCube(quint64 x, quint64 y, quint64 z, QString fileNameTop="", QString fileNameBottom="", QString fileNameFace="");
    obj3D calcFace(const obj3D &obj);
    QPoint3D calcPoint(QPoint3D point, QPoint3D ptplus=QPoint3D(0,0,0));
    QPixmap refresh(quint64 width, quint64 height, double x, double y, double z, int rX, int rY, double zoom, double perspective);
    QList<obj3D> refreshCalc(quint64 width, quint64 height, double zoom, double perspective);
    QImage refreshDraw(QImage img, QList<obj3D> obj);
    QImage compileImage(QImage source, quint64 x, quint64 y, QImage plus);//i2 pour le test, avec qu'une face claire
    QImage calcImg(obj3D obj, QList<quint64> pts, QString fileName, QPointF &ptCentre);
    QImage drawAxes(QImage img);
    QImage drawLigne(QImage img, QPoint3D pt0, QPoint3D pt1, QString file);
    QImage loadImg(QString fileName);
    QImage drawCamera(QImage img);

private:
    obj3D *client;//la caméra
    QImage imgCamera;
    QList<obj3D> *objets;
    const QString ressourcepacks="C:/Qt/MesProg/jeux/map3D/ressourcepacks/";
    QString ressourcepack="defaut/";
};

#endif // GRAPHICSCENE3D_H
