#include "fenetre.h"

fenetre::fenetre(QWidget *parent) : QMainWindow(parent)
{
    //chargement
    setAccessibleName("fenetre");
    client = new Client();

    //création
    QMainWindow::resize(1200, 750);

    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(5);
    plateau = new Plateau(this);
    splitter->addWidget(plateau);

    chat = new Chat(this);
    splitter->addWidget(chat);

    QMainWindow::setCentralWidget(splitter);

    connection = new WidgetConnection(client, this);

    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuServer = new QMenu("Serveur");
    menuServer->addAction("Connexion", connection, &QWidget::show);
    menuBar->addMenu(menuServer);
    QMainWindow::setMenuBar(menuBar);


    //organisation
    PacketManager::Message::handlerMessage = [this](Socket *socket, const ClientInfo *sender, const QString &message)
                                                { this->addChat(socket, sender, message); };
    connect(chat, &Chat::newMessage, this, &fenetre::sendMsg);
}

fenetre::~fenetre()
{
    if(plateau) delete plateau;
    plateau = nullptr;
    if(chat) delete chat;
    chat = nullptr;
    if(connection) delete connection;
    connection = nullptr;
    if(client) delete client;
    client = nullptr;
}



void fenetre::closeEvent(QCloseEvent *event)
{
    connection->hide();
    event->accept();
}

void fenetre::show()
{
    QMainWindow::show();
    plateau->actuPlateau();
    if(!connection->isConnected())
        connection->show();
}

void fenetre::addChat(const Socket *socket, const ClientInfo *sender, QString message)
{
    if(connection->getSocket() != socket)
        return;
    message.replace("@"+client->getPseudo(), "<span style=\"background:#FFC67C\">@"+client->getPseudo()+"</span>");
    chat->addMessage(sender, message);
}

void fenetre::sendMsg(QString msg)
{
    connection->sendMessage(msg);
}
