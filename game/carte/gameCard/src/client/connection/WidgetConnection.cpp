#include "WidgetConnection.h"

WidgetConnection::WidgetConnection(Client *client, QWidget *parent) : QDialog(parent)
{
    this->client = client;
    servPass = new Password();
    socket = new Socket(servPass, this);
    connect(socket, &Socket::error, this, &WidgetConnection::onError);
    connect(socket, &Socket::connected, this, &WidgetConnection::onConnect);
    connect(socket, &Socket::disconnected, this, &WidgetConnection::onDisconnect);


    setWindowTitle(tr("Connection to a server", "dialog box title"));

    ip = new QLineEdit("127.0.0.1");
    ip->setPlaceholderText(tr("IP"));
    port = new QSpinBox;
    port->setRange(0, 65535);
    port->setValue(25567);
    servPassword = new QLineEdit;
    servPassword->setPlaceholderText(tr("Server Password"));
    servPassword->setEchoMode(QLineEdit::Password);
    QFormLayout *grpHost = new QFormLayout;
    grpHost->addRow("Host", ip);
    grpHost->addRow("Port", port);
    grpHost->addWidget(servPassword);

    pseudo = new QLineEdit;
    pseudo->setPlaceholderText(tr("Login"));
    password = new QLineEdit;
    password->setPlaceholderText(tr("Password"));
    password->setEchoMode(QLineEdit::Password);
    QFormLayout *grpClient = new QFormLayout;
    grpClient->addRow("Pseudo", pseudo);
    grpClient->addWidget(password);

    bConnect = new QPushButton(tr("Connect"));
    bInfo = new QPushButton(tr("Info"));
    QHBoxLayout *grpAction = new QHBoxLayout;
    grpAction->addWidget(bConnect);
    grpAction->addWidget(bInfo);
    connect(bConnect, &QPushButton::clicked, this, &WidgetConnection::onBConnect);
    connect(bInfo, &QPushButton::clicked, this, &WidgetConnection::showInfo);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(10);
    layout->addLayout(grpHost);
    layout->addSpacing(20);
    layout->addLayout(grpClient);
    layout->addSpacing(20);
    layout->addLayout(grpAction);
    resize(340, 0);
}

WidgetConnection::~WidgetConnection()
{
    if(socket) { disconnectFromHost(); delete socket; socket = nullptr; }

    if(ip) { delete ip; ip = nullptr; }
    if(port) { delete port; port = nullptr; }
    if(servPass) { delete servPass; servPass = nullptr; }
    if(pseudo) { delete pseudo; pseudo = nullptr; }
    if(password) { delete password; password = nullptr; }
    if(bConnect) { delete bConnect; bConnect = nullptr; }
    if(bInfo) { delete bInfo; bInfo = nullptr; }
    if(servPass) { delete servPass; servPass = nullptr; }
}

void WidgetConnection::onError(QAbstractSocket::SocketError error)
{
    QMessageBox::StandardButton answer;
    switch(error) // Message différent selon l'erreur qu'on nous indique
    {
    case QAbstractSocket::HostNotFoundError:
        //"ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.";
        answer = QMessageBox::warning(this, tr("Socket Connection"),
                                      tr("ERROR: ") + tr("can't reach the server.", "HostNotFoundError") + "\n" +
                                      tr("Check the IP and port."),
                                      QMessageBox::Retry | QMessageBox::Cancel,
                                      QMessageBox::Retry);
        break;

    case QAbstractSocket::ConnectionRefusedError:
        //"ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.";
        answer = QMessageBox::warning(this, tr("Socket Connection"),
                                      tr("ERROR: ") + tr("the server refused the connection.", "ConnectionRefusedError") + "\n" +
                                      tr("Check the IP and port.") + " " + tr("Also check if it has been started.", "is server started?"),
                                      QMessageBox::Retry | QMessageBox::Cancel,
                                      QMessageBox::Retry);
        break;

    case QAbstractSocket::RemoteHostClosedError:
        //"ERREUR : le serveur a coupé la connexion.";
        answer = QMessageBox::warning(this, tr("Socket Connection"),
                                      tr("ERROR: ") + tr("The server has cut the connection.", "RemoteHostClosedError"),
                                      QMessageBox::Ok);
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug() << "Socket Connection ERROR:" << "Data remain in packet";
        answer = QMessageBox::NoButton;
        break;
    default:
        answer = QMessageBox::warning(this, tr("Socket Connection"),
                                      tr("ERROR: ")+socket->errorString(),
                                      QMessageBox::Retry | QMessageBox::Discard | QMessageBox::Cancel,
                                      QMessageBox::Cancel);
        break;
    }
    switch(answer) {
    case QMessageBox::Retry:
        connectToHost();
        break;
    case QMessageBox::Discard:
        disconnectFromHost();
        break;
    default:
        actuButtons();
        break;
    }
}

void WidgetConnection::onBConnect()
{
    if(socket->isDisconnected())
        connectToHost();
    else
        disconnectFromHost();
}

void WidgetConnection::connectToHost()
{
    if(!socket->isDisconnected()) {
        //n'arrive plus normalement
        int answer = 0;
        if(socket->peerName() == ip->text() && socket->peerPort() == port->value())
            answer = QMessageBox::warning(this, tr("Socket Connection"),
                                          tr("Your are already connected to this host. Do you want to reconnect to it?"),
                                          QMessageBox::Yes | QMessageBox::Cancel,
                                          QMessageBox::Yes);
        else
            answer = QMessageBox::warning(this, tr("Socket Connection"),
                                          tr("An other connection is already establied. Are you sure you want to join another server?"),
                                          QMessageBox::Yes | QMessageBox::Cancel,
                                          QMessageBox::Yes);
        if(answer != QMessageBox::Yes)
            return;
        socket->disconnectFromHost();
    }
    *(ClientAuth*)client = ClientAuth(pseudo->text());
    client->setPseudo(pseudo->text());
    client->setPassword(password->text());
    *servPass = servPassword->text();
    socket->connectToHost(ip->text(), port->value(), client, servPass);
    actuButtons();
}

void WidgetConnection::disconnectFromHost()
{
    socket->disconnectFromHost();
    actuButtons();
}

void WidgetConnection::showInfo() const
{
    QString state = "";
    switch (socket->getState()) {
        case QTcpSocket::UnconnectedState:
            state = tr("Disconnected");
            break;
        case QTcpSocket::ConnectedState: {
            switch (socket->getAuthState()) {
            case Socket::Authentified:
                state = tr("Connected to ") + socket->peerName() + ":" + QString::number(socket->peerPort()) + "\n" +
                        tr("Identified as ") + client->getPseudo();
                break;
            case Socket::Rejected:
                state = tr("Connected to ") + socket->peerName() + ":" + QString::number(socket->peerPort()) + "\n" +
                        tr("But you has been rejected");
                break;
            default:
                state = tr("Connected to ") + socket->peerName() + ":" + QString::number(socket->peerPort()) + "\n" +
                        tr("But your are not identified yet (step %n of 4)", "", socket->getAuthState());
                break;
            }
            break;
        }
        case QTcpSocket::ConnectingState:
            state = tr("Connecting to ") + socket->peerName() + ":" + QString::number(socket->peerPort()) + "\n" +
                    tr("The server hasn't answer yet");
            break;
        default:
            state = tr("Unkow state: %n", "", socket->getState());
        break;
    }
    QMessageBox::information((QWidget*)this, tr("Connection Info"),
                state + "\n" +
                "version " + QString::number(socket->version));
}

void WidgetConnection::onConnect()
{ actuButtons(); }
void WidgetConnection::onDisconnect()
{ actuButtons(); }

void WidgetConnection::actuButtons()
{
    if(socket->isConnected() || socket->isDisconnected()) {
        setCursor(Qt::ArrowCursor);
        //bConnect->setEnabled(true);
    }
    else {
        setCursor(Qt::WaitCursor);
        //bConnect->setDisabled(true);
    }
    switch (socket->getState()) {
    case QTcpSocket::UnconnectedState:
        bConnect->setText(tr("Connect"));
        break;

    case QTcpSocket::ConnectedState:
        switch (socket->getAuthState()) {
        case Socket::Authentified:
            bConnect->setText(tr("Disconnect"));
            break;
        case Socket::Rejected:
            bConnect->setText(tr("Rejected..."));
            break;
        default:
            bConnect->setText(tr("Waiting answer..."));
            break;
        }
        break;

    default:
        bConnect->setText(tr("Other... (%n)", "", socket->getState()));
    }
    if(socket->isDisconnected()) {
        ip->setEnabled(true);
        port->setEnabled(true);
        servPassword->setEnabled(true);
        pseudo->setEnabled(true);
        password->setEnabled(true);
    }
    else {
        ip->setDisabled(true);
        port->setDisabled(true);
        servPassword->setDisabled(true);
        pseudo->setDisabled(true);
        password->setDisabled(true);
    }
}

