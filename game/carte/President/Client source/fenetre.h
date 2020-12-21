#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QtWidgets>
#include "ui_fenetre.h"
#include <QString>
#include <QVector>
#include "connexion.h"
#include "information.h"
#include <QSoundEffect>
#include <QTimer>

#include <QtNetwork>


#include "parametre.h"
#include "cartes.h"



class fenetre : public QMainWindow, private Ui::fenetre
{
    Q_OBJECT

public:
    fenetre();
    void affich();
    void addMessage(QString Rpseudo, QString type, QString msg);


public slots:
    void closeEvent(QCloseEvent *event);
    void bConnexionPresse();
    void keyPressEvent(QKeyEvent *event);
    void jouerSon(QString son);
    void addErreur(QString type, QString erreur);


private slots:
    void ActuPlayer();
    void NewPartie(QVector<int> cartes, QList<QString> autresJoueurs);
    void finPartie();
    QString GetCarte(int carte);
    void envoyerCartes(bool cartesValidees=false);
    void bPasserPresse();


    void donneesRecues();
    void Analyse(QString Rpseudo, QString type, QString message, QString action);
    void Analyse(QString Rpseudo, QString type, QVector<int> cartes, QList<QString> autresJoueurs, QList<int> autresJoueurs2, QString action);

    void connecte();
    void deconnecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void envoyerMessage(QString type, QString msg);


    void actSeConnecter();
    void actInformation();
    void actParametre();
    void actuSelection01();
    void actuSelection02();
    void actuSelection03();
    void actuSelection04();
    void actuSelection05();
    void actuSelection06();
    void actuSelection07();
    void actuSelection08();
    void actuSelection09();
    void actuSelection10();
    void actuSelection11();
    void actuSelection12();
    void actuSelection13();
    void actuSelection(int carte, bool coche);
    void changeEtatCarte(int carte, bool checked);
    bool getEtatCarte(int carte);

    void EnvoyerChat();

    void sonAlerteJouer();


private:
    QString version;
    int nbG;
    int nbH;
    int nbD;
    QVector<int> milieu;
    QVector<int> milieuL;
    QVector<int> player;


    QTcpSocket *socket; // Représente le serveur
    quint32 tailleMessage;

    connexion *fenC;
    information *fenInfo;
    parametre *fenParam;
    QString ip;
    QString mdpServ;
    int port;
    QString pseudo;
    QString mdp;
    bool passer;

    QString joueurQuiJoue;

    QTimer *sonAlerteJouerTimer;

    QSoundEffect sonStart;
    QSoundEffect sonWin;
    QSoundEffect sonCarte;
    QSoundEffect sonMsg;
    QSoundEffect sonAlerte;




};

#endif // FENETRE_H
