#include "FenServeur.h"

FenServeur::FenServeur()
{
    //#Version 1.0
    // Création et disposition des widgets de la fenêtre
    {
        QVBoxLayout *layout = new QVBoxLayout;

        etatServeur = new QLabel;
        layout->addWidget(etatServeur);

        lblMdp = new QLabel("Mot de passe du serveur");
        mdpDuServ = new QLineEdit;
        connect(mdpDuServ, SIGNAL(textChanged(QString)), this, SLOT(updateMdp()));
        QHBoxLayout *layoutMdp = new QHBoxLayout;
        layoutMdp->addWidget(lblMdp);
        layoutMdp->addWidget(mdpDuServ);
        layout->addLayout(layoutMdp);


        QGroupBox *layoutBClient = new QGroupBox("Clients");
        layoutClient = new QVBoxLayout;
        layoutBClient->setLayout(layoutClient);
        layout->addWidget(layoutBClient);

        layout->addSpacerItem(new QSpacerItem(1, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));


        QHBoxLayout *lPartieBouttons = new QHBoxLayout;
        bLancerPartie = new QPushButton("Lancer la partie");
        bLancerPartie->setDisabled(true);
        connect(bLancerPartie, SIGNAL(clicked(bool)), this, SLOT(LancerPartie()));
        bArreterPartie = new QPushButton("Arreter la partie");
        bArreterPartie->setDisabled(true);
        connect(bArreterPartie, SIGNAL(clicked(bool)), this, SLOT(bArreterPartiePrece()));
        lPartieBouttons->addWidget(bLancerPartie);
        lPartieBouttons->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
        lPartieBouttons->addWidget(bArreterPartie);

        QHBoxLayout *lPartieJoueurQuiCommence = new QHBoxLayout;
        QLabel *lblJCom = new QLabel("Le joueur N° : ");
        QLabel *lblJCom2 = new QLabel(" commencera.");
        joueurQuiCommence=0;
        joueurQuiCommenceProchainePartie = new QSpinBox;
        joueurQuiCommenceProchainePartie->setMaximum(1);
        joueurQuiCommenceProchainePartie->setMinimum(1);
        connect(joueurQuiCommenceProchainePartie, SIGNAL(valueChanged(int)), this, SLOT(updateSpinJoueurQuiCommenceProchainePartie()));
        lPartieJoueurQuiCommence->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
        lPartieJoueurQuiCommence->addWidget(lblJCom);
        lPartieJoueurQuiCommence->addWidget(joueurQuiCommenceProchainePartie);
        lPartieJoueurQuiCommence->addWidget(lblJCom2);

        bPasserJoueur = new QPushButton("Passer le joueur");
        bPasserJoueur->setDisabled(true);
        connect(bPasserJoueur, SIGNAL(clicked(bool)), this, SLOT(PasserJoueur()));

        tLog = new QTextEdit;
        tLog->setReadOnly(true);

        QVBoxLayout *lPartie = new QVBoxLayout;
        lPartie->addLayout(lPartieBouttons);
        lPartie->addLayout(lPartieJoueurQuiCommence);
        lPartie->addWidget(bPasserJoueur);
        lPartie->addWidget(tLog);

        layout->addLayout(lPartie);

        setLayout(layout);
        setWindowTitle(tr("Président - Serveur"));
    }


    // Gestion du serveur
    {
        serveur = new QTcpServer(this);
        if (!serveur->listen(QHostAddress::Any, 25567)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 25567
        {
            // Si le serveur n'a pas été démarré correctement
            etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());

        }
        else// Si le serveur a été démarré correctement
            connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));

        tailleMessage = 0;

    }
    partieLancee = false;


    connect(this, SIGNAL(sPartieTerminee()), this, SLOT(resetPartie()));
    milieu = new QVector<int>;
    milieuLast = new QVector<int>;

    clientsT.clear();
    QRandomGenerator generateur = QRandomGenerator::securelySeeded();//permet d'empecher la connection au "client" serveur1 et serveur2 (sécuritée)
    strServeur1 = new QString("Serveur1");
    clientsT.insert(strServeur1, new Client(nullptr, strServeur1, QString::number(generateur.bounded(1000000)), this));//font les joueurs qui ne jouent pas
    layoutClient->addWidget(clientsT.find(strServeur1).value());
    strServeur2 = new QString("Serveur2");
    clientsT.insert(strServeur2, new Client(nullptr, strServeur2, QString::number(generateur.bounded(1000000)), this));//font les joueurs qui ne jouent pas
    layoutClient->addWidget(clientsT.find(strServeur2).value());

    actuJoueurs();
    afficherInfo();
    resize(540, 300);//taille optimale de la fenetre
}


void FenServeur::afficherInfo()
{
    if(serveur->isListening())
    {
        etatServeur->setText("Le serveur a été démarré sur le port <strong>" + QString::number(serveur->serverPort()) + "</strong>.<br />"
                            +"Des clients peuvent maintenant se connecter.<br />"
                            +"Il y a " + QString::number(clients.size()) + " clients connectés");
    }
}



//gestion de la connexion
void FenServeur::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    if(!clients.contains(nouveauClient)) //pas besoin de le mettre plusieurs fois
    {
        clients << nouveauClient;//on cree pas encore le client, on attendra qu'il nous envoie ces informations
        connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
    }
    afficherInfo();
}

void FenServeur::donneesRecues()
{
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *ts = qobject_cast<QTcpSocket *>(sender());
    if (ts == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(ts);

        if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
        {
            if (ts->bytesAvailable() < (int)sizeof(quint32)) // On n'a pas reçu la taille du message en entier
                 return;

            in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
        }

        // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
        if (ts->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
            return;
        tailleMessage = 0;

        // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
        QString mdpServ;
        in >> mdpServ;
        QString rPseudo;
        in >> rPseudo;
        QString *pseudo = new QString(rPseudo);
        QString mdp;
        in >> mdp;
        QString version;
        in >> version;
        QString type;
        in >> type;
        QString message;
        in >> message;
        QVector<int> carte;
        if(version == QString("carte"))
            in >> carte;

        pseudo = chercheClientsT(pseudo);//change le pointeur de pseudo
        if(!clientsT.contains(pseudo))//new connexion du client
        {
            newClient(ts, pseudo, mdp);
        }

        //verification du client
        if(mdpServ != mdpDuServ->text())
        {
            ts->write(preparerPaquetMsg(pseudo, "Connexion Erreur", "Le mot de passe serveur \"" + mdpServ + "\" est invalide", "deconnexion"));
            addLog(*pseudo + " c'est connecté avec le mauvais mot de passe serveur");
            return;
        }

        if(mdp != *clientsT.find(pseudo).value()->GetMDP())
        {
            ts->write(preparerPaquetMsg(pseudo, "Connexion Erreur", "Le mot de passe client est invalide", "deconnexion"));
            addLog(*pseudo + " c'est connecté avec le mauvais mot de passe client");
            return;
        }

        clientsT.find(pseudo).value()->setTs(ts); //on set quand meme le socket pour etre sur
        clientsT.find(pseudo).value()->setConnecte(true);

        if(version != QString("message") || type == QString("Commande"))//sinon on a déja les msg avec l'envoie aux autres
            addLog(*pseudo + " a envoyé une information : ["+type+"] "+message);


        if(version == QString("message"))
        {
            if(type!=QString("Commande"))
                envoyerATous(clientsT.find(pseudo).value()->GetNom(), type, message, "");
            if(joueursPartie.contains(clientsT.find(pseudo).value()))
            {
                analyseMsg(clientsT.find(pseudo).value(), type, message);
            }
            else if(type!=QString("Chat") && type!=QString("Connexion"))
                envoyerUniquementA(clientsT.find(pseudo).value(), "Erreur", "Vous devez ètre dans une partie pour envoyer des informations", "");
        }
        else if(version == QString("carte"))
            AnalyseCarte(clientsT.find(pseudo).value(), new QVector<int>(carte));

    }

void FenServeur::analyseMsg(Client *client, QString type, QString msg)
{
    if(type == QString("Partie"))
    {
        if(msg == QString("Je passe !"))
        {
            if(client->GetNom() == joueursPartie[joueurQuiJoue]->GetNom())
            {
                client->setPass(true);
                milieuLast->clear();//si il y a une carte ou rien demandée, plus besoin
                nextJoueur();
            }
            else
            {
                envoyerUniquementA(client, "Erreur", "Vous ne pouvez pas passer maintenand.", "");
            }
        }
    }
    else if(type == QString("Commande"))
    {
        if(msg.startsWith("/pass"))
        {
            if(client->isForcePass())
            {
                envoyerUniquementA(client, "Commande", "Vous avez déja voté.", "");
            }
            else
            {
                if(clientsT.find(new QString(msg.mid(6, -1))).value()->isDansLaPartie())
                {
                    if(*joueursPartie[joueurQuiJoue]->GetNom() == msg.mid(6, -1))// "/pass "=5 chars
                    {
                        client->setForcePass(true);
                        double forcePass=0, nbPasIA=0;//on peut pas laisser en int, la virgule metrai 1 au lieu de 1.5
                        for(int i=0; i<4; i++)
                        {
                            if(joueursPartie[i]->isForcePass())
                                forcePass++;
                            if(!joueursPartie[i]->isIA())
                                nbPasIA++;
                        }

                        if(forcePass >= nbPasIA/2.0)//il faut que la moitié des clients valident
                        {
                            envoyerAPartie(new QString("Serveur"), "Commande", "La moitiée des joueurs on décidé de faire passer " + *joueursPartie[joueurQuiJoue]->GetNom() + ", joueur suivant.", "pass");
                            joueursPartie[joueurQuiJoue]->setPass(true);
                            milieuLast->clear();//si il y a une carte ou rien demandée, plus besoin
                            nextJoueur();
                        }
                        else
                            envoyerAPartie(client->GetNom(), "Commande", "Un vote de plus contre " + *joueursPartie[joueurQuiJoue]->GetNom() + "(" + QString::number(forcePass/nbPasIA*100) + "%), vous ne pouvez pas jouer ?", "");
                    }
                    else
                        envoyerUniquementA(client, "Commande", "Ce joueur ne joue pas.", "");
                }
                else
                    envoyerUniquementA(client, "Commande", "Ce joueur n'existe pas dans cette partie.", "");
            }
        }
    }
    //puis envoyer a tous l'action
    actuJoueurs();
}


void FenServeur::AnalyseCarte(Client *client, QVector<int> *carte)
{
    if(client->GetNom() == joueursPartie[joueurQuiJoue]->GetNom())
    {
        if(carte->size()<=4 && (milieu->isEmpty() || cartes::carteSansCouleur(joueursPartie[joueurQuiJoue]->GetCartes()->at(carte->at(0))) >= cartes::carteSansCouleur(milieu->at(0))))
        {//si c vailide, et que les cartes sont plus grandes ou egales au milieu        ->carte[0] est un "pointeur" vers la carte du paquet du joueur
            nouvelleCarteMilieu(carte);
        }
        else
        {
            envoyerUniquementA(client, "Erreur", "Cartes non valides, reassayez", "joue");
        }
    }
    else
    {
        envoyerUniquementA(client, "Erreur", "Vous avez envoyé des cartes alors que ce n'est pas votre tour", "");
    }

    //puis envoyer a tous l'action
    actuJoueurs();
}


void FenServeur::deconnexionClient()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    clients.removeOne(socket);
    socket->deleteLater();

    actuJoueurs();
    afficherInfo();
}

void FenServeur::envoyerATous(QString *pseudo, const QString &type, const QString &message, const QString &action)
{
    if(action.isEmpty())
    {
        addLog("message à partie : " + *pseudo + " [" + type + "] " + message);
    }
    else
        addLog("message à partie : " + *pseudo + " [" + type + "] " + message + " (" + action + ")");

    // Envoi du paquet préparé à tous les clients connectés au serveur
    if(!clientsT.isEmpty())//ici c pour tous
    {
        QMapIterator<QString*, Client*> i(clientsT); //on parcours tout les joueurs
        while (i.hasNext())
        {
            i.next();
            if(i.value()->isIA())
                continue;
            if(i.value()->isConnected())
            {
                i.value()->GetTs()->write(preparerPaquetMsg(pseudo, type, message, action));
            }
        }
    }
    else
        addLog(QString("erreur envoye a tous : ") + message);
}

void FenServeur::envoyerAPartie(QString *pseudo, const QString &type, const QString &message, const QString &action)
{
    if(action.isEmpty())
    {
        addLog("message à partie : " + *pseudo + " [" + type + "] " + message);
    }
    else
        addLog("message à partie : " + *pseudo + " [" + type + "] " + message + " (" + action + ")");

    // Envoi du paquet préparé à tous les clients connectés au serveur
    if(!joueursPartie.isEmpty())//ici c seulmement pour ceux de la partie
    {
        for(int i=0; i<joueursPartie.size(); i++)
        {
            if(message.contains("il a la versoin"))
                addLog(QString("\"il a la version\" à ") + *joueursPartie[i]->GetNom());
            if(joueursPartie[i]->isIA())
                continue;
            if(joueursPartie[i]->isConnected())
            {
                joueursPartie[i]->GetTs()->write(preparerPaquetMsg(pseudo, type, message, action));
            }
        }
    }
    else
        addLog(QString("erreur envoye à partie : ") + message);
}

void FenServeur::envoyerUniquementA(Client *client, const QString &type, const QString &message, const QString &action)
{
    if(action.isEmpty())
    {
        addLog("message à " + *client->GetNom() + " : [" + type + "] " + message);
    }
    else
        addLog("message à tous : [" + type + "] " + message + " (" + action + ")");
    client->GetTs()->write(preparerPaquetMsg(client->GetNom(), type, message, action));
}

QByteArray FenServeur::preparerPaquetMsg(QString *name, const QString &type, const QString &message, const QString &action)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint32) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << *name;
    const QString version = "message";
    out << version;
    out << type;
    out << message; // On ajoute le message à la suite
    out << action;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint32) (paquet.size() - sizeof(quint32)); // On écrase le 0 qu'on avait réservé par la longueur du message

    return paquet;
}

QByteArray FenServeur::preparerPaquetCartes(QString *name, const QString &type, QVector<int> *cartes, const QString &action)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint32) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << *name;
    out << QString("carte");
    out << type;
    out << *cartes;
    QVector<QString> JoueursOKT;//envoie le nom des autres joueurs
    QVector<int> JoueursOKT2;//envoie le nb de cartes des autres joueurs
    for(int i=0; i<4; i++)//doit metre aussi les cartes!!
    {
        JoueursOKT.append(*joueursPartie[i]->GetNom());
        JoueursOKT2.append(joueursPartie[i]->nbDeCartes());
    }

    out << JoueursOKT;
    out << JoueursOKT2;

    out << action;
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint32) (paquet.size() - sizeof(quint32)); // On écrase le 0 qu'on avait réservé par la longueur du message
    return paquet;
}

void FenServeur::envoyerCartes(QString *client, const QString &type, QVector<int> *cartes, const QString &action)
{
    QByteArray paquet=preparerPaquetCartes(client, type, cartes, action);
    if(*client == QString("Serveur") || type == QString("Milieu"))
    {//envoie a tous
        addLog("envoye de cartes à tous (" + type + ")");
        if(!joueursPartie.isEmpty())
        {
            for(int i=0; i<joueursPartie.size(); i++)
            {
                if(joueursPartie[i]->isIA())
                    continue;
                if(joueursPartie[i]->isConnected())
                {
                    joueursPartie[i]->GetTs()->write(paquet);
                }
            }
        }

    }//ça viens pas du serveur et c pas le milieu
    else if(client != strServeur1 && client != strServeur2)//pas besoin de leurs envoyer, ils n'existerons jamais
    {
        addLog("envoye de cartes à " + *client + " (" + type + ")");
        clientsT.find(client).value()->GetTs()->write(paquet);
    }
}


void FenServeur::nouvelleCarteMilieu(QVector<int> *carte)
{
    //test si les cartes sont pareils
    int nbCarte=cartes::carteSansCouleur(joueursPartie[joueurQuiJoue]->GetCartes()->at(carte->at(0)));
    for(int i=0; i<carte->size(); i++)
        if(cartes::carteSansCouleur(joueursPartie[joueurQuiJoue]->GetCartes()->at(carte->at(i))) != nbCarte)
        {//si les cartes donnees n'ont pas la meme valeur (doubles)
            envoyerUniquementA(joueursPartie[joueurQuiJoue], "Erreur", "Vos cartes ne sont pas bonnes.", "joue");
            return;
        }


    //si il demande une carte présice (ou rien)
    if(!milieuLast->isEmpty() && milieuLast->size()==1 && milieu->size()==1 && cartes::carteSansCouleur(milieu->at(0)) == cartes::carteSansCouleur(milieuLast->at(0)))
        if(nbCarte != cartes::carteSansCouleur(milieu->at(0)))
        {//si c pas la bonne carte

            envoyerCarteOuRien();//2eme avertissement, ne trichez pas !
            actuJoueurs();
            return;
        }


    //tout va biens, on échange les cartes
    addLog(*joueursPartie[joueurQuiJoue]->GetNom() + " nouvelles cartes au milieu");
    dernierJoueurQuiJoue = joueurQuiJoue;
    milieuLast = new QVector<int>(*milieu);
    milieu->clear();
    for(int i=0; i<carte->size(); i++)
    {
        milieu->append(joueursPartie[joueurQuiJoue]->GetCartes()->at(carte->at(i)));
        joueursPartie[joueurQuiJoue]->removeCartes(carte->at(i));
    }
    envoyerCartes(joueursPartie[joueurQuiJoue]->GetNom(), "Milieu", milieu, "");

    if(joueursPartie[joueurQuiJoue]->cartesVides())
    {//si le joueur a terminé
        GagnantF(joueurQuiJoue);
        if(!partieLancee)
            return;
    }


    if(cartes::carteSansCouleur(milieu->at(0)) == 15)
    {
        envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", "Deux (2), reset.", "reset");
        newPlateau();
        return;
    }

    if(!milieuLast->isEmpty() && milieu->size()==2 && milieuLast->size()==2 &&    cartes::carteSansCouleur(milieu->at(0)) == cartes::carteSansCouleur(milieuLast->at(0))  )
    {//10*2 et 10*2 = carré de doubles
        envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", "Double " + cartes::carteEnText(milieu->at(0)) + " a la suite, carré !", "reset");
        newPlateau();
        return;
    }


    if(milieuLast->isEmpty() || cartes::carteSansCouleur(milieuLast->at(0)) != cartes::carteSansCouleur(milieu->at(0)))
    {//un carré de 4 meme cartes a la suite
        cartesALaSuite=1;
    }
    else
    {
        cartesALaSuite++;
        if(cartesALaSuite==4)//au bout des 4 a la suite
        {
            envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", cartes::carteEnText(milieu->at(0)) + " 4 fois de suite, carré.", "reset");
            newPlateau();
            return;
        }
    }


    if(carteUltimeUtilisee==false)//le permier msg ce fait dans next joueur
    {
        nextJoueur();
    }
    else
    {
        envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", "Vous avez déja posé votre carte ultime, reset.", "reset");
        dernierJoueurQuiJoue=joueurQuiJoue;
        newPlateau();
        return;
    }


    if(milieuLast->size()==1 && milieu->size()==1 && cartes::carteSansCouleur(milieu->at(0))==cartes::carteSansCouleur(milieuLast->at(0)))
    {
        envoyerCarteOuRien();//premier demande
    }

}



void FenServeur::nextJoueur()
{
    passerAuJoueur((joueurQuiJoue+1)%4);
    if(!qqPeutJouer(false))//si le joueur est le seul à continuer, carte ultime
    {
        envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", "Vous etes seul, voulez vous poser une dernière carte ?", "carte ultime");
        carteUltimeUtilisee=true;
    }
}



void FenServeur::GagnantF(int joueurQuiGG)//gagne definitivement
{//joueurQuiGG = joueurQuiJoue
    if(!joueursPartie[joueurQuiGG]->cartesVides())//forcement mais verif
    {
        addLog("Erreur GagnantF, "+*joueursPartie[joueurQuiGG]->GetNom());
        return;
    }


    if(cartes::carteSansCouleur(milieu->at(0)) == 15)
    {
        joueurFini(joueursPartie[joueurQuiGG], false);//a perdue
        envoyerAPartie(joueursPartie[joueurQuiGG]->GetNom(), "Partie", "Vous avez terminé par un 2 ! Boooo. Vous avec le grade " + QString::number(joueursPartie[joueurQuiGG]->GetNiveau()) + ".", "gagnant");
    }
    else
    {
        joueurFini(joueursPartie[joueurQuiGG], true);//a gagné
        envoyerAPartie(joueursPartie[joueurQuiGG]->GetNom(), "Partie", "Felicitation, vous avez terminé votre jeu avec le grade " + QString::number(joueursPartie[joueurQuiGG]->GetNiveau()) + ".", "gagnant");
    }
    int mondeAFini=0;
    int joueurAPasFini;
    for(int i=0; i<4; i++)
    {
        if(joueursPartie[i]->GetNiveau() != 0)//si son score a été défini
        {
            mondeAFini++;
        }
        else
            joueurAPasFini=i;
    }
    if(mondeAFini+1 == 4)//si une seule personne n'a pas fini, il a perdue
    {
        joueurFini(joueursPartie[joueurAPasFini], false);//a perdue
        envoyerCartes(new QString("Serveur"), "Milieu", milieu, "");
        emit sPartieTerminee();//les ia ont de base gagné/perdue
        return;
    }



    bool qqAutreAFini=false;
    for(int i=0; i<4; i++)
    {
        if(i == joueurQuiJoue)
            continue;
        if(joueursPartie[i]->cartesVides() && !joueursPartie[i]->isIA())//si il a fini et que c pas une ia
            qqAutreAFini = true;
    }


    if(!qqAutreAFini)//on ne joue pas au dessus du president
        newPlateau();
}

void FenServeur::joueurFini(Client *client, bool victoire)
{
    if(victoire)//a pas terminé par un 2
    {//les gagnant on les valeurs les plus faible (1er...)
        int maximumDesGagnants=0;
        for(int i=0; i<4; i++)
            if(joueursPartie[i]->GetNiveau() == maximumDesGagnants+1)
            {
                maximumDesGagnants = joueursPartie[i]->GetNiveau();
                i=-1;//on reset pour qu'il recalcul tt
            }
        client->setNiveau(maximumDesGagnants+1);
    }
    else//il a terminé par un 2, boo
    {
        int minimumDesPerdants=5;//les perdants on les valeurs les plus élevées (4ème...)
        for(int i=0; i<4; i++)
            if(joueursPartie[i]->GetNiveau() == minimumDesPerdants-1)
            {
                minimumDesPerdants = joueursPartie[i]->GetNiveau();
                i=-1;
            }
        client->setNiveau(minimumDesPerdants-1);
    }
    addLog(*client->GetNom() + " a terminé avec le score " + QString::number(client->GetNiveau()));
}

bool FenServeur::qqPeutJouer(bool joueurQuiJoueAussi)
{
    for(int i=0; i<4; i++)
        if(!joueursPartie[i]->isPass()) //si il joue (packet non vide, passe pas)
            if(joueurQuiJoueAussi || i != joueurQuiJoue)
                return true;

    return false;//si arrive la, personne ne peut jouer
}

void FenServeur::newPlateau()
{
    clearPlateau();
    passerAuJoueur(dernierJoueurQuiJoue);
}

void FenServeur::clearPlateau()
{
    milieu->clear();
    milieuLast->clear();
    for(int i=0; i<4; i++)
    {
        joueursPartie[i]->setPass(false);
        joueursPartie[i]->setForcePass(false);
    }
    envoyerCartes(new QString("Serveur"), "Milieu", milieu, "");
    cartesALaSuite=0;
    carteUltimeUtilisee=false;
}



void FenServeur::envoyerCarteOuRien()
{
    envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", cartes::carteEnText(milieu->at(0)) + " ou rien.", "carte ou rien");
}





void FenServeur::newClient(QTcpSocket *ts, QString *pseudo, QString mdp)
{
    clientsT.insert(pseudo, new Client(ts, pseudo, mdp, this));
    connect(clientsT.find(pseudo).value(), SIGNAL(kicker(Client*)), this, SLOT(KickP(Client*)));
    connect(clientsT.find(pseudo).value(), SIGNAL(ajoutGame(Client*)), this, SLOT(AjouterP(Client*)));
    connect(this, SIGNAL(sPartieLancee()), clientsT.find(pseudo).value(), SLOT(partieLancee()));
    connect(this, SIGNAL(sPartieTerminee()), clientsT.find(pseudo).value(), SLOT(partieTerminee()));
    if(partieLancee)//desactiver auto l'entree car la partie est deja lancée
        clientsT.find(pseudo).value()->partieTerminee();
    layoutClient->addWidget(clientsT.find(pseudo).value());
    addLog("Un nouveau client viens d'apparaitre : "+*pseudo);
}


void FenServeur::passerAuJoueur(int joueur, bool annoncer)
{
    if(joueur < 0 || joueur > 3)
    {
        addLog(QString("essaye de passer au joueur n°") + QString::number(joueur));
        joueur = joueurQuiJoue;
    }
    if(joueursPartie[joueur]->isPass())
    {
        if(!qqPeutJouer(true))
        {
            envoyerAPartie(new QString("Serveur"), "Partie", "Tout le monde a passé, reset.", "reset");
            newPlateau();
            return;
        }
        passerAuJoueur((joueur+1)%4);
        return;
    }
    joueurQuiJoue = joueur;
    if(annoncer)
        envoyerAPartie(joueursPartie[joueurQuiJoue]->GetNom(), "Partie", "Le joueur " + QString::number(joueurQuiJoue+1) + " joue.", "joue");
    bPasserJoueur->setText("Passer "+*joueursPartie[joueurQuiJoue]->GetNom()+" (" + QString::number(joueurQuiJoue+1) + "/4)");
    for(int i=0; i<4; i++)
        joueursPartie[i]->setForcePass(false);
}

QString *FenServeur::chercheClientsT(QString *name)
{
    QMapIterator<QString*, Client*> i(clientsT); //on parcours tout les joueurs
    while (i.hasNext())
    {
        i.next();
        if(*name == *i.value()->GetNom())
        {
            name->clear();//sécuritée, permet de "suppr" le contenu parce que changement de case RAM
            return i.value()->GetNom();
        }
    }
    return name;
}

void FenServeur::addLog(QString texte)
{
    qDebug(QString("[" + QTime::currentTime().toString() + "] : " + texte).toStdString().c_str());
    tLog->append("[" + QTime::currentTime().toString() + "] : " + texte);
}

bool FenServeur::serveurIsListenning()
{
    if(serveur->isListening())
        return true;
    //sinon on demande
    int reponse = QMessageBox::question(this, "Erreur Serveur", "Le serveur n'a pas démaré correctement. Raison :\n" + serveur->errorString() + "\n\nVoulez vous quitter ?", QMessageBox::Yes | QMessageBox::No);
    return reponse != QMessageBox::Yes;//si on appuyer sur oui, retourne faux
}


void FenServeur::updateMdp()
{
    envoyerATous(new QString("Serveur"), "Connexion", "Le mot de passe du serveur viens d'etre changé.", "deconnexion");
}



//gestion des joueurs
void FenServeur::actuJoueurs()
{
    QMapIterator<QString*, Client*> i(clientsT); //on parcours tout les joueurs
    while (i.hasNext())
    {
        i.next();
        if(i.value()->isIA())//si c une ia, pas besoin de regarder
            continue;
        if(!clients.contains(i.value()->GetTs()) && i.value()->isConnected())
        {
            i.value()->setConnecte(false);
            envoyerATous(i.key(), "Connexion", "Viens de se déconnecter", "deconnexion");
        }//la connexion se fait quand le ts se créé dans ::nouvelleConnexion()
    }

    if(!partieLancee)
    {
        bLancerPartie->setEnabled( 2 <= joueursPartie.size() && joueursPartie.size() <= 4 );
    }
    if(!joueursPartie.isEmpty())
        joueurQuiCommenceProchainePartie->setMaximum(joueursPartie.size());

    joueurQuiCommenceProchainePartie->setValue(joueurQuiCommence+1);//pour la fenetre on verra aller de 1 à 4 pour plus de facilitée
    //l'inverse de ::updateSpinJoueurQuiCommenceProchainePartie
}



void FenServeur::AjouterP(Client *client)
{
    if(client->isDansLaPartie())//si il doit aller dans la partie
    {
        if(!joueursPartie.contains(client) && joueursPartie.size() < 4 && client->isConnected())//si il y est pas dedans (erreur signal) et si il y a de la place
        {
            joueursPartie.append(client);
            envoyerATous(client->GetNom(), "Partie", "Le joueur a été ajouté à la partie.", "joueursPartie");
        }
        else
            addLog("le joueur n'est pas connecté ou il n'y a plus de place dans la partie");
    }
    else//si il quit la partie
    {
        joueursPartie.removeAll(client);
        envoyerATous(client->GetNom(), "Partie", "Le joueur a été retiré de la partie.", "joueursPartie");
    }
    client->setDansLaPartie(joueursPartie.contains(client));
    actuJoueurs();
}


void FenServeur::KickP(Client *client)
{
    envoyerATous(client->GetNom(), "Connexion", "Vous avez été kick par le serveur.", "deconnexion");
    clientsT.find(client->GetNom()).value()->setConnecte(false);
    actuJoueurs();
}


//gestion de la partie
void FenServeur::LancerPartie()//NewPartie NouvellePartie
{
    emit sPartieLancee();
    bLancerPartie->setText("Lancer la partie (partie en cours)");
    bArreterPartie->setEnabled(true);
    //clients se disabled auto grace au signal sPartieLancee() (desactivation de l'entree/sortie et des grades)
    dernierJoueurQuiJoue = joueurQuiJoue;


    //partage cartes
    {//les joueurs ont déja fait leurs entrées/sorties dans la liste joueursPartie
        if(joueursPartie.size() < 2 || 4 < joueursPartie.size())//il faut de 2 a 4 joueurs
        {
            addLog("Erreur, il faut de de 2 à 4 personnes dans la partie");
            emit sPartieTerminee();
            return;
        }
        switch(joueursPartie.size())
        {
            case 2:
                joueursPartie.append(clientsT.find(strServeur2).value());//ajout de 2 clients inactifs (le 2eme est sur la ligne d'après)
            case 3:
                joueursPartie.append(clientsT.find(strServeur1).value());//ajout de 1 clients inactifs
                break;
        }


        QVector<int> *paquet52Cartes = cartes::generer52Cartes();
        QRandomGenerator generateur = QRandomGenerator::securelySeeded();
        int carte;

        for(int i=0; i<4; i++)
            joueursPartie[i]->clearCartes();
        for(int ligne=0; ligne<13; ligne++)
        {
            for(int i=0; i<4; i++)
            {
                carte = generateur.bounded(paquet52Cartes->size());//au départ génère entre 0 et 51
                joueursPartie[i]->ajouterCartes(paquet52Cartes->at(carte));//le trie est automatique
                paquet52Cartes->removeAt(carte);
            }
        }

    }
    //message et envoie carte
    QString joueurs="";
    for(int i=0; i<4; i++)
    {
        joueurs += *joueursPartie[i]->GetNom();//retourne le nom des joueurs a la suite
        if(i+1<4)//si il reste du monde
            joueurs += ", ";

    }

    envoyerATous(new QString("Serveur"), "Partie", "La partie a été lancée avec : " + joueurs, "lancé");

    for(int i=0; i<4; i++)//il y a 4 clients
        envoyerCartes(joueursPartie[i]->GetNom(), "Mes cartes", joueursPartie[i]->GetCartes(), "");


    clearPlateau();


    //on echange les cartes


    for(int i=0; i<4; i++)
        joueursPartie[i]->setNiveau(0);//reset tt les niveaux (doivent etre traités avant)
    addLog("L'échange des cartes est terminé");
    for(int i=0; i<4; i++)
    {
        if(joueursPartie[i]->GetNom() == strServeur1)
        {
            joueursPartie[i]->clearCartes();//vide ses cartes
            joueurFini(joueursPartie[i], false);//lui n'a pas gagné
        }
        else if(joueursPartie[i]->GetNom() == strServeur2)
        {
            joueursPartie[i]->clearCartes();//vide ses cartes
            joueurFini(joueursPartie[i], true);//lui a gagné
        }
    }
    clearPlateau();//après l'echange des cartes

    passerAuJoueur(joueurQuiCommence);

    bPasserJoueur->setEnabled(true);
}

void FenServeur::PasserJoueur()
{
    envoyerAPartie(new QString("Serveur"), "Partie", "Désolé " + *joueursPartie[joueurQuiJoue]->GetNom() + ", vous ne jourez pas maintenand", "pass");
    joueursPartie[joueurQuiJoue]->setPass(true);
    milieuLast->clear();//si il y a une carte ou rien demandée, plus besoin
    nextJoueur();
}


void FenServeur::resetPartie() //quand on recoit le signal sPartieTerminee
{
    bLancerPartie->setText("Lancer la partie");
    bArreterPartie->setDisabled(true);
    envoyerATous(new QString("Serveur"), "Partie", "La partie viens de se terminer.", "finPartie");
    joueurQuiJoue = joueurQuiCommence;
    joueurQuiCommence = (joueurQuiCommence+1) % 4;
    while(joueursPartie[joueurQuiCommence]->isIA())
        joueurQuiCommence = (joueurQuiCommence+1) % 4;

    cartesALaSuite=0;
    bPasserJoueur->setText("Passer le joueur");
    bPasserJoueur->setDisabled(true);
}

void FenServeur::bArreterPartiePrece()
{
    emit sPartieTerminee();
}


void FenServeur::updateSpinJoueurQuiCommenceProchainePartie()
{
    if(!joueursPartie.isEmpty())//sinon on pourra pas lancer la partie
        joueurQuiCommenceProchainePartie->setMaximum(joueursPartie.size());

    joueurQuiCommence = joueurQuiCommenceProchainePartie->value()-1;//update directe pour la prochaine partie
}
