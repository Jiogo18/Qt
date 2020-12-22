#ifndef FENETRE_H
#define FENETRE_H


#include <QtWidgets>
#include <QtNetwork>
#include <QString>
#include "screen.h"

#include <iostream>



class fenetre : public QWidget
{
    Q_OBJECT
    public:
        fenetre();
        void setEtat(QString msg);

    public slots:
        void closeEvent(QCloseEvent *event);
        void click(QMouseEvent *event);
        void clavier(QKeyEvent *event);




    private slots:
        void bLancerPresse();
        void bArreterPresse();

        void donneesRecues();

        void connecte();
        void deconnecte();
        void erreurSocket(QAbstractSocket::SocketError erreur);
        void envoyerMessage(QString msg, int x=0, int y=0);
        void mouvement();
        void resetFps();



    private:
        QTcpSocket *socket;
        QString adresse;
        int port;
        QLineEdit *Tadresse;
        QSpinBox *Sport;
        QPlainTextEdit *etat;
        QLabel *lblFPS;

        QPushButton *bLancer;
        QPushButton *bArreter;

        quint32 tailleMessage;

        Screen *fenScreen;
        QTimer *timerFps;
        int Fps;
        QTimer *timerCoo;

};

#endif // FENETRE_H
