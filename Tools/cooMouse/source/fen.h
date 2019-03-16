#ifndef FEN_H
#define FEN_H

#include <QtWidgets>
#include <windows.h>
#include <QMouseEvent>
#include <QTimer>

#include <iostream>



class fen : public QWidget
{
    Q_OBJECT


    public:
        fen();


private slots:
        void actuTemps();
        void DeplaceMouse();
        void actuMouse();
        void actuPosRelative();




    private:
        QSpinBox *x;
        QSpinBox *y;
        QSpinBox *temps;
        QLabel *souris;
        QTimer *TimerCoo;

        QPushButton *bLancer;
        QCheckBox *posRelative;
        QCheckBox *bloquerAuMilieu;
        int xRelatif;
        int yRelatif;


};

#endif // FEN_H
