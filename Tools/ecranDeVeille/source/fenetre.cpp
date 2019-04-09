#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    setWindowIcon(QIcon(":/images/preferences-desktop-screensaver-2.png"));
    setWindowTitle("Ecran de veille");
    toastI = new toastIcon(this);

    ui->setupUi(this);

    connect(ui->lFichier, &QLineEdit::editingFinished, this, &fenetre::newFileName);
    connect(ui->bLaunch, &QPushButton::clicked, this, &fenetre::bAfficher);
    connect(ui->bActuVideo, &QPushButton::clicked, this, &fenetre::newFileName);

    tActu = new QTimer;
    tActu->start(500);
    actuTempsMouse();
    tActuPict = new QTimer;
    tActuPict->start(10000);
    actuTempsScreenshot();
    connect(tActu, &QTimer::timeout, this, &fenetre::afficher);
    connect(tActuPict, &QTimer::timeout, this, &fenetre::calcChangementsPict);
    connect(ui->sTMouse, &QSpinBox::editingFinished, this, &fenetre::actuTempsMouse);
    connect(ui->sTScreenshot, &QSpinBox::editingFinished, this, &fenetre::actuTempsScreenshot);
    dernierActu=0;
    lastCapture=screenshot();

    uploadFileConfig();
    viewVideo = new videoWidget();
    connect(viewVideo, &videoWidget::noAFK, this, &fenetre::newChangement);
    connect(viewVideo, &videoWidget::videoNumberChanged, this, &fenetre::lblVideoNbChange);
    newFileName();//securité
}

fenetre::~fenetre() {actuFileConfig(); delete ui;}

void fenetre::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (isVisible()) {
        hide();
        event->ignore();
    }
}


void fenetre::newFileName()
{
    ui->bLaunch->setDisabled(true);
    viewVideo->setVideo(ui->lFichier->text());
    //le bouton sera réactivé si il y a des vidéos ( lblVideoNbChange() )
}

void fenetre::afficher()
{
    calcChangementsMouse();
    qint64 tMSecIna=ui->tInactivite->value()*1000;
    qint64 tMSecIna2=QDateTime::currentMSecsSinceEpoch() - dernierActu;
    ui->lcdNBTime->display((int)(tMSecIna-tMSecIna2)/1000);
    if(tMSecIna-tMSecIna2 < 0)
        ui->lcdNBTime->display(0);//pas besoin de négatifs
    if(tMSecIna2 >= tMSecIna)
    {//si on a passé le tps, on peut afficher
        if(!viewVideo->isVisible())
            viewVideo->show();//si c'est pas encore affiché...
        if(viewVideo->isVisible())//si ça c'est affiché
            tActuPict->stop();//on en a plus besoin
    }
}

void fenetre::bAfficher()
{
    ui->bLaunch->setDisabled(true);
    viewVideo->show();
    ui->bLaunch->setEnabled(true);
    ui->bLaunch->setDown(false);
}

void fenetre::calcChangementsMouse()
{
    const int vari=ui->sTxMouse->value();
    if(lastCursor != cursor().pos())
    {
        if(!(cursor().pos().x()-vari < lastCursor.x() && lastCursor.x() < cursor().pos().x()+vari &&
                cursor().pos().y()-vari < lastCursor.y() && lastCursor.y() < cursor().pos().y()+vari))
        {//legere variation du curseur
            newChangement();
            lastCursor=cursor().pos();
        }
    }
}
void fenetre::calcChangementsPict()
{
    QImage imgCapture=screenshot();
    //test de la différence des 2 imgs
    if(difference(lastCapture,imgCapture) > ui->sTxScreenshot->value()/100.0)
    {//0.02 est le min si une video est lancée
        lastCapture=imgCapture;
        dernierActu=QDateTime::currentMSecsSinceEpoch();
        //on dit pas les nouveaux changements, pour quand on est en mode affichage
    }
}

void fenetre::newChangement()
{
    dernierActu=QDateTime::currentMSecsSinceEpoch();
    viewVideo->hide();
    if(!viewVideo->isVisible())//caché:
        if(!tActuPict->isActive())//relance le timer de test
            tActuPict->start();
}

double fenetre::difference(QImage img1, QImage img2)
{
    int nbPixsDiff=0;
    int width=img1.width();
    if(img2.width()<width)
        width=img2.width();
    int height=img1.height();
    if(img2.height()<height)
        height=img2.height();
    for(int ix=0; ix<width; ix++)
        for(int iy=0; iy<height; iy++)
            if(img1.pixelColor(ix,iy)!=img2.pixelColor(ix,iy))
                nbPixsDiff++;
    return (double)nbPixsDiff / (img2.width()*img2.height());
}

QImage fenetre::screenshot()
{return QGuiApplication::primaryScreen()
            ->grabWindow(0).toImage();}

void fenetre::actuFileConfig()
{
    QFile *file = new QFile("ecranDeVeille.txt");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    ecrireFichier(file, "file:"+ui->lFichier->text()+"\n");
    ecrireFichier(file, "time:"+QString::number(ui->tInactivite->value())+"\n");
    ecrireFichier(file, "txMouse:"+QString::number(ui->sTxMouse->value())+"\n");
    ecrireFichier(file, "txScreenshot:"+QString::number(ui->sTxScreenshot->value())+"\n");
    ecrireFichier(file, "showStart:"+QString(ui->checkStartShow->isChecked() ? "true":"false")+"\n");
    ecrireFichier(file, "tempMouse:"+QString::number(ui->sTMouse->value())+"\n");
    ecrireFichier(file, "tempScreenshot:"+QString::number(ui->sTScreenshot->value())+"\n");

    file->close();
    delete file;
}
void fenetre::uploadFileConfig()
{
    QFile file("ecranDeVeille.txt");
    if(file.exists())//si il existe pas, show()
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString arg=line.mid(line.indexOf(":")+1);
            arg = arg.left(arg.size()-1);//espace de la fin
            if(line.startsWith("file"))
                ui->lFichier->setText(arg);
            if(line.startsWith("time"))
                ui->tInactivite->setValue(arg.toInt());
            if(line.startsWith("txMouse"))
                ui->sTxMouse->setValue(arg.toDouble());
            if(line.startsWith("txScreenshot"))
                ui->sTxScreenshot->setValue(arg.toDouble());
            if(line.startsWith("showStart"))
                ui->checkStartShow->setChecked(arg!="false");//true par défault
            if(line.startsWith("tempMouse"))
                ui->sTMouse->setValue(arg.toInt());
            if(line.startsWith("tempScreenshot"))
                ui->sTScreenshot->setValue(arg.toInt());
        }
        if(ui->checkStartShow->isChecked())
            show();
    }
    else
        show();
    file.close();
    actuFileConfig();
}

void fenetre::lblVideoNbChange(quint64 nbVideo)
{
    QString style;
    if(nbVideo == 0)
    {
        style="QLabel { color : red; }";
        ui->bLaunch->setDisabled(true);
    }
    else
    {
        style="QLabel { color : black; }";
        ui->bLaunch->setEnabled(true);
    }
    ui->lblVideoNb->setText(QString::number(nbVideo));
    ui->lblVideoNb->setStyleSheet(style);
}

void fenetre::ecrireFichier(QFile *file, QString str)
{file->write(str.toStdString().c_str());}

void fenetre::actuTempsMouse()
{
    tActu->stop();
    tActu->setInterval(ui->sTMouse->value());
    tActu->start();
}
void fenetre::actuTempsScreenshot()
{
    tActuPict->stop();
    tActuPict->setInterval(ui->sTScreenshot->value()*1000);
    tActuPict->start();
}
