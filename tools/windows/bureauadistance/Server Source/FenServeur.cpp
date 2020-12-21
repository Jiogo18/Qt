#include "FenServeur.h"

FenServeur::FenServeur()
{
    // Création et disposition des widgets de la fenêtre
    {
        etatServeur = new QLabel;
        boutonQuitter = new QPushButton(tr("Quitter"));
        connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));


        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(etatServeur);
        layout->addWidget(boutonQuitter);


        setLayout(layout);
        setWindowTitle(tr("Serveur - TrackPC"));
        }


    // Gestion du serveur
    {
        serveur = new QTcpServer(this);
        if (!serveur->listen(QHostAddress::Any, 25567)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 25567
        {
            // Si le serveur n'a pas été démarré correctement
            etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());
        }
        else
        {
            // Si le serveur a été démarré correctement

            connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
        }

        tailleMessage = 0;

    }
    afficherInfo();
    setMouseTracking(true);

    timerScreen = new QTimer(this);
    timerScreen->start(100);
    timerScreen->stop();
    connect(timerScreen, SIGNAL(timeout()), this, SLOT(actuScreen()));
}




void FenServeur::afficherInfo(bool estConnecte)
{
    QString clientCoo;
    if(clients.size() && estConnecte)
    {
        clientCoo = "Un client est connecté";
    }
    else
        clientCoo = "Aucun client n'est connecté";

    etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />") + clientCoo);
}


void FenServeur::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Alt)
    {
        stop = true;
        std::cout << "stoppé" << std::endl;
    }
    else if(event->key() == Qt::Key_Control)
    {
        stop = false;
        std::cout << "désactivation de l'arret" << std::endl;
    }
    else
        std::cout << "mauvaise touche" << std::endl;
}

void FenServeur::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}

void FenServeur::actuScreen()
{
    envoyerA("screen", grabScreens());
}



QPixmap FenServeur::grabScreens()
{
    auto screens = QGuiApplication::screens();
      QList<QPixmap> scrs;
      int w = 0, h = 0, p = 0;
      foreach (auto scr, screens) {
        QPixmap pix = scr->grabWindow(0);
        w += pix.width();
        if (h < pix.height()) h = pix.height();
        scrs << pix;
      }
      QPixmap final(w, h);
      QPainter painter(&final);
      final.fill(Qt::black);
      foreach (auto scr, scrs) {
        painter.drawPixmap(QPoint(p, 0), scr);
        p += scr.width();
      }
      return final;
}





//gestion de la connexion
void FenServeur::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    if(clients.size() == 0)
    {
        clients << nouveauClient;
        afficherInfo();
        connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
        tailleMessage = 0;
        timerScreen->start();
    }
    stop = false;


}

void FenServeur::donneesRecues()
{
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    if(clients.size()==0)
        return;
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0 || clients.size() == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(socket);

    while(true && clients.size())
    {
        if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
        {
            if (socket->bytesAvailable() < (int)sizeof(quint32)) // On n'a pas reçu la taille du message en entier
                 break;

            in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
        }

        // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
        if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
            break;
        tailleMessage = 0;


        // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
        QString message;
        in >> message;
        int x;
        in >> x;
        int y;
        in >> y;
        if(!stop)
        {
            QCoreApplication ::processEvents();
            if(message == "mouseMoveEvent")
            {

                float x2 = x;
                x2 /= 1000;
                x2 *= GetSystemMetrics( SM_CXSCREEN );
                if(x2 < 0)
                        x2 = 0;
                if(x2 > GetSystemMetrics( SM_CXSCREEN ))
                    x2 = GetSystemMetrics( SM_CXSCREEN );

                float y2 = y;
                y2 /= 1000;
                y2 *= GetSystemMetrics( SM_CYSCREEN );
                if(y2 < 0)
                    y2 = 0;
                if(y2 > GetSystemMetrics( SM_CYSCREEN ))
                    y2 =  GetSystemMetrics( SM_CYSCREEN );
                SetCursorPos(x2, y2);
                //std::cout << "recu : (" << x << "," << y << ")" << std::endl;
            }
            else if(message == "click")
            {
                switch(x)
                {
                case 1:
                    if(y)
                    {
                        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    }
                    else
                        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                    break;
                case 2:
                    if(y)
                    {
                        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                    }
                    else
                        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                    break;
                case 3:
                    if(y)
                    {
                        mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
                    }
                    else
                        mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
                    break;
                }

            }
            else if(message == "clavier")
            {
                BYTE modifieur=VK_SPACE; //si y=0, mais inutile
                BYTE touche;
                switch (y)
                {
                case 1:
                    modifieur = VK_CONTROL;
                    break;
                case 2:
                    modifieur = VK_SHIFT;
                    break;
                case 3:
                    modifieur = VK_MENU;
                    break;
                case 4:
                    modifieur = VK_LWIN; //on peut aussi prendre RWIN
                    break;

                }
                switch (x)
                {
                case Qt::Key_0:
                    touche = VkKeyScanA('0');
                    break;
                case Qt::Key_1:
                    touche = VkKeyScanA('1');
                    break;
                case Qt::Key_2:
                    touche = VkKeyScanA('2');
                    break;
                case Qt::Key_3:
                    touche = VkKeyScanA('3');
                    break;
                case Qt::Key_4:
                    touche = VkKeyScanA('4');
                    break;
                case Qt::Key_5:
                    touche = VkKeyScanA('5');
                    break;
                case Qt::Key_6:
                    touche = VkKeyScanA('6');
                    break;
                case Qt::Key_7:
                    touche = VkKeyScanA('7');
                    break;
                case Qt::Key_8:
                    touche = VkKeyScanA('8');
                    break;
                case Qt::Key_9:
                    touche = VkKeyScanA('9');
                    break;
                case Qt::Key_A:
                    touche = VkKeyScanA('a');
                    break;
                case Qt::Key_B:
                    touche = VkKeyScanA('b');
                    break;
                case Qt::Key_C:
                    touche = VkKeyScanA('c');
                    break;
                case Qt::Key_D:
                    touche = VkKeyScanA('d');
                    break;
                case Qt::Key_E:
                    touche = VkKeyScanA('e');
                    break;
                case Qt::Key_F:
                    touche = VkKeyScanA('f');
                    break;
                case Qt::Key_G:
                    touche = VkKeyScanA('g');
                    break;
                case Qt::Key_H:
                    touche = VkKeyScanA('h');
                    break;
                case Qt::Key_I:
                    touche = VkKeyScanA('i');
                    break;
                case Qt::Key_J:
                    touche = VkKeyScanA('j');
                    break;
                case Qt::Key_K:
                    touche = VkKeyScanA('k');
                    break;
                case Qt::Key_L:
                    touche = VkKeyScanA('l');
                    break;
                case Qt::Key_M:
                    touche = VkKeyScanA('m');
                    break;
                case Qt::Key_N:
                    touche = VkKeyScanA('n');
                    break;
                case Qt::Key_O:
                    touche = VkKeyScanA('o');
                    break;
                case Qt::Key_P:
                    touche = VkKeyScanA('p');
                    break;
                case Qt::Key_Q:
                    touche = VkKeyScanA('q');
                    break;
                case Qt::Key_R:
                    touche = VkKeyScanA('r');
                    break;
                case Qt::Key_S:
                    touche = VkKeyScanA('s');
                    break;
                case Qt::Key_T:
                    touche = VkKeyScanA('t');
                    break;
                case Qt::Key_U:
                    touche = VkKeyScanA('u');
                    break;
                case Qt::Key_V:
                    touche = VkKeyScanA('v');
                    break;
                case Qt::Key_W:
                    touche = VkKeyScanA('w');
                    break;
                case Qt::Key_X:
                    touche = VkKeyScanA('x');
                    break;
                case Qt::Key_Y:
                    touche = VkKeyScanA('y');
                    break;
                case Qt::Key_Z:
                    touche = VkKeyScanA('z');
                    break;
                case Qt::Key_Alt:
                    touche = VK_MENU;
                    break;
                case Qt::Key_Control:
                    touche = VK_CONTROL;
                    break;
                case Qt::Key_Shift:
                    touche = VK_SHIFT;
                    break;
                case Qt::Key_Meta:
                    touche = VK_LWIN;
                    break;
                case Qt::Key_Space:
                    touche = VK_SPACE;
                    break;
                case Qt::Key_Escape:
                    touche = VK_ESCAPE;
                    break;
                case Qt::Key_Return:
                    touche = VK_RETURN;
                    break;
                case Qt::Key_Delete:
                    touche = VK_DELETE;
                    break;
                case Qt::Key_Backspace:
                    touche = VK_BACK;
                    break;
                case Qt::Key_Tab:
                    touche = VK_TAB;
                    break;
                case Qt::Key_Left:
                    touche = VK_LEFT;
                    break;
                case Qt::Key_Right:
                    touche = VK_RIGHT;
                    break;
                case Qt::Key_Up:
                    touche = VK_UP;
                    break;
                case Qt::Key_Down:
                    touche = VK_DOWN;
                    break;
                case Qt::Key_ApplicationRight:
                    touche = VK_APPS;
                    break;
                case Qt::Key_F4:
                    touche = VK_F4;
                    break;


                }
                if(y)
                    keybd_event(modifieur, 0, 0, 0);
                keybd_event(touche, 0, 0, 0);
                keybd_event(touche, 0, 2, 0);
                if(y)
                    keybd_event(modifieur, 0, 2, 0);


            }
            else
                std::cout << "erreur reception" << std::endl;

        }
        else
            std::cout << "msg blocké" << std::endl;
    }

}


void FenServeur::deconnexionClient()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    clients.removeOne(socket);
    socket->deleteLater();
    afficherInfo(false);
    timerScreen->stop();
}

void FenServeur::envoyerA(const QString &message, const QPixmap &screen)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint32) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out << screen;
    out << cursor().pos().x();
    out << cursor().pos().y();
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint32) (paquet.size()- sizeof(quint32)); // On écrase le 0 qu'on avait réservé par la longueur du message
    quint32 valeurTemp=paquet.size() - sizeof(quint32);
    if(clients.size() && message != "")
    {
        clients[0]->write(paquet);
        //std::cout << "envoyé : screen" << std::endl;
    }
    else
        std::cout << "erreur envoie" << std::endl;

}
