#ifndef PARAMETRE_H
#define PARAMETRE_H

#include <QtWidgets>
#include "ui_parametre.h"
#include <QFile>
#include <QTextStream>

class parametre : public QDialog, public Ui::parametre
{
    Q_OBJECT

    public:
        parametre();
        bool getChatErreur();
        bool getChatCarteOuRien();
        bool getChatCartes();
        bool getChatJoueur();
        bool getChatPass();
        bool getChatReset();
        bool getChatUltimeCarte();
        qreal getSonVolumeStart();
        qreal getSonVolumeNext();
        qreal getSonVolumeWin();
        qreal getSonVolumeChat();
        qreal getSonVolumeAlerte();
        int getSonIntervalAlerte();

        QString getValeurFichier(QString *fichier, QString variable);//get la valeur du fichier
        QString getStrValeur(int type, bool activate, int value);//get la valeur pour un fichier

    private slots:
        void updateFichier();

    signals:
        void addErreur(QString, QString);


};

#endif // PARAMETRE_H
