#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QDateTime>

class fenetre : public QWidget
{
    Q_OBJECT
    public:
        fenetre();
        int erreur(QString fichier);
        void copier(QString fileIn, QString fileOut, QString fichier, QString updateFichier);
        void updateMsgFichier(QString updateFichier);
        void touche(int touche, int etat=0);
        void ecrire(QString texte);

    public slots:
        void lancer();


    private:
        QLabel *lblFichiers;
        QCheckBox *releaseClient;
        QCheckBox *releaseServeur;
        QCheckBox *winrar;
        QPushButton *bLancer;
};

#endif // FENETRE_H
