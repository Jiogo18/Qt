#ifndef FENETRE_H
#define FENETRE_H

#include "GUI.h"
#include "map3D.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QPoint>
#include <QThread>
//#include <QWinTaskbarButton>//need 'winextras' removed in Qt 6.0.0
//#include <QWinTaskbarProgress>


class fenetre : public QGraphicsView
{
public:
    fenetre(QWidget *parent = nullptr);
    ~fenetre() override;
    void actualise();

    void keyPressEvent(QKeyEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    //void showEvent(QShowEvent *event) override { Q_UNUSED(event) button->setWindow(this->windowHandle()); }
//    QPoint MidWindow();
//    void moveMouseMidWindow();

public slots:
    void workStarted();
    void workFinished();
//    void setPBMax(int max);
//    void setPBValue(int value);
private:
    QTimer timerRefresh;
    qint64 lastRefreshTime;
    qint64 lastRefreshDuration;
    map3D *map = nullptr;
    QThread GUIThread;
    GUI *gui = nullptr;

    //QWinTaskbarButton *winTaskbarButt = nullptr;

//    QPoint posMouse;
    doubli MouseSensibility = 100;
};

#endif // FENETRE_H
