#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "GraphicsButton.h"
#include "Block.h"
#include <QString>
#include <QList>


class fenetre : public QWidget
{
    Q_OBJECT
    public:
        fenetre();
        void newRessourcepack(QString nressourcepack);
        void removeButtons();
        void setBackground(QString limage);

        void openMenu();
        void openSolo();
        void openMulti();
        void openOption();


    public slots:
        void buttonClicked(QString action);

    signals:
        void actuRessourcepack(QString nressourcepack);


    private:
        QGraphicsScene *scene;
        QGraphicsView *view;
        QString ressourcepack;
        QList<GraphicsButton*> *boutons;
        bool inGame;
        bool optionOpen;
        int etat;/*!optionOpen : !inGame : 1 menu, 2 menu solo, 3 new world, 4 menu multi, 5 new serveur
                                  inGame : 1 chargement, 2 chargé
                    optionOpen : !inGame : 1 principale, 2 sounds, 3 controles
                                  inGame :               //                     */
        float version;

};

#endif // FENETRE_H
