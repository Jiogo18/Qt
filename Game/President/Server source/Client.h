#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <QVector>
#include "cartes.h"

class Client : public QWidget
{
    Q_OBJECT

    public:
        Client(QTcpSocket *ts, QString *name, QString mdp, QWidget *parent);
        void actuWidget();

        void setCartes(QVector<int> *cartes);
        void setConnecte(bool connecte);
        void setDansLaPartie(bool dansLaPartie);
        void setForcePass(bool forcePass);
        void setNiveau(int niveau);
        void setPass(bool pass);
        void setTs(QTcpSocket *ts);

        void ajouterCartes(QVector<int> *cartes);
        void ajouterCartes(int carte);
        bool cartesVides();
        void clearCartes();
        bool containsCartes(QVector<int> *cartes);
        void removeCartes(QVector<int> *cartes);
        void removeCartes(int i);
        int nbDeCartes();

        QVector<int> *GetCartes();
        QString *GetMDP();
        int GetNiveau();
        QString *GetNom();
        QTcpSocket *GetTs();

        bool isConnected();
        bool isDansLaPartie();
        bool isForcePass();
        bool isIA();
        bool isPass();


    private slots:
        void bPartie();
        void bKicker();
    public slots:
        void partieLancee();
        void partieTerminee();

    signals:
        void ajoutGame(Client *client);
        void kicker(Client *client);


    private:
        QTcpSocket *m_ts;
        QString *m_name;
        QString *m_mdp;
        bool m_connecte;
        bool m_pass;
        bool m_forcePass;

        QLabel *lblCo;
        QLabel *lblNom;
        QCheckBox *m_dansLaPartie;
        QSpinBox *m_niveau;
        QPushButton *bKick;

        QVector<int> *m_cartes;

};

#endif // CLIENT_H
