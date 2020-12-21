#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "ui_fenetre.h"
#include <QSoundEffect>
#include <QSound>
#include <QFile>
#include <QTime>
#include <iostream>
#include "windows.h"
#include <QtMath>

#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>

class fenetre : public QWidget, private Ui::fenetre
{
    Q_OBJECT

    public:
        fenetre();
        void debug(QString str);
        void closeEvent(QCloseEvent *event);

    public slots:
        void slot1();
        void slot2();
        void slot3();
        void slot4();
        void slot4_2();
        void slot5();
        void slotImg();
        //void actWindow();
        double moyenne(QList<double> l);

    private:
        QTime *TimeExec;
        QAudioInput *myQAudioInput;
        QAudioOutput *myQAudioOutput;
        QPixmap img;
        QGraphicsScene *scene;
        QGraphicsPixmapItem *imgItem;
        QGraphicsPixmapItem *centreItem;
        QPixmap centre;

};

#endif // FENETRE_H
