#ifndef CONNEXION_H
#define CONNEXION_H

#include <QDialog>
#include <QtWidgets>
#include "ui_connexion.h"
#include <QString>
#include <QtNetwork>
#include <QAbstractSocket>


class connexion : public QDialog, private Ui::connexion
{
    Q_OBJECT

public:
    connexion();
    QString getServeur();
    QString getMdpServ();
    int getPort();
    void setEtat(QString rEtat, int rTypeEtat = 0);

    QString getPseudo();
    QString getMdp();
    void affich();
    bool pseudoValide(QString pseudo);

private slots:
    void connection();
    void fermer();


signals:
    void SBConnexion();

private:
    QString Dserv;
    QString DmdpServ;
    int Dport;
    QString Dpseudo;
    QString Dmdp;

};

#endif // CONNEXION_H
