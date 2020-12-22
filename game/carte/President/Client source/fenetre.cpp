#include "fenetre.h"

fenetre::fenetre()
{
    setupUi(this);
    version = "1.3";

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    tailleMessage = 0;

    nbG=nbH=nbD=0;
    milieu.resize(4);
    player.resize(13);
    ActuPlayer();


    connect(actionSe_conntecter, SIGNAL(triggered()), this, SLOT(actSeConnecter()));
    connect(actionInformation, SIGNAL(triggered()), this, SLOT(actInformation()));
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionParametre, SIGNAL(triggered()), this, SLOT(actParametre()));

    fenC = new connexion();
    connect(fenC, SIGNAL(SBConnexion()), this, SLOT(bConnexionPresse()));
    fenInfo = new information();
    fenParam = new parametre();
    connect(fenParam, SIGNAL(addErreur(QString, QString)), this, SLOT(addErreur(QString, QString)));

    ip = "127.0.0.1";
    mdpServ = "";
    port = 25567;
    pseudo = "";
    mdp = "";
    connect(bcarte01, SIGNAL(clicked(bool)), this, SLOT(actuSelection01()));
    connect(bcarte02, SIGNAL(clicked(bool)), this, SLOT(actuSelection02()));
    connect(bcarte03, SIGNAL(clicked(bool)), this, SLOT(actuSelection03()));
    connect(bcarte04, SIGNAL(clicked(bool)), this, SLOT(actuSelection04()));
    connect(bcarte05, SIGNAL(clicked(bool)), this, SLOT(actuSelection05()));
    connect(bcarte06, SIGNAL(clicked(bool)), this, SLOT(actuSelection06()));
    connect(bcarte07, SIGNAL(clicked(bool)), this, SLOT(actuSelection07()));
    connect(bcarte08, SIGNAL(clicked(bool)), this, SLOT(actuSelection08()));
    connect(bcarte09, SIGNAL(clicked(bool)), this, SLOT(actuSelection09()));
    connect(bcarte10, SIGNAL(clicked(bool)), this, SLOT(actuSelection10()));
    connect(bcarte11, SIGNAL(clicked(bool)), this, SLOT(actuSelection11()));
    connect(bcarte12, SIGNAL(clicked(bool)), this, SLOT(actuSelection12()));
    connect(bcarte13, SIGNAL(clicked(bool)), this, SLOT(actuSelection13()));

    connect(benvoyer, SIGNAL(clicked(bool)), this, SLOT(envoyerCartes()));
    connect(bPasser, SIGNAL(clicked(bool)), this, SLOT(bPasserPresse()));

    connect(bEnvoyerMsg, SIGNAL(clicked(bool)), this, SLOT(EnvoyerChat()));
    CartesML->hide();

    sonAlerteJouerTimer = new QTimer;
    sonAlerteJouerTimer->setInterval(fenParam->getSonIntervalAlerte()*1000);//30 sec par default
    connect(sonAlerteJouerTimer, SIGNAL(timeout()), this, SLOT(sonAlerteJouer()));

    jouerSon("alerte");
}

void fenetre::closeEvent(QCloseEvent *event)
{
    deconnecte();
    qApp->quit();
    event->accept();
}





void fenetre::ActuPlayer()
{
    carteG01->hide();
    carteG02->hide();
    carteG03->hide();
    carteG04->hide();
    carteG05->hide();
    carteG06->hide();
    carteG07->hide();
    carteG08->hide();
    carteG09->hide();
    carteG10->hide();
    carteG11->hide();
    carteG12->hide();
    carteG13->hide();
    switch (nbG)
    {
        case 13:
            carteG13->show();
        case 12:
            carteG12->show();
        case 11:
            carteG11->show();
        case 10:
            carteG10->show();
        case 9:
            carteG09->show();
        case 8:
            carteG08->show();
        case 7:
            carteG07->show();
        case 6:
            carteG06->show();
        case 5:
            carteG05->show();
        case 4:
            carteG04->show();
        case 3:
            carteG03->show();
        case 2:
            carteG02->show();
        case 1:
            carteG01->show();
        break;
    }

    carteH01->hide();
    carteH02->hide();
    carteH03->hide();
    carteH04->hide();
    carteH05->hide();
    carteH06->hide();
    carteH07->hide();
    carteH08->hide();
    carteH09->hide();
    carteH10->hide();
    carteH11->hide();
    carteH12->hide();
    carteH13->hide();
    switch (nbH)
    {
        case 13:
            carteH13->show();
        case 12:
            carteH12->show();
        case 11:
            carteH11->show();
        case 10:
            carteH10->show();
        case 9:
            carteH09->show();
        case 8:
            carteH08->show();
        case 7:
            carteH07->show();
        case 6:
            carteH06->show();
        case 5:
            carteH05->show();
        case 4:
            carteH04->show();
        case 3:
            carteH03->show();
        case 2:
            carteH02->show();
        case 1:
            carteH01->show();
        break;
    }

    carteD01->hide();
    carteD02->hide();
    carteD03->hide();
    carteD04->hide();
    carteD05->hide();
    carteD06->hide();
    carteD07->hide();
    carteD08->hide();
    carteD09->hide();
    carteD10->hide();
    carteD11->hide();
    carteD12->hide();
    carteD13->hide();
    switch (nbD)
    {
        case 13:
            carteD13->show();
        case 12:
            carteD12->show();
        case 11:
            carteD11->show();
        case 10:
            carteD10->show();
        case 9:
            carteD09->show();
        case 8:
            carteD08->show();
        case 7:
            carteD07->show();
        case 6:
            carteD06->show();
        case 5:
            carteD05->show();
        case 4:
            carteD04->show();
        case 3:
            carteD03->show();
        case 2:
            carteD02->show();
        case 1:
            carteD01->show();
        break;
    }

    carteM01->hide();
    carteM02->hide();
    carteM03->hide();
    carteM04->hide();
    switch (milieu.size())
    {
    case 4:
        carteM04->show();
        carteM04->setPixmap(QPixmap("cartes/" + GetCarte(milieu.at(3)) + ".bmp"));
    case 3:
        carteM03->show();
        carteM03->setPixmap(QPixmap("cartes/" + GetCarte(milieu.at(2)) + ".bmp"));
    case 2:
        carteM02->show();
        carteM02->setPixmap(QPixmap("cartes/" + GetCarte(milieu.at(1)) + ".bmp"));
    case 1:
        carteM01->show();
        carteM01->setPixmap(QPixmap("cartes/" + GetCarte(milieu.at(0)) + ".bmp"));
        break;
    }



    carte01->hide();
    carte02->hide();
    carte03->hide();
    carte04->hide();
    carte05->hide();
    carte06->hide();
    carte07->hide();
    carte08->hide();
    carte09->hide();
    carte10->hide();
    carte11->hide();
    carte12->hide();
    carte13->hide();
    if(!player.size())
        return;

    for(int i=0; i<13; i++)
    {

        if(player.at(i)) //bug ici
        {
            QString acces = "cartes/" + GetCarte(player.at(i)) + ".bmp";
            switch (i)
            {
                case 0:
                    carte01->show();
                    lblCarte01->setPixmap(acces);
                break;
                case 1:
                    carte02->show();
                    lblCarte02->setPixmap(acces);
                break;
                case 2:
                    carte03->show();
                    lblCarte03->setPixmap(acces);
                break;
                case 3:
                    carte04->show();
                    lblCarte04->setPixmap(acces);
                break;
                case 4:
                    carte05->show();
                    lblCarte05->setPixmap(acces);
                break;
                case 5:
                    carte06->show();
                    lblCarte06->setPixmap(acces);
                break;
                case 6:
                    carte07->show();
                    lblCarte07->setPixmap(acces);
                break;
                case 7:
                    carte08->show();
                    lblCarte08->setPixmap(acces);
                break;
                case 8:
                    carte09->show();
                    lblCarte09->setPixmap(acces);
                break;
                case 9:
                    carte10->show();
                    lblCarte10->setPixmap(acces);
                break;
                case 10:
                    carte11->show();
                    lblCarte11->setPixmap(acces);
                break;
                case 11:
                    carte12->show();
                    lblCarte12->setPixmap(acces);
                break;
                case 12:
                    carte13->show();
                    lblCarte13->setPixmap(acces);
                break;

            }
        }


    }





}


void fenetre::NewPartie(QVector<int> cartes, QList<QString> autresJoueurs)
{
    for(int i=1; i<=13; i++)
        changeEtatCarte(i, false);
    nbG = nbH = nbD = 13;
    player = cartes;
    passer = false;
    milieu.clear();
    CartesG->setDisabled(true);
    CartesH->setDisabled(true);
    CartesD->setDisabled(true);
    MesCartes->setDisabled(true);
    CartesG->setTitle("Gauche");
    CartesH->setTitle("Haut");
    CartesD->setTitle("Droite");
    MesCartes->setTitle("Mes cartes");

    if(autresJoueurs.contains(pseudo))
    {
        while(autresJoueurs.first() != pseudo)
        {
            autresJoueurs.append(autresJoueurs.first());
            autresJoueurs.removeFirst();
        }
        MesCartes->setEnabled(true);
        if(autresJoueurs[1] != QString(""))
        {
            CartesG->setEnabled(true);
            CartesG->setTitle(autresJoueurs.at(1));
        }
        if(autresJoueurs[2] != QString(""))
        {
            CartesH->setEnabled(true);
            CartesH->setTitle(autresJoueurs.at(2));
        }
        if(autresJoueurs[3] != QString(""))
        {
            CartesD->setEnabled(true);
            CartesD->setTitle(autresJoueurs.at(3));
        }

        jouerSon("start");
    }
    else
    {
        addErreur("Erreur carte", "Vous avez recu des cartes et pourtant vous n'etes pas dans la partie");
    }



    ActuPlayer();
}

void fenetre::finPartie()
{
    joueG->setValue(0);
    joueH->setValue(0);
    joueD->setValue(0);
    joueMoi->setValue(0);
    sonAlerteJouerTimer->stop();
    benvoyer->hide();
    bPasser->setDisabled(true);//PasserDisabled
    ActuPlayer();
}


QString fenetre::GetCarte(int carte)//get nom du fichier de la carte
{
    if(carte != 0)
    {
        if(carte<10)
        {
            return "0" + QString::number(carte);
        }
        else
            return QString::number(carte);
    }
    return "error";
}



void fenetre::envoyerCartes(bool cartesValidees)
{
    if(socket->state() != QTcpSocket::ConnectedState)
        return;
    if(!cartesValidees)
    {//il faut envoyer des cartes
        MesCartes->setDisabled(true);
        benvoyer->setDisabled(true);
        bPasser->setDisabled(true);//PasserDisabled
        sonAlerteJouerTimer->stop();
        if(!passer)
        {}
        QVector<int> cartes;
        for(int i=0; i<player.size(); i++)
            if(getEtatCarte(i+1))//si elle est cochée
            {
                cartes.append(i);
            }
        //envoie des cartes
        {
            QByteArray paquet;
            QDataStream out(&paquet, QIODevice::WriteOnly);

            // On prépare le paquet à envoyer

            out << (quint32) 0;
            out << mdpServ;
            out << pseudo;
            out << mdp;
            out << QString("carte");
            out << QString("Info Partie");
            out << QString("Mes cartes ont été envoyées");
            out << cartes;
            out.device()->seek(0);
            out << (quint32) (paquet.size() - sizeof(quint32));

            socket->write(paquet); // On envoie le paquet
        }

    }
    else//quand on recoit la reponse du serveur
    {//ont peut supr les cartes
        for(int i=0; i<player.size(); i++)
            if(getEtatCarte(i+1))
            {
                player.replace(i,0);
            }
        MesCartes->setEnabled(true);

        ActuPlayer();

    }
}

void fenetre::bPasserPresse()
{
    passer = true;
    envoyerMessage("Partie", "Je passe !");
}





void fenetre::bConnexionPresse()
{
    // On annonce sur la fenêtre qu'on est en train de se connecter
    fenC->setEtat("Tentative de connexion en cours...", 0);
    benvoyer->hide();

    ip = fenC->getServeur();
    mdpServ = fenC->getMdpServ();
    port = fenC->getPort();
    pseudo = fenC->getPseudo();
    mdp = fenC->getMdp();


    socket->abort(); // On désactive les connexions précédentes s'il y en a
    socket->connectToHost(ip, port); // On se connecte au serveur demandé

}

void fenetre::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        bEnvoyerMsg->click();
}

void fenetre::jouerSon(QString son)
{
#if hasMultimedia
    sonStart.setSource(QUrl::fromLocalFile("sons/start.wav"));
    sonWin.setSource(QUrl::fromLocalFile("sons/win.wav"));
    sonCarte.setSource(QUrl::fromLocalFile("sons/carte.wav"));
    sonMsg.setSource(QUrl::fromLocalFile("sons/message.wav"));
    sonAlerte.setSource(QUrl::fromLocalFile("sons/alerte.wav"));
    sonStart.setVolume(fenParam->getSonVolumeStart()/100.0);
    sonWin.setVolume(fenParam->getSonVolumeWin()/100.0);
    sonCarte.setVolume(fenParam->getSonVolumeNext()/100.0);
    sonMsg.setVolume(fenParam->getSonVolumeChat()/100.0);
    sonAlerte.setVolume(fenParam->getSonVolumeAlerte()/100.0);

    if(son == QString("start"))
    {
        sonStart.play();
    }
    else if(son == QString("win"))
    {
        sonWin.play();
    }
    else if(son == QString("carte"))
    {
        sonCarte.play();
    }
    else if(son == QString("message"))
    {
        sonMsg.play();
    }
    else if(son == QString("alerte"))
    {
        sonAlerte.play();
    }
#endif // hasMultimedia
}

void fenetre::addErreur(QString type, QString erreur)
{
    if(fenParam->getChatErreur())
        addMessage("", type, erreur);
    qDebug(QString(type + " : " + erreur).toStdString().c_str());
}



// On a reçu un paquet (ou un sous-paquet)
void fenetre::donneesRecues()
{
    /* Même principe que lorsque le serveur reçoit un paquet :
    On essaie de récupérer la taille du message
    Une fois qu'on l'a, on attend d'avoir reçu le message entier (en se basant sur la taille annoncée tailleMessage)
    */
    QDataStream in(socket);

    while(true)
    {
        if (tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint32))
                 break;

            in >> tailleMessage;
        }

        if (socket->bytesAvailable() < tailleMessage)
            break;
        // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
        tailleMessage = 0;


        // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
        QString Rpseudo;
        in >> Rpseudo;
        QString version;
        in >> version;
        QString type;
        in >> type;
        QString messageRecu;
        QVector<int> cartes;
        QList<QString> autresJoueurs;
        QList<int> autresJoueurs2;

        if(version == QString("message"))
        {
            in >> messageRecu;
        }
        else if(version == QString("carte"))
        {
            in >> cartes;
            in >> autresJoueurs;
            in >> autresJoueurs2;
        }

        QString action;
        in >> action;
        if(version == QString("message"))
        {
            Analyse(Rpseudo, type, messageRecu, action);
        }
        else if(version == QString("carte"))
        {
            Analyse(Rpseudo, type, cartes, autresJoueurs, autresJoueurs2, action);
        }
    }
}


void fenetre::Analyse(QString Rpseudo, QString type, QString message, QString action)//analyseMsg analyseMessage
{
    if(action == QString("finPartie"))
        finPartie();
    if(action == QString("gagnant"))
        jouerSon("win");

    if(Rpseudo == pseudo || Rpseudo == "Undefined")
    {
        if(action == QString("deconnexion"))
            deconnecte();
        if(action == QString("joue"))
        {
            bPasser->setEnabled(true);
            benvoyer->setEnabled(true);
            MesCartes->setEnabled(true);
            sonAlerteJouerTimer->start();
            ActuPlayer();
        }

        if(type == QString("Connexion") || type == QString("Connexion Erreur"))
        {
            if(action == QString("connexion") || action == QString("deconnexion"))
                fenC->setEtat(message, 2);
        }
        else if(type == QString("Erreur"))
        {
            MesCartes->setEnabled(true);
            ActuPlayer();
        }

    }
    if(type == "Partie")
    {
        lblCarteOuRien->setText("");
        CartesML->hide();
        if(action == QString("joue"))
        {
            joueurQuiJoue = Rpseudo;
            joueG->setValue(0);
            joueH->setValue(0);
            joueD->setValue(0);
            joueMoi->setValue(0);
            sonAlerteJouerTimer->stop();
            benvoyer->hide();

            if(pseudo == Rpseudo)
            {
                joueMoi->setValue(1);
                sonAlerteJouerTimer->setInterval(fenParam->getSonIntervalAlerte()*1000);//30 sec
                sonAlerteJouerTimer->start();
                benvoyer->show();
                benvoyer->setDisabled(true);
                for(int i=1; i<=13; i++)
                {
                    if(getEtatCarte(i) && cartes::carteSansCouleur(milieu.at(0)) <= cartes::carteSansCouleur(player[i-1]))
                    {
                        benvoyer->setEnabled(true);
                        break;
                    }
                }

                bPasser->setEnabled(true);
                if(passer)
                    bPasserPresse();

            }
            else if(CartesG->title().startsWith(Rpseudo))
            {
                joueG->setValue(1);
            }
            else if(CartesH->title().startsWith(Rpseudo))
            {
                joueH->setValue(1);
            }
            else if(CartesD->title().startsWith(Rpseudo))
            {
                joueD->setValue(1);
            }

            if(pseudo != Rpseudo)
            {
                sonAlerteJouerTimer->stop();
                bPasser->setDisabled(true);//PasserDisabled
            }

        }
        else if(message == QString("Mes cartes ont été envoyées"))
        {
            ActuPlayer();
        }
        else if(action == QString("carte ou rien"))
        {
            if(pseudo == Rpseudo)
            {
                MesCartes->setEnabled(true);
                bPasser->setEnabled(true);
                ActuPlayer();
                lblCarteOuRien->setText(message);
            }
            else
                lblCarteOuRien->setText(message + "(" + Rpseudo + ")");
            CartesML->show();
            if(milieuL.size()!=0)
            {
                carteML->setPixmap(QPixmap("cartes/" + GetCarte(milieuL.at(0)) + ".bmp"));
            }
            else
                carteML->setPixmap(QPixmap());
        }

    }

    //si le msg contient :                                                                     et que c interdit :
    if(action == QString("carte ou rien") && !fenParam->getChatCarteOuRien())
        return;
    if((message.startsWith("Le joueur ") && message.endsWith(" joue.") && action == QString("joue")) && !fenParam->getChatJoueur())
        return;//il faut pas le mettre dans le chat
    if((action == QString("pass") || message == QString("Je passe !")) && !fenParam->getChatPass())
        return;
    if(action == QString("reset") && !fenParam->getChatReset())
        return;
    if(action == QString("carte ultime") && !fenParam->getChatUltimeCarte())
        return;


    addMessage(Rpseudo, type, message);

}

void fenetre::Analyse(QString Rpseudo, QString type, QVector<int> cartes, QList<QString> autresJoueurs, QList<int> autresJoueurs2, QString action)
{//analyseCarte
    if(cartes.size() <= 4 && type == QString("Milieu"))
    {
        milieuL = milieu;
        milieu = cartes;
        jouerSon("carte");
        nbG=autresJoueurs2[autresJoueurs.indexOf(CartesG->title())];
        nbH=autresJoueurs2[autresJoueurs.indexOf(CartesH->title())];
        nbD=autresJoueurs2[autresJoueurs.indexOf(CartesD->title())];

        if(milieu.isEmpty())
            passer = false;
        if(!MesCartes->isEnabled()) //supr les cartes envoyées
        {
            envoyerCartes(true);//dire a MesCartes qu'on a bien envoyé les cartes et qu'elles ont ete validées
        }
        ActuPlayer();
        if(fenParam->getChatCartes() && !milieu.isEmpty())//si il faut mettre le msg et que milieu est pas vide
            addMessage(Rpseudo, type, "Nombres de cartes : " + QString::number(milieu.size()) + ", Valeur : " + cartes::carteEnText(milieu.at(0)));
        return;
    }
    else if(cartes.size() == 13 && Rpseudo == pseudo)
    {
        NewPartie(cartes, autresJoueurs);
    }
    else
        addErreur("Erreur Cartes", "Un vector a été recut mais n'a pas été analysé");
    if(fenParam->getChatCartes())//si on autorise
    addMessage(Rpseudo, "Cartes", type + " ont été reçut.");
}






// Ce slot est appelé lorsque la connexion au serveur a réussi
void fenetre::connecte()
{
    fenC->setEtat("Connexion réussie !\nVous avez le pseudo : " + pseudo + "\net le code d'accès : " + mdp, 1);

    Sleep(1000);
    envoyerMessage("Connexion", "Le joueur viens d'arriver, il a la version : "+version);
}

// Ce slot est appelé lorsqu'on est déconnecté du serveur
void fenetre::deconnecte()
{
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        socket->abort();//execute le slot deconnecte()
        return;
    }
    fenC->setEtat("Déconnecté du serveur", 2);
    addMessage("", "Connexion", "Le serveur n'est plus accessible");
    benvoyer->hide();
    player.clear();
    milieu.clear();
    nbG = nbH = nbD = 0;
    joueG->setValue(0);
    joueH->setValue(0);
    joueD->setValue(0);
    joueMoi->setValue(0);
    sonAlerteJouerTimer->stop();

    MesCartes->setEnabled(true);
        bPasser->setEnabled(true);
    lblCarteOuRien->setText("");
    CartesML->hide();
    milieuL.clear();
    if(milieuL.size()!=0)
    {
        carteML->setPixmap(QPixmap("cartes/" + GetCarte(milieuL.at(0)) + ".bmp"));
    }
    else
        carteML->setPixmap(QPixmap());

    ActuPlayer();
}

// Ce slot est appelé lorsqu'il y a une erreur
void fenetre::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            fenC->setEtat("ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.", 2);
            break;
        case QAbstractSocket::ConnectionRefusedError:
            fenC->setEtat("ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.", 2);
            break;
        case QAbstractSocket::RemoteHostClosedError:
            fenC->setEtat("ERREUR : le serveur a coupé la connexion.", 2);
            break;
        default:
            fenC->setEtat("ERREUR : " + socket->errorString() + "", 2);
    }
    benvoyer->hide();

    deconnecte();//sécuritée
}


void fenetre::envoyerMessage(QString type, QString msg)
{
    if(socket->state() != QTcpSocket::ConnectedState)
        return;
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer

    out << (quint32) 0;
    out << mdpServ;
    out << pseudo;
    out << mdp;
    out << QString("message");
    out << type;
    out << msg;
    out.device()->seek(0);
    out << (quint32) (paquet.size() - sizeof(quint32));

    socket->write(paquet); // On envoie le paquet
}



void fenetre::actSeConnecter() //si fenetre de connexion ouverte
{
    fenC->affich();
}
void fenetre::actInformation()
{
    fenInfo->show();
}
void fenetre::actParametre()
{
    fenParam->show();
}


void fenetre::actuSelection01() {actuSelection(1, bcarte01->isChecked());}
void fenetre::actuSelection02() {actuSelection(2, bcarte02->isChecked());}
void fenetre::actuSelection03() {actuSelection(3, bcarte03->isChecked());}
void fenetre::actuSelection04() {actuSelection(4, bcarte04->isChecked());}
void fenetre::actuSelection05() {actuSelection(5, bcarte05->isChecked());}
void fenetre::actuSelection06() {actuSelection(6, bcarte06->isChecked());}
void fenetre::actuSelection07() {actuSelection(7, bcarte07->isChecked());}
void fenetre::actuSelection08() {actuSelection(8, bcarte08->isChecked());}
void fenetre::actuSelection09() {actuSelection(9, bcarte09->isChecked());}
void fenetre::actuSelection10() {actuSelection(10, bcarte10->isChecked());}
void fenetre::actuSelection11() {actuSelection(11, bcarte11->isChecked());}
void fenetre::actuSelection12() {actuSelection(12, bcarte12->isChecked());}
void fenetre::actuSelection13() {actuSelection(13, bcarte13->isChecked());}

void fenetre::actuSelection(int carte, bool coche)
{
    benvoyer->setDisabled(true);
    if(!coche)//si decoche, tout decocher
    {
        for(int i=1; i<=player.size(); i++)
            changeEtatCarte(i, false); //desactive toutes les cartes
    }
    else
    {

        if(milieu.size())
        {//decocher toutes les cartes qui ont pas le meme nb

            if(cartes::carteSansCouleur(player[carte-1]) < cartes::carteSansCouleur(milieu[0]))
            {
                changeEtatCarte(carte, false);
                return; //arrete si la carte que tu mes est trop petite
            }
            int nbCochees=0; //de la meme valeur
            int premiereCarteValeurOK=0;
            for(int i=1; i<=player.size(); i++)
            {
                    if(cartes::carteSansCouleur(player[carte-1]) == cartes::carteSansCouleur(player[i-1]))
                    {
                        nbCochees++;
                        if(nbCochees==1)
                        {
                            premiereCarteValeurOK=i; //recupere la premiere carte cochee de la valeur demandée
                        }
                    }
                    changeEtatCarte(i, false); //on desactive tout, reactive plus tard
            }

            for(int i=milieu.size(); i; i--)
            {
                if(cartes::carteSansCouleur(player[i+premiereCarteValeurOK-2]) != cartes::carteSansCouleur(player[carte-1]))
                {
                    break; //si il y a pas asser de carte ont fait rien
                }
                else
                {
                    changeEtatCarte(i+premiereCarteValeurOK-1, true);
                    benvoyer->setEnabled(true);
                }

            }
        }
        else
        {
            for(int i=1; i<=player.size(); i++)
                if(cartes::carteSansCouleur(player[carte-1]) != cartes::carteSansCouleur(player[i-1]))
                    changeEtatCarte(i, false);
            benvoyer->setEnabled(true);
        }
    }

}

void fenetre::changeEtatCarte(int carte, bool checked)
{
    switch (carte)
    {
    case 1:
        bcarte01->setChecked(checked);
        break;
    case 2:
        bcarte02->setChecked(checked);
        break;
    case 3:
        bcarte03->setChecked(checked);
        break;
    case 4:
        bcarte04->setChecked(checked);
        break;
    case 5:
        bcarte05->setChecked(checked);
        break;
    case 6:
        bcarte06->setChecked(checked);
        break;
    case 7:
        bcarte07->setChecked(checked);
        break;
    case 8:
        bcarte08->setChecked(checked);
        break;
    case 9:
        bcarte09->setChecked(checked);
        break;
    case 10:
        bcarte10->setChecked(checked);
        break;
    case 11:
        bcarte11->setChecked(checked);
        break;
    case 12:
        bcarte12->setChecked(checked);
        break;
    case 13:
        bcarte13->setChecked(checked);
        break;
    }
}

bool fenetre::getEtatCarte(int carte)
{
    switch(carte)
    {
    case 1:
        return bcarte01->isChecked();
    case 2:
        return bcarte02->isChecked();
    case 3:
        return bcarte03->isChecked();
    case 4:
        return bcarte04->isChecked();
    case 5:
        return bcarte05->isChecked();
    case 6:
        return bcarte06->isChecked();
    case 7:
        return bcarte07->isChecked();
    case 8:
        return bcarte08->isChecked();
    case 9:
        return bcarte09->isChecked();
    case 10:
        return bcarte10->isChecked();
    case 11:
        return bcarte11->isChecked();
    case 12:
        return bcarte12->isChecked();
    case 13:
        return bcarte13->isChecked();
    }
    return false;
}






void fenetre::addMessage(QString Rpseudo, QString type, QString msg)
{
    if(Rpseudo == "")
        Rpseudo = pseudo;
    if(type.startsWith("[") && type.endsWith("]"))
        TChat->append(type + "  <strong>" + Rpseudo + "</strong> : " + msg);
    else
        TChat->append("[" + type + "]" + "  <strong>" + Rpseudo + "</strong> : " + msg);

    if(type==QString("Chat") && Rpseudo != pseudo)
    {
        jouerSon("message");
    }
}

void fenetre::EnvoyerChat()
{
    QString msg = message->text();
    if(msg == "" || msg == " ")
        return;
    if(!msg.startsWith("/") && socket->state() == QTcpSocket::ConnectedState)
    {
        emit envoyerMessage("Chat", message->text());
    }
    else if(msg.startsWith("/"))
    {

        if(msg == "/help")
        {
            TChat->append("");
            TChat->append("<strong>Liste des commandes :</strong>");
            TChat->append("<strong>/help</strong> : Affiche les commandes disponnibles");
            TChat->append("<strong>/clear</strong> : Vide le chat");
            TChat->append("<strong>/pass [pseudo]</strong> : Forcer le joueur a passer (50%)");
            TChat->append("");
        }
        else if(msg == "/clear")
        {
            TChat->clear();
        }
        else if(msg.left(5) == "/pass")
        {
            if(socket->state() == QTcpSocket::ConnectedState)
                envoyerMessage("Commande", msg);
        }
        else
            TChat->append("La commande <strong>" + message->text() + "</strong> n'a pas été trouvée.");

    }
    if(socket->state() == QTcpSocket::ConnectedState || msg.startsWith("/"))//si on est pas coo ou que c une commande
        message->clear();
}

void fenetre::sonAlerteJouer()
{
    if(sonAlerteJouerTimer->interval()!=fenParam->getSonIntervalAlerte()*1000)
        sonAlerteJouerTimer->setInterval(fenParam->getSonIntervalAlerte()*1000);//30 sec par default
    if(joueMoi->value()==0)
    {
        sonAlerteJouerTimer->stop();
        return;
    }

    jouerSon("alerte");
}
