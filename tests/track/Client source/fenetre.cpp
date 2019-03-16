#include "fenetre.h"

fenetre::fenetre()
{
    //set fenetre
    {
        setWindowTitle("Client - TrackPC");

        QVBoxLayout *layout = new QVBoxLayout;
        QFormLayout *loption = new QFormLayout;
        Tadresse = new QLineEdit("127.0.0.1");
        loption->addRow(new QLabel("Adresse : "), Tadresse);
        Sport = new QSpinBox;
        Sport->setMaximum(100000);
        Sport->setValue(25567);
        loption->addRow(new QLabel("Port : "), Sport);

        layout->addLayout(loption);

        etat = new QPlainTextEdit;
        layout->addWidget(etat);

        lblFPS = new QLabel("Fps : 0");
        Fps = 0;
        layout->addWidget(lblFPS);
        timerFps = new QTimer(this);
        connect(timerFps, SIGNAL(timeout()), this, SLOT(resetFps()));
        timerFps->start(1000);

        timerCoo = new QTimer(this);
        connect(timerCoo, SIGNAL(timeout()), this, SLOT(mouvement()));
        timerCoo->start(10000); // a remettre a 100
        timerCoo->stop();



        QHBoxLayout *lBoutons = new QHBoxLayout;
        bLancer = new QPushButton("Lancer");
        lBoutons->addWidget(bLancer);
        connect(bLancer, SIGNAL(clicked()), this, SLOT(bLancerPresse()));
        bArreter = new QPushButton("Arreter");
        lBoutons->addWidget(bArreter);
        bArreter->setDisabled(true);
        connect(bArreter, SIGNAL(clicked()), this, SLOT(bArreterPresse()));
        layout->addLayout(lBoutons);

        setLayout(layout);
    }


    //set connexion
    {
        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
        connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    }


    fenScreen = new screen;
    connect(fenScreen, SIGNAL(envoyerClick(QMouseEvent*)), this, SLOT(click(QMouseEvent*)));
    connect(fenScreen, SIGNAL(envoyerTouche(QKeyEvent*)), this, SLOT(clavier(QKeyEvent*)));

}

void fenetre::setEtat(QString msg)
{
    etat->setPlainText(msg);
    if(msg == "Connexion réussie !")
    {
        fenScreen->hide();
        fenScreen->resize(320, 256);
        fenScreen->show();

    }
    else
        fenScreen->hide();
}

void fenetre::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}

void fenetre::click(QMouseEvent *event)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer

    out << (quint32) 0;
    out << QString("click");
    int x=0;
    switch(event->button())
    {
    case Qt::LeftButton:
        x=1;
        break;
    case Qt::RightButton:
        x=2;
        break;
    case Qt::MidButton:
        x=3;
        break;
    }
    out << x;
    int y=0;
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
        y = 1;
        break;
    case QEvent::MouseButtonRelease:
        y = 0;
        break;
    }
    out << y;
    out.device()->seek(0);
    out << (quint32) (paquet.size() - sizeof(quint32));

    socket->write(paquet); // On envoie le paquet
}

void fenetre::clavier(QKeyEvent *event)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer

    out << (quint32) 0;
    out << QString("clavier");
    int x=0;
    x = event->key();
    out << x;
    int y=0;
    switch(event->modifiers())
    {
    case Qt::NoModifier:
        y=0;
        break;
    case Qt::ControlModifier:
        y=1;
        break;
    case Qt::ShiftModifier:
        y=2;
        break;
    case Qt::AltModifier:
        y=3;
        break;
    case Qt::MetaModifier:
        y=4;
        break;

    }
    out << y;

    out.device()->seek(0);
    out << (quint32) (paquet.size() - sizeof(quint32));

    socket->write(paquet); // On envoie le paquet
}




void fenetre::bLancerPresse()
{
    adresse = Tadresse->text();
    port = Sport->value();
    tailleMessage = 0;
    setEtat("Tentative de connexion en cours...");
    bLancer->setDisabled(true);
    socket->abort(); // On désactive les connexions précédentes s'il y en a
    socket->connectToHost(adresse, port); // On se connecte au serveur demandé
}

void fenetre::bArreterPresse()
{
    bLancer->setEnabled(true);
    bArreter->setDisabled(true);
    socket->abort();
    timerCoo->stop();
}



void fenetre::donneesRecues()
{
    QDataStream in(socket);

    while(true) //permet de recevoir plusieurs packets en meme temps
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
        QString messageRecu;
        in >> messageRecu;
        QPixmap screenRecu;
        in >> screenRecu;
        int xRecu;
        in >> xRecu;
        int yRecu;
        in >> yRecu;

        if(messageRecu == "screen")
        {
            fenScreen->setScreen(screenRecu, xRecu, yRecu);
            //std::cout << "recu : image" << std::endl;
            //mouvement();
            Fps++;
        }
        else
            std::cout << "erreur reception" << std::endl;

    }
}

// Ce slot est appelé lorsque la connexion au serveur a réussi
void fenetre::connecte()
{
    setEtat("Connexion réussie !");
    bLancer->setEnabled(true);
    bArreter->setEnabled(true);
    timerCoo->start();
}

// Ce slot est appelé lorsqu'on est déconnecté du serveur
void fenetre::deconnecte()
{
    setEtat("Déconnecté du serveur");
    bLancer->setEnabled(true);
    bArreter->setDisabled(true);
    lblFPS->setText("Fps : 0");
    timerCoo->stop();
}

// Ce slot est appelé lorsqu'il y a une erreur
void fenetre::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            setEtat("ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            setEtat("ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.");
            break;
        case QAbstractSocket::RemoteHostClosedError:
            setEtat("ERREUR : le serveur a coupé la connexion.");
            break;
        default:
            setEtat("ERREUR : " + socket->errorString() + "");
    }
    bLancer->setEnabled(true);
    bArreter->setDisabled(true);
}



void fenetre::envoyerMessage(QString msg, int x, int y)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer

    out << (quint32) 0;
    out << msg;
    out << x;
    out << y;
    out.device()->seek(0);
    out << (quint32) (paquet.size() - sizeof(quint32));

    socket->write(paquet); // On envoie le paquet
}


void fenetre::mouvement()
{

    int x, y;
        //x = cursor().pos().x();
        //y = cursor().pos().y();
        x = fenScreen->getMouse().x();
        y = fenScreen->getMouse().y();
        //std::cout << "envoyé : (" << x << "," << y << ")" << std::endl;
        envoyerMessage("mouseMoveEvent", x, y);
}


void fenetre::resetFps()
{
    lblFPS->setText("Fps : " + QString::number(Fps));
    Fps = 0;
}
