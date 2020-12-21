#include "socketS.h"

socketS::socketS(const QString *version, QWidget *parent) : QWidget()
{
    versionP = version;
    serveurP = new QTcpServer(parent);
    etatP = new QString("Deconnecté");
    connecteP=false;
    tabPlayer = new QStandardItemModel(0, 3);
    tabPlayer->setHorizontalHeaderLabels({"ID", "Pseudo", "Partie"});
    tailleMessage=0;
    actuTabPlayer();
}

const QString *socketS::getEtat()
{
    actuTabPlayer();
    if(connecteP)
    {
        *etatP="Le serveur a démarré sur le port <strong>"+QString::number(serveurP->serverPort())+"</strong>.<br />"+
               "Il y a "+QString::number(clients.size())+" clients connectés.<br />";
        if(mdpP.isEmpty())
        {
            *etatP += "Il n'y a pas de mot de passe";
        }
        else
            *etatP += "Le mot de passe actuel est \""+mdpP+"\"";
    }
    else
    {
        *etatP="Deconnecté";
    }
    emit newEtat(*etatP);
    return etatP;
}

void socketS::changeMdp(QString mdp)
{
    mdpP=mdp;
    mdp.isEmpty() ? mdpPHash=mdpP : mdpPHash=clientOld::hash(mdpP);
    getEtat();
}

QAbstractItemModel *socketS::getTablePlayer() {return tabPlayer;}

void socketS::actuTabPlayer()
{
    tabPlayer->setRowCount(clientM.size());

    QMapIterator<QString, clientOld*> i(clientM);
    int i2=0;
    while(i.hasNext())
    {
        i.next();
        QString connected;
        i.value()->isConnected() ? connected="✔ " : connected="❌ ";
        tabPlayer->setItem(i2, 0, new QStandardItem(connected+i.key()));
        tabPlayer->setItem(i2, 1, new QStandardItem(i.value()->getPseudo()));
        tabPlayer->setItem(i2, 2, new QStandardItem(i.value()->getPartie()));
        i2++;
    }
}

QString socketS::generateUUID()
{
    QString part1=QString::number(QDateTime::currentMSecsSinceEpoch());
    QString part2=QString::number(clientOld::randomNb(0, 999));// 1/1000 d'avoir le meme (la meme msec)
    while(part2.size() < 4)
        part2 = "0"+part2;
    part1 += part2;
    if(clientM.contains(part1) || !clients.keys(part1).isEmpty())
    {
        qDebug()<<"socketG/generateUUID un UUID double !"<<part1;
        return generateUUID();//retente ta chance (la msec changera forcement)
    }

    return part1;
}

QStringList socketS::versionCompatible()
{return QStringList({"2.0"});}

void socketS::addLog(QString log, bool traitement)
{
    if(traitement)
        log = "["+QTime::currentTime().toString()+" Serveur] " + log;
    emit newLog(log, false);//on envoie au parent
}

/*void socketS::sendMsgAll(messageSocket msg)
{
    QByteArray *paquet = new QByteArray;
    QDataStream *out=createData(paquet);
    if(out==nullptr) return;//socket n'est pas coo
    *out << QString("message");
    *out << msg;

    out->device()->seek(0);
    //taille du packet sans le quint32
    *out << (quint32)(paquet->size() - sizeof(quint32));
    QMapIterator<QString, clientOld*> i(clientM);
    while(i.hasNext())
    {
        i.next();
        if(i.value()->isConnected())
            i.value()->getSocket()->write(*paquet);//envoie aux connectés
    }
}*/

void socketS::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveurP->nextPendingConnection();
    if(!clients.contains(nouveauClient)) //pas besoin de le mettre plusieurs fois
    {
        clients.insert(nouveauClient, generateUUID());//son uuid permet de l'identifier (find ensuite)
        connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
    }
    getEtat();
}

void socketS::donneesRecues()
{
    QTcpSocket *ts = qobject_cast<QTcpSocket *>(sender());
    if (ts == 0)//on n'a pas trouvé le client à l'origine
        return;

    QDataStream *in = new QDataStream(ts);
    while(true)
    {
        if (tailleMessage == 0)//on connaît pas encore la taille du message
        {
            if (ts->bytesAvailable() < (int)sizeof(quint32))//le quint32 est meme pas arrivé
                break;

            *in >> tailleMessage; //on a recu le quint32
        }

        if (ts->bytesAvailable() < tailleMessage) //on a pas tout reçu
            break;
        tailleMessage = 0;

        //on recupère les info
        QString type;
        *in >> type;
        if(type=="connexion")
        {
            createClient(ts, in);
        }
        else
        {
            clientOld *c = clientM.value(clients.value(ts));
            if(c == nullptr)
            {
                accepteConnection(ts, false, "not_registred", nullptr);
                in->abortTransaction();
                //si il y a des info après, c'est pas grave, le client est en tort
            }
            if(type=="message")
            {
                /*messageSocket msg("#recu");
                *in >> msg;
                msgRecus(c, msg);*/
            }
            else
                qDebug()<<"socketS/donneesRecus type inconnu:"<<type;
        }
    }
}

void socketS::deconnexionClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)//on n'a pas trouvé le client à l'origine
        return;
    //le client qui appartient à l'uuid de ce socket
    clientOld *clientDeco = clientM.value(clients.value(socket));

    if(clientDeco != nullptr)//il existe pas
    {
        clientDeco->setConnected(false);
        clientDeco->setSocket(nullptr);
    }

    clients.remove(socket);
    socket->deleteLater();

    getEtat();
}

void socketS::createClient(QTcpSocket *ts, QDataStream *in)
{
    QString versionC;
    *in >> versionC;
    if(!versionCompatible().contains(versionC))
    {
        addLog("Un client a tenté de ce connecter avec la version "+versionC, true);
        in->abortTransaction();// ça marche?
        //refuse
        accepteConnection(ts, false, "version", nullptr);
        return;
    }
    QString mdpC;
    *in >> mdpC;
    clientOld *c = new clientOld;
    *in >> *c;//? ou client c2; in >> c2; client *c = new client(c2);

    if(mdpC != mdpPHash)
    {
        accepteConnection(ts, false, "password_server", c);
        return;
    }

    //le client existe déja?
    bool contains=false;
    QMapIterator<QString, clientOld*> i(clientM);
    while(i.hasNext())
    {
        i.next();
        if(i.value()->getPseudo() == c->getPseudo())
        {
            contains=true;
            break;
        }
    }

    if(!contains)
    {//créer le client
        if(!c->isPseudoValid(c->getPseudo()))
        {
            accepteConnection(ts, false, "pseudo", c);
            return;
        }
        clientM.insert(clients.value(ts), c);//uuid + client
        c->setConnected(true);
        c->setSocket(ts);
    }
    else
    {
        if(*c != *i.value())//pseudo est déja ==, on verifie le mdp
        {
            //refuse
            accepteConnection(ts, false, "password_client", c);
            return;
        }
        if(i.value()->isConnected())//déja connecté
        {
            //refuse
            accepteConnection(ts, false, "already_connected", c);
            return;
        }
        //set l'uuid et le socket du client
        clients.insert(ts, i.key());//remplace avec l'ancien uuid
        c=i.value();
        c->setSocket(ts);
        c->setConnected(true);
    }
    accepteConnection(ts, true, "accepte", c);
    actuTabPlayer();
}

QDataStream *socketS::createData(QByteArray *paquet)
{
    QDataStream *out = new QDataStream(paquet, QIODevice::WriteOnly);
    *out << (quint32)0;//taille du paquet
    return out;
}
void socketS::sendData(QTcpSocket *ts, QByteArray *paquet, QDataStream *out)
{
    out->device()->seek(0);
    //taille du packet sans le quint32
    *out << (quint32)(paquet->size() - sizeof(quint32));
    ts->write(*paquet); // On envoie le paquet
}

void socketS::accepteConnection(QTcpSocket *ts, bool accept, QString description, clientOld *c)
{
    QByteArray *paquet = new QByteArray;
    QDataStream *out=createData(paquet);
    *out << QString("connexion");
    accept ? *out << QString("accepte") : *out << QString("refuse");
    *out << *versionP;
    *out << description;
    if(c != nullptr)
    {
        *out << true;
        *out << *c;
    }
    else
        *out << false;//on a pas d'objet client après
    sendData(ts, paquet, out);//confirmation de connection
}

/*void socketS::msgRecus(client *c, messageSocket msg)
{
    if(msg.getType() == "chat")
    {
        if(msg.getMessage().startsWith("/"))
        {
            emit newCmd(msg, c->getPseudo());
        }
        else
        {
            msg.setCreator(c->getPseudo());//pour etre sur
            sendMsgAll(msg);
            emit newChat(msg, c->getPseudo());
        }
    }
    else
        qDebug()<<"socketG/msgRecus type inconnu:"<<msg.getType();
}*/

bool socketS::createServer(quint16 port, QString mdp, QString *etat)
{
    changeMdp(mdp);
    QMapIterator<QTcpSocket*, QString> i(clients);
    while(i.hasNext())
    {
        i.next();
        i.key()->close();
        clientOld *c = clientM.value(i.value());
        if(c!= nullptr)
        {
            c->setConnected(false);
            c->setSocket(nullptr);
        }
        i.key()->deleteLater();
    }
    clients.clear();//retire tout les anciens socket
    actuTabPlayer();

    serveurP->close();
    tailleMessage=0;
    connecteP = serveurP->listen(QHostAddress::Any, port);//essaye de démarrer
    if(connecteP)
    {
        getEtat();
        connect(serveurP, &QTcpServer::newConnection, this, &socketS::nouvelleConnexion);
    }
    else
        *etatP="Le serveur n'a pas pu être démarré. Raison :<br />" + serveurP->errorString();
    if(etat != nullptr)
        *etat=*etatP;

    emit newEtat(*etatP);
    return connecteP;
}
