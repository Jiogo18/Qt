#ifndef SOCKETG_H
#define SOCKETG_H
//gestion du socket

#include <QTcpSocket>
#include <QDataStream>
#include <QtWidgets>
#include "clientOld.h"

class socketG : public QWidget
{
    Q_OBJECT
public:
    socketG(const QString *version, QWidget *parent=nullptr);
    const QString *connexion();
    const QString *deconnecte();
    bool sendConnexion();

    void setEtat(QString etat);
    const clientOld *getMe();

private slots:
    void connecte2(QDataStream& in);

private:
    QTcpSocket *socket;
    QString *reponse;
    quint32 tailleMessage;

    QLineEdit *ip;
    QLineEdit *mdpServ;
    QSpinBox *port;
    QLineEdit *pseudo;
    QLineEdit *mdp;
    QTextEdit *etatT;
    QPushButton *bConnexion;

    QString *erreurParam;
    clientOld *clientP;

    const QString *versionP;
};

#endif // SOCKETG_H
