#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "RSA.h"
#include <QMap>
#include <QRandomGenerator64>
#include <QRandomGenerator>
//#include "actuTask.h"

//#include "intBig.h"//test de toQString() pour convertisseurBases, peut retirer après

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

    public:
        explicit fenetre(QWidget *parent = 0);
        ~fenetre();
        void closeEvent(QEvent *event);
        void stopGenerer(QString resultat);
        void generer();
        void test();
        void genererRandom();
        void envoyerGACD();//envoyer G à CD (n, d et e)

        void chiffrerD();
        void chiffrerE();
        void chiffrerDP();
        void chiffrerEP();
        void debug(QString str);
        void actuWindow();

        void crackerMsg();


    private:
        Ui::fenetre *ui;
        //QTimer *actuT;
        //actuTask *actu;
};

#endif // FENETRE_H
