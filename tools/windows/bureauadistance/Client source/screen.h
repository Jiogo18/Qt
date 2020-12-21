#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include "ui_screen.h"
#include <windows.h>

#include <iostream>



class screen : public QWidget, private Ui_screen
{
    Q_OBJECT

public:
    explicit screen();
    void setScreen(QPixmap image, int x, int y);
    QPoint getMouse();

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void envoyerClick(QMouseEvent *event);
    void envoyerTouche(QKeyEvent *event);

};

#endif // SCREEN_H
