#ifndef FENETRE_H
#define FENETRE_H


#include "map3D.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QPoint>
#include <QPixmap>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>


class fenetre : public QGraphicsView
{
public:
    fenetre(QWidget *parent = nullptr);
    ~fenetre() override;
    void actualise();

    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override { Q_UNUSED(event) button->setWindow(this->windowHandle()); }
    QPoint MidWindow();
    void moveMouseMidWindow();

public slots:
    void workStarted();
    void workFinished();
    void setPBMax(int max);
    void setPBValue(int value);
private:
    QTimer timerRefresh;
    map3D *map = nullptr;
    QPoint posMouse;
    QGraphicsScene *scene;
    QWinTaskbarButton *button = nullptr;

    doubli MouseSensibility = 100;
};

#endif // FENETRE_H
