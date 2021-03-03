#include "GUI.h"


GUI::GUI(const map3D *map, QObject *parent) : QGraphicsScene(parent)
{
    this->map = map;

    workerThread = new RayTracing(map);
    connect(workerThread, &RayTracing::resultReady, this, &GUI::handleWorkerResults);

    lastImage = workerThread->getImage();
}

GUI::~GUI()
{
    workerThread->quit();
    workerThread->wait(5000);
    delete workerThread;
}

// changer la taille de l'image à l'écran
void GUI::setSceneSize(const QSize &size)
{
    setSceneRect(QRect(0, 0, size.width()-2, size.height()-2));
    updateGui();
}

// garder l'image d'origine mais changer la taille de l'image à l'écran
void GUI::updateGui()
{
    setBackgroundBrush(lastImage.scaled(getSceneSize()));
}

// actualiser toute l'image (pour le GUIWorker)
void GUI::refresh()
{
    if(workerThread->isRunning()) return;

    emit workStarted();

    workerThread->setSize(getRayTracingSize())->start();
}

// toute l'image a été actualisée (par le GUIWorker)
void GUI::handleWorkerResults(const QImage &image)
{
    lastImage = image;
    updateGui();// actualise l'affichage
    emit workFinished();
}

//pourquoi je m'embete avec des ppp quand je peux juste faire un scale down ?
QSize GUI::getRayTracingSize() const
{
    return QSize(width() * RAYTRACING::pppH, height() * RAYTRACING::pppV);
}
