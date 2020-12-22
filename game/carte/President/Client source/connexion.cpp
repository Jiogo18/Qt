#include "connexion.h"

connexion::connexion()
{
    setupUi(this);


    connect(bConnexion, SIGNAL(clicked()), this, SLOT(connection()));
    connect(bFermer, SIGNAL(clicked()), this, SLOT(fermer()));

    Dserv="127.0.0.1";
    DmdpServ="";
    Dport=25567;
    Dpseudo="";
    Dmdp="";
}



QString connexion::getServeur()
{
    return adresse->text();
}

QString connexion::getMdpServ()
{
    return mdpServ->text();
}

int connexion::getPort()
{
    return port->value();
}

void connexion::setEtat(QString rEtat, int rTypeEtat)
{
    etat->setPlainText(rEtat);
    if(rTypeEtat)
    {
        bConnexion->setEnabled(true);
        if(rTypeEtat==1)
        {
            Dserv=adresse->text();
            DmdpServ=mdpServ->text();
            Dport=port->value();
            Dpseudo=pseudo->text();
            Dmdp=mdp->text();
        }
    }
    else
    {
        bConnexion->setDisabled(true);
    }
}



QString connexion::getPseudo()
{
    return pseudo->text();
}

QString connexion::getMdp()
{
    return mdp->text();
}



void connexion::affich()
{
    adresse->setText(Dserv);
    mdpServ->setText(DmdpServ);
    port->setValue(Dport);
    pseudo->setText(Dpseudo);
    mdp->setText(Dmdp);
    show();
}

bool connexion::pseudoValide(QString pseudo)
{
    if(QStringList({"Undefined", ""}).contains(pseudo))
    {
        etat->setPlainText("Pseudo invalide");
        return false;
    }
    if(pseudo.toLower().contains("serveur"))//n'importe quel "serveur", ça peut etre 11fdsf_ServEUr_51d
    {
        etat->setPlainText("Le pseudo ne doit pas contenir le mot \"serveur\"");
        return false;
    }
    for(int i=0; i<pseudo.size(); i++)
    {
        if(!QString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_").contains(pseudo[i]))
        {
            etat->setPlainText(QString("Charactère ") + pseudo[i] + " invalide");
            return false;
        }
    }
    return true;
}





void connexion::connection()
{
    if(pseudo->text() != "")
    {
        if(pseudoValide(pseudo->text()))
        {
            if(mdp->text() != "")
            {
                emit SBConnexion();
            }
            else
                etat->setPlainText("Veuillez mettre un mot de passe");
        }
    }
    else
        etat->setPlainText("Veuillez mettre un pseudo");
}

void connexion::fermer()
{
    close();
}
