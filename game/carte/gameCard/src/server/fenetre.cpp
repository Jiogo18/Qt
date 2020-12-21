#include "fenetre.h"

fenetre::fenetre(QWidget *parent) : QMainWindow(parent)
{
    setAccessibleName("fenetre");
    //ui->tablePlayer->setModel(serveur->getTablePlayer());
    chat = new Chat(this);
    server = new Server(this);
    game = new GameServer(server, this);

    QTabWidget *layout = new QTabWidget(this);
    layout->addTab(server, tr("Server Configuration"));
    layout->addTab(game, tr("Clients and Players"));
    layout->addTab(chat, tr("Console"));
    setCentralWidget(layout);
    resize(800, 300);

    connect(chat, &Chat::newMessage, this, &fenetre::onMessageFromChat);
    /*QObject::connect(serveur, SIGNAL(newEtat(QString)), ui->lEtat, SLOT(setText(QString)));
    QObject::connect(serveur, SIGNAL(newCmd(messageSocket,QString)), this, SLOT(newCmd(messageSocket, QString)));*/
    show();
    server->createServ();//tentative de connexion
}

fenetre::~fenetre()
{
    if(server) { delete server; server = nullptr; }
    if(game) { delete game; game = nullptr; }
    if(chat) { delete chat; chat = nullptr; }
}

void fenetre::onMessageFromChat(QString line)
{
    server->onConsoleReturned(line);
}

