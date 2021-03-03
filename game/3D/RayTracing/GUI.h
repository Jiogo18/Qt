#ifndef GUI_H
#define GUI_H

#include "map3D.h"
#include "RayTracing.h"
#include <QGraphicsScene>
#include <QThread>
#include <QRandomGenerator>


class GUI : public QGraphicsScene
{
    Q_OBJECT
public:
    GUI(const map3D *map, QObject *parent = nullptr);
    ~GUI();


    void setSceneSize(const QSize &size);
    QSize getSceneSize() const { return QSize(qFloor(width()), qCeil(height())); }
    void updateGui();
    void refresh();


signals:
    void workStarted();
    void workFinished();
    void resizeWorker(const QSize &size);

private:
    void handleWorkerResults(const QImage &image);
    QSize getRayTracingSize() const;

    QImage lastImage;
    RayTracing *workerThread;
    const map3D *map;
};

#endif // GUI_H
