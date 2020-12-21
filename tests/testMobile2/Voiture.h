#ifndef VOITURE_H
#define VOITURE_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QtMath>
#include <QQmlApplicationEngine>
#include "QmlItem.h"

class Voiture : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    Voiture(const Voiture &v);
    Voiture(QQuickItem *parent = 0);
    Voiture(QQmlApplicationEngine *engine, qreal x, qreal y, qreal width, qreal height);
    static Voiture *create(QQmlApplicationEngine *engine);
    Voiture &operator=(const Voiture &v);
    Voiture &operator=(const Voiture *v);
    ~Voiture();
    void paint(QPainter *painter);

    qreal x() const;
    void setX(qreal x);
    qreal y() const;
    void setY(qreal y);

    const QImage &image() const;
    void setImage(QImage img, QSizeF size=QSize());
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

    void turnCar(qreal rot=10);
    void moveCar(qreal distance=1);


    void turnLeft() {turnCar(-10);}
    void turnRight() {turnCar(10);}
    void forward() {moveCar(10);}
    void back() {moveCar(-10);}


    //Q_INVOKABLE void onClick();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


signals:
    void moved();
    void turned();
    void voitChanged();


private:
    void actuImg();
    QImage m_img;
    QImage m_baseImg;
};

QDebug operator<<(QDebug debug, const Voiture &v);

#endif // VOITURE_H
