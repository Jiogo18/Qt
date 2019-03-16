#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "ui_fenetre.h"
#include <QFile>
#include <QStringList>
#include <QTime>


class fenetre : public QWidget, private Ui::fenetre
{
    Q_OBJECT

    public:
        fenetre();
        void testMot(QString mot);

    private slots:
        void parcourir();
        void calculer();
        void stop();
        void actuNbChrs();

    //signals:
        //void analyse(QString);

    private:
        bool execution;
        QStringList *mots;

};

#endif // FENETRE_H
