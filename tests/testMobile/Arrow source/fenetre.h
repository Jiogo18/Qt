#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include <windows.h>
#include <QTimer>

class fenetre : public QWidget
{
    Q_OBJECT
    public:
        fenetre();
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);

    public slots:
        void sliderChange(int value);
        void deplace();

    public:
        QLabel *sliderPixMoveLbl;
        QSlider *sliderPixMove;
        QTimer *move;
        bool xP;
        bool xM;
        bool yP;
        bool yM;
};

#endif // FENETRE_H
