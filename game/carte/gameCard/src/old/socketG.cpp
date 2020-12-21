#include "socketG.h"

socketG::socketG(const QString *version, QWidget *parent) : QWidget()
{
    versionP = version;
    socket = new QTcpSocket(parent);
    tailleMessage=0;
    reponse = new QString;
    erreurParam = new QString;

    //creation
        ip = new QLineEdit("127.0.0.1");
        mdpServ = new QLineEdit();
        port = new QSpinBox;
        port->setMaximum(65535);//2^16
        port->setValue(25567);
        pseudo = new QLineEdit("Joueur-"+QString::number(clientOld::randomNb()));
        mdp = new QLineEdit(QString::number(clientOld::randomNb(100000, 999999)));
        clientP = new clientOld(pseudo->text(), mdp->text());
        clientP->setVersion(*versionP);
        etatT = new QTextEdit(*reponse);
        etatT->setReadOnly(true);
        bConnexion = new QPushButton("Connexion");
        QVBoxLayout *layout = new QVBoxLayout;
        QFormLayout *layoutParam = new QFormLayout;
        layoutParam->addRow(new QLabel("IP"), ip);
        layoutParam->addRow(new QLabel("Mot de passe serveur"), mdpServ);
        layoutParam->addRow(new QLabel("Port"), port);
        layoutParam->addRow(new QLabel("Pseudo"), pseudo);
        layoutParam->addRow(new QLabel("Mot de passe"), mdp);
        layout->addLayout(layoutParam);
        layout->addWidget(new QLabel("Etat :"));
        layout->addWidget(etatT);
        layout->addWidget(bConnexion);
        connect(bConnexion, &QPushButton::clicked, this, &socketG::connexion);
        QWidget::setLayout(layout);
        QWidget::resize(264, 323);

        //connect(ip, &QLineEdit::textEdited, this, &socketG::actuParam);
        //connect(port, SIGNAL(valueChanged(int)), this, SLOT(actuParam()));
        //connect(pseudo, &QLineEdit::textEdited, this, &socketG::actuParam);
        //connect(mdp, &QLineEdit::textEdited, this, &socketG::actuParam);

    setEtat("Déconnecté");
    //actuParam();
}

const QString *socketG::connexion()
{
    //actuParam();
    clientP->setPseudo(pseudo->text());
    clientP->setMdp(mdp->text());
    clientP->setConnected(false);
    if(!bConnexion->isEnabled())
    {
        setEtat(*erreurParam);
        return reponse;
    }
    bConnexion->setDisabled(true);
    setEtat("Connexion...");
    socket->abort(); //On désactive les connexions précédentes s'il y en a
    socket->connectToHost(ip->text(), port->value()); //On se connecte au serveur demandé
    return reponse;
}
const QString *socketG::deconnecte()
{
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        socket->abort();
        setEtat("Déconnection...");
        return reponse;//securité
    }
    clientP->setConnected(false);
    setEtat("Déconnecté");
    bConnexion->setEnabled(true);
    //emit disconnected();

    return reponse;
}



bool socketG::sendConnexion()
{
    QByteArray *paquet = new QByteArray;
    /*QDataStream *out=createData(paquet);
    if(out==nullptr) return false;
    *out << QString("connexion");
    *out << *versionP;
    if(mdpServ->text().isEmpty())
    {
        *out << mdpServ->text();
    }
    else
        *out << client::hash(mdpServ->text());//on le hash pour etre sur
    *out << *clientP;
    sendData(paquet, out);*/
    return true;
}


void socketG::setEtat(QString etat)
{
    *reponse = etat;
    etatT->setText(etat);
}

const clientOld *socketG::getMe() {return clientP;}



void socketG::connecte2(QDataStream &in)//2eme etape de la connexion
{
    QString accepte;
    in >> accepte;
    QString versionS;
    in >> versionS;
    QString description;
    in >> description;
    bool withClient;
    in >> withClient;
    clientOld *c;
    if(withClient)
    {//sinon c'est un nullptr
        c = new clientOld;
        in >> *c;
    }

    if(accepte == "accepte")
    {
        if(c != nullptr)
            clientP = c;
        setEtat("La connexion est établie\nVous avez le pseudo "+clientP->getPseudo()+"\n"+
                "et le mot de passe "+mdp->text()+"\n"+
                "\n"+
                "Le serveur a la version "+versionS);
        //emit connected();
    }
    else if(accepte=="refuse")
    {
        deconnecte();
        QString pseudoR="";
        if(c != nullptr)
        {
            pseudoR=c->getPseudo();
        }
        setEtat("Le serveur a refusé la connexion, raison : \n"+
                description+"\n"+
                "Le serveur a la version "+versionS+"\n"+
                "Pseudo : "+pseudoR);
    }
    else
    {
        deconnecte();
        setEtat("Le serveur a répondu à la connexion avec : "+accepte+"\n"+
                "Il a donné la raison suivante : "+description+"\n"+
                "Version du serveur : "+versionS);
    }
}


