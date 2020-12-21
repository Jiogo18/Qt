//version du 12/06/2019 18:09 (7)
#include "clientOld.h"

clientOld::clientOld(QString pseudo, QString mdp, bool mdpHashed, bool bot, bool connected) : QObject()
{
    if(!setPseudo(pseudo))
        pseudoP="Joueur-" + QString::number(randomNb(0, 999));
    if(mdpHashed)
    {
        if(!setMdp(mdp))
            setMdp(QString::number(randomNb(100000, 999999)));
    }
    else
        setMdpHash(mdp);
    botP=bot;
    connecteP=connected;
    tailleMessage=0;
}
clientOld::clientOld(const clientOld &c) : QObject()
{
    pseudoP=c.getPseudo();
    mdpHashP=c.getMdpHash();
    botP=c.isBot();
    connecteP=c.isConnected();
    tailleMessage=0;
}

bool clientOld::setPseudo(QString pseudo)
{
    if(isPseudoValid(pseudo))
    {
        pseudoP = pseudo;
        return true;
    }
    return false;
}
bool clientOld::setMdp(QString mdp)
{
    if(isMdpValid(mdp))
    {
        mdpHashP=hash(mdp);
        return true;
    }
    return false;
}
void clientOld::setMdpHash(QString mdpHash) {mdpHashP=mdpHash;}
void clientOld::setBot(bool bot) {botP=bot;}
void clientOld::setConnected(bool connected)
{
    connecteP=connected;
    //signaux? / message
}

QString clientOld::hash(QString str)
{
    QCryptographicHash hasheur(QCryptographicHash::Sha256);
    hasheur.addData(str.toLocal8Bit());
    return QString(hasheur.result().data());
}

QString clientOld::getPseudo() const {return pseudoP;}
QString clientOld::getMdpHash() const {return mdpHashP;}
bool clientOld::isBot() const {return botP;}
bool clientOld::isPseudoValid(QString pseudo, QString *reponse)
{
    if(reponse==nullptr)
        reponse = new QString;
    *reponse = "Pseudo valide";
    if(pseudo.isEmpty() || pseudo.size() == pseudo.count(" "))
    {
        *reponse="Pseudo vide";
        return false;
    }
    for(int i=0; i<pseudo.length(); i++)
    {
        if(!charPseudo().contains(pseudo.at(i)))
        {
            *reponse = "Le pseudo ne peut contenir que les charactères alphanumériques, '_' et '-'";
            return false;
        }
    }
    if(pseudo.size() < 2) *reponse = "Pseudo faible";
    return true;
}
bool clientOld::isMdpValid(QString mdp, QString *reponse)
{
    if(reponse==nullptr)
        reponse = new QString;
    *reponse="Mot de passe valide";
    if(mdp.isEmpty() || mdp.size() == mdp.count(" "))
    {
        *reponse="Mot de passe vide";
        return false;
    }
    if(mdp.size() < 4) *reponse = "Mot de passe faible";
    return true;
}
bool clientOld::isConnected() const {return connecteP;}

void clientOld::operator =(const clientOld &c)
{pseudoP=c.getPseudo(); mdpHashP=c.getMdpHash();}
bool clientOld::operator ==(const clientOld &c) const//bot est pas important
{return pseudoP==c.getPseudo() && mdpHashP==c.getMdpHash();}
bool clientOld::operator !=(const clientOld &c) const {return !operator ==(c);}

QString clientOld::charPseudo()
{return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";}

quint32 clientOld::randomNb(quint32 min, quint32 max)
{
    QRandomGenerator random=QRandomGenerator::securelySeeded();
    return random.bounded(min, max);
}


void clientOld::setSocket(QTcpSocket *socket) {socketP = socket;}
//demander isConnected avant de demander le socket
QTcpSocket *clientOld::getSocket() {return socketP;}

void clientOld::setVersion(QString version) {versionP = version;}
QString clientOld::getVersion() {return versionP;}

QString clientOld::getPartie() {return partie;}

QDataStream &operator<<(QDataStream &out, const clientOld &c)
{
    out << c.getPseudo() << c.getMdpHash() << c.isBot() << c.isConnected();
    return out;
}
QDataStream &operator>>(QDataStream &in, clientOld &c)
{
    QString pseudo;
    QString mdp;
    bool bot;
    bool connected;
    in >> pseudo >> mdp >> bot >> connected;
    c = clientOld(pseudo, mdp, true, bot, connected);
    return in;
}
QDebug operator<<(QDebug debug, const clientOld &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(Pseudo:' << c.getPseudo() << ", mdp (hash):" << c.getMdpHash() << ", Bot:" << c.isBot() << ", connected" << c.isConnected() <<')';
    return debug;
}
