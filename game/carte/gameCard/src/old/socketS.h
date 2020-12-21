#ifndef SOCKETS_H
#define SOCKETS_H
//socket serveur

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStandardItemModel>
#include "src/old/clientOld.h"
#include <QDateTime>

class socketS : public QWidget
{
    Q_OBJECT
public:
    socketS(const QString *version, QWidget *parent=nullptr);
    bool createServer(quint16 port, QString mdp, QString *etat=nullptr);
    const QString *getEtat();//actuEtat
    void changeMdp(QString mdp);
    QAbstractItemModel *getTablePlayer();
    void actuTabPlayer();
    QString generateUUID();
    QStringList versionCompatible();
    void addLog(QString log, bool traitement);
    //void sendMsgAll(messageSocket msg);

private slots:
    void nouvelleConnexion();
    void donneesRecues();
    void deconnexionClient();
    void createClient(QTcpSocket *ts, QDataStream *in);
    QDataStream *createData(QByteArray *paquet);
    void sendData(QTcpSocket *ts, QByteArray *paquet, QDataStream *out);
    void accepteConnection(QTcpSocket *ts, bool accept, QString description, clientOld *c);
    //void msgRecus(client *c, messageSocket msg);

signals:
    void newEtat(QString);
    void newLog(QString, bool);
    //void newCmd(messageSocket, QString);
    //void newChat(messageSocket, QString);


private:
    QTcpServer *serveurP;
    const QString *versionP;
    QString mdpP;
    QString mdpPHash;
    QString *etatP;
    bool connecteP;
    QMap<QTcpSocket*, QString> clients;//QString : uuid
    QStandardItemModel *tabPlayer;
    QMap<QString, clientOld*> clientM;
    quint32 tailleMessage;
};

#endif // SOCKETS_H
