#include "Client.h"

Client::Client(QTcpSocket *ts, QString *name, QString mdp, QWidget *parent) : QWidget(parent)
{
    m_ts = ts;
    m_name = name;
    m_mdp = new QString(mdp);
    m_connecte = true;


    QHBoxLayout *layout = new QHBoxLayout;

    lblCo = new QLabel;
    lblCo->setWhatsThis("État de connexion du client");
    layout->addWidget(lblCo);

    lblNom = new QLabel;
    lblCo->setWhatsThis("Nom du client");
    layout->addWidget(lblNom);

    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

    m_dansLaPartie = new QCheckBox;
    m_dansLaPartie->setChecked(false);
    layout->addWidget(m_dansLaPartie);
    connect(m_dansLaPartie, SIGNAL(clicked(bool)), this, SLOT(bPartie()));

    m_niveau = new QSpinBox;
    m_niveau->setMaximum(4);
    m_niveau->setMinimum(0);
    m_niveau->setValue(0);
    layout->addWidget(m_niveau);

    bKick = new QPushButton("Kick");
    connect(bKick, SIGNAL(clicked(bool)), this, SLOT(bKicker()));
    layout->addWidget(bKick);

    setLayout(layout);
    actuWidget();

    m_pass=false;
    m_forcePass=false;
    m_cartes = new QVector<int>;
    for(int i=0; i<13; i++)
        m_cartes->push_back(0);
}

void Client::actuWidget()
{
    QPixmap pixCo(16,16);
    if(m_connecte)
    {
        pixCo.load(":/icones/icones/valide2.png");
    }
    else
    {
        pixCo.load(":/icones/icones/annuler.png");
    }
    pixCo = pixCo.scaled(16, 16);
    lblCo->setPixmap(pixCo);

    lblNom->setText(*m_name);
    if(isIA())
        hide();//pas besoin de le voir
}



void Client::setCartes(QVector<int> *cartes)
{
    m_cartes = cartes;
    m_cartes = cartes::Trieur(m_cartes);
}

void Client::setConnecte(bool connecte)
{
    m_connecte = connecte;
    actuWidget();
}

void Client::setDansLaPartie(bool dansLaPartie)
{
    m_dansLaPartie->setChecked(dansLaPartie);
}

void Client::setForcePass(bool forcePass)
{
    if(forcePass && !m_dansLaPartie->isChecked())
    {//les serveur1 et serveur2 ne sont pas dans la partie mais comme ils forcepass jamais, pas de pb
        qDebug(QString("Le client "+*m_name+" aurais voulus force pass mais il n'est pas dans la partie").toStdString().c_str());
        return;
    }
    m_forcePass = forcePass;
}

void Client::setNiveau(int niveau)
{
    m_niveau->setValue(niveau);
}

void Client::setPass(bool pass)
{
    m_pass=pass;
}

void Client::setTs(QTcpSocket *ts)
{
    m_ts = ts;
    actuWidget();
}



void Client::ajouterCartes(QVector<int> *cartes)
{//ajoute les cartes au paquet du client (echange entre les clients) (doit avoir des cases == 0 !)
    for(int i=0; i<m_cartes->size(); i++)
        if(m_cartes->at(i)==0)
        {
            m_cartes->replace(i, cartes->first());
            cartes->removeFirst();
        }
    m_cartes = cartes::Trieur(m_cartes);
}

void Client::ajouterCartes(int carte)
{
    for(int i=0; i<m_cartes->size(); i++)
        if(m_cartes->at(i)==0)
        {
            m_cartes->replace(i, carte);
            break;
        }
    m_cartes = cartes::Trieur(m_cartes);
}

bool Client::cartesVides()
{
    return cartes::cartesJoueurVide(m_cartes);
}

void Client::clearCartes()
{
    m_cartes->clear();
    for(int i=0; i<13; i++)
        m_cartes->push_back(0);
}

bool Client::containsCartes(QVector<int> *cartes)//test si le client a les cartes donnes (utile pour ensuite les lui enlever)
{
    QVector<int> cartes2;
    for(int i=0; i<cartes->size(); i++)
    {
        if(!m_cartes->contains(cartes->at(i)))
            return false;
        if(cartes2.contains(cartes->at(i)))//si on avait déja la carte (une carte ne réapparait jamais 2 fois)
            return false;
        cartes2.append(cartes->at(i));//sinon on l'ajoute au tableau
    }
    return true;
}

void Client::removeCartes(QVector<int> *cartes)
{
    for(int i=0; i<cartes->size(); i++)//retire les cartes au paquet du client
        m_cartes->replace(m_cartes->indexOf(cartes->at(i)), 0);//remplace par un 0
}

void Client::removeCartes(int i)
{
    m_cartes->replace(i, 0);//remplace par un 0
}

int Client::nbDeCartes()
{
    return m_cartes->size() - m_cartes->count(0);//tt les cartes - les 0 = les cartes non vides
    /*int carte=0;
    for(int i=0; i<m_cartes->size(); i++)
    {
        if(m_cartes->at(i) != 0)
            carte++;
    }
    return carte;*/
}



QVector<int> *Client::GetCartes()
{
    return m_cartes;
}

QString *Client::GetMDP()
{
    return m_mdp;
}

int Client::GetNiveau()
{
    return m_niveau->value();
}

QString *Client::GetNom()
{
    return m_name;
}

QTcpSocket* Client::GetTs()
{
    return m_ts;
}



bool Client::isConnected()
{
    return m_connecte;
}

bool Client::isDansLaPartie()
{
    return m_dansLaPartie->isChecked();
}

bool Client::isForcePass()
{
    return m_forcePass;
}

bool Client::isIA()
{
    return *m_name == "Serveur1" || *m_name == "Serveur2";
}

bool Client::isPass()
{
    return m_pass || cartesVides() || isIA();
    //pass si : il pass, il a plus de cartes, c une ia
}

void Client::bPartie()
{
    emit ajoutGame(this);
}



void Client::bKicker()
{
    emit kicker(this);
}

void Client::partieLancee()
{
    m_dansLaPartie->setDisabled(true);
    m_niveau->setDisabled(true);
    m_pass=false;
    m_forcePass=false;
}
void Client::partieTerminee()
{
    m_dansLaPartie->setEnabled(true);
    m_niveau->setEnabled(true);
    m_pass=false;
    m_forcePass=false;
}
