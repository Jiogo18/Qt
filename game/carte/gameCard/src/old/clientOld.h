#ifndef CLIENTOLD_H
#define CLIENTOLD_H

#include <QString>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDebugStateSaver>
#include <QDataStream>
#include <QTcpSocket>

class clientOld : public QObject
{
    Q_OBJECT
public:
    clientOld(QString pseudo="", QString mdp="", bool mdpHashed=false, bool bot=false, bool connected=false);
    clientOld(const clientOld &c);
    bool setPseudo(QString pseudo);
    bool setMdp(QString mdp);
    void setMdpHash(QString mdpHash);
    void setBot(bool bot);
    void setConnected(bool connected);
    static QString hash(QString str);
    QString getPseudo() const;
    QString getMdpHash() const;//pas de getMdp car pas gardé
    bool isBot() const;
    static bool isPseudoValid(QString pseudo, QString *reponse=nullptr);
    static bool isMdpValid(QString mdp, QString *reponse=nullptr);
    bool isConnected() const;

    void operator =(const clientOld &c);
    bool operator ==(const clientOld &c) const;
    bool operator !=(const clientOld &c) const;
    static QString charPseudo();
    static quint32 randomNb(quint32 min=0, quint32 max=9999);

    void setSocket(QTcpSocket *socket);
    QTcpSocket *getSocket();

    void setVersion(QString version);
    QString getVersion();
    QString getPartie();

private:
    QString pseudoP;
    QString mdpHashP;
    bool botP;
    bool connecteP;
    QTcpSocket *socketP;
    quint32 tailleMessage;
    QString versionP;
    QString partie;
};

QDataStream &operator<<(QDataStream &out, const clientOld &c);
QDataStream &operator>>(QDataStream &in, clientOld &c);
QDebug operator<<(QDebug debug, const clientOld &c);

#endif // CLIENTOLD_H
