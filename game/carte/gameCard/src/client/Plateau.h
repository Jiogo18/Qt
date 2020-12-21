#ifndef PLATEAU_H
#define PLATEAU_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMap>
#include <QString>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QVBoxLayout>

class Plateau : public QWidget
{
    Q_OBJECT
public:
    Plateau(QWidget *parent = nullptr);
    ~Plateau();

    void actuPlateau();
    void loadPicture();

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;
    QTimer *timerActu = nullptr;

    QMap<QString, QImage> image;
    QMap<QString, QGraphicsPixmapItem*> graphicsPixmap;
};

#endif // PLATEAU_H
