#ifndef FENSERVEUR_H
#define FENSERVEUR_H


#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <QMap>
#include <QTime>
#include <QMessageBox>

#include "Client.h"


class FenServeur : public QWidget
{
    Q_OBJECT

    public:
        FenServeur();
        void envoyerATous(QString *pseudo, const QString &type, const QString &message, const QString &action);
        void envoyerAPartie(QString *pseudo, const QString &type, const QString &message, const QString &action);
        void envoyerUniquementA(Client *client, const QString &type, const QString &message, const QString &action);
        QByteArray preparerPaquetMsg(QString *name, const QString &type, const QString &message, const QString &action);
        QByteArray preparerPaquetCartes(QString *name, const QString &type, QVector<int> *cartes, const QString &action);
        void envoyerCartes(QString *client, const QString &type, QVector<int> *cartes, const QString &action);
        void nouvelleCarteMilieu(QVector<int> *carte);
        void nextJoueur();
        void GagnantF(int joueurQuiGG);
        void joueurFini(Client *client, bool victoire);
        bool qqPeutJouer(bool joueurQuiJoueAussi=true);
        void newPlateau();
        void clearPlateau();
        void envoyerCarteOuRien();

        void newClient(QTcpSocket *ts, QString *pseudo, QString mdp);
        void passerAuJoueur(int joueur, bool annoncer=true);
        QString *chercheClientsT(QString *name);
        void addLog(QString texte);

        bool serveurIsListenning();



    private slots:
        void updateMdp();
        void nouvelleConnexion();

        void donneesRecues();
        void analyseMsg(Client *client, QString type, QString msg);
        void AnalyseCarte(Client *client, QVector<int> *carte);
        void deconnexionClient();
        void afficherInfo();

        void actuJoueurs();

        void AjouterP(Client *client);
        void KickP(Client *client);
        void LancerPartie();
        void PasserJoueur();

        void resetPartie();
        void bArreterPartiePrece();
        void updateSpinJoueurQuiCommenceProchainePartie();

signals:
        void sPartieTerminee();
        void sPartieLancee();

    private:
        QLabel *etatServeur;
        QLabel *lblMdp;
        QLineEdit *mdpDuServ;

        QTcpServer *serveur;
        quint32 tailleMessage;
        QList<QTcpSocket *> clients;
        QMap<QString*, Client*> clientsT;
        QVBoxLayout *layoutClient;

        QPushButton *bLancerPartie;
        QPushButton *bArreterPartie;
        QLabel *etatPartie;
        QPushButton *bPasserJoueur;
        QTextEdit *tLog;

        bool partieLancee;
        QList<Client*> joueursPartie;
        QVector<int> *milieu;
        QVector<int> *milieuLast;
        int joueurQuiJoue;
        int joueurQuiCommence;
        QSpinBox *joueurQuiCommenceProchainePartie;
        int dernierJoueurQuiJoue;
        int cartesALaSuite;
        bool carteUltimeUtilisee;

        QString *strServeur1;
        QString *strServeur2;
};

#endif // FENSERVEUR_H
