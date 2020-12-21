#ifndef MAP3D_H
#define MAP3D_H

#include "Object3D.h"
#include "Transfo.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "RayTracing.h"
#include <QThread>

class map3D : public QObject
{
    Q_OBJECT

    QThread rayThread;
public:
    map3D(QGraphicsScene *scene);
    ~map3D();
    Entity *getClient() { return client; }
    void refresh();
    void resizeEvent(const QSize &size);
public slots:
    void rayFinished(const QPixmap &pixmap);
    void rayStarted(int maxInfo) { emit PBMax(maxInfo); }
    void rayStep(int stepInfo) { emit PBValue(stepInfo); }
signals:
    void clientChanged(const Entity *client);
    void draw(const QSizeF &sceneSize);
    void finished();
    void PBMax(int max);
    void PBValue(int value);

private:
    Entity *client = nullptr;
    World *world = nullptr;
    QGraphicsScene *scene = nullptr;
    QGraphicsPixmapItem *pixmap = nullptr;
    RayTracing *rayt = nullptr;
    bool rayOnCalc = false;
    QPixmap lastPixmap;
    QSize lastSize;
};

#endif // MAP3D_H
