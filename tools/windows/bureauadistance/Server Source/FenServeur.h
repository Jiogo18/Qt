#ifndef FENSERVEUR_H
#define FENSERVEUR_H


#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include <qwindow.h>

#include <iostream>

class FenServeur : public QWidget
{
    Q_OBJECT

    public:
        FenServeur();
        QPixmap grabScreens();



public slots:
        void keyPressEvent(QKeyEvent *event);
        void closeEvent(QCloseEvent *event);


    private slots:
        void afficherInfo(bool estConnecte=true);
        void actuScreen();

        void nouvelleConnexion();

        void donneesRecues();
        void deconnexionClient();
        void envoyerA(const QString &message, const QPixmap &screen);



signals:

    private:
        QLabel *etatServeur;
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        quint32 tailleMessage;
        QList<QTcpSocket *> clients;

        bool stop;
        QTimer *timerScreen;

};

#endif // FENSERVEUR_H
