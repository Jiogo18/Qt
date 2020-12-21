#ifndef QPOINT3D_H
#define QPOINT3D_H

#include <QString>

class QPoint3D
{
public:
    QPoint3D();
    QPoint3D(const QPoint3D &point);
    QPoint3D(double xPos, double yPos, double zPos);
    double x() const;
    double y() const;
    double z() const;
    double perspective() const;
    double xPerspective() const;
    double yPerspective() const;
    double zPerspective() const;
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setPerspective(double width, double height, double perspective, double zoom);
    static double moyenne(QList<double> l);

    void debug() const;
    //bool operator <(const QPoint3D &pt2) const;
    QString getDebug() const;

private:
    double xP;
    double yP;
    double zP;
    double widthP;
    double heightP;
    double perspectiveP;
    double zoomP;
};

#endif // QPOINT3D_H
