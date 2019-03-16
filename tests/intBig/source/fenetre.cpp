#include "fenetre.h"
#include "ui_fenetre.h"
#include "RSA.h"
#include "uintBig.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    setAccessibleName("fenetre");
    calcOn=false;
    fSave = new QFile;
    connect(ui->bCalcAll, &QPushButton::clicked, this, &fenetre::calcAll);
    connect(ui->bCalcOnly, &QPushButton::clicked, this, &fenetre::calcOnly);
    connect(ui->bParcourir, &QPushButton::clicked, this, &fenetre::parcourir);
}

fenetre::fenetre(const fenetre &fen)
{
    fSave = fen.fSave;
}//on a pas grand chose à faire, c'est juste pour le test

fenetre::~fenetre() {delete ui;}

void fenetre::closeEvent(QEvent *event) {fSave->close();}

void fenetre::calcAll()
{
    if(calcOn)
        return;
    calcOn=true;
    ui->bCalcAll->setDisabled(true);
    for(int i=1; i<=ui->sCalcOnly->maximum(); i++)
        calc(i);
    finCalc();
}

void fenetre::calcOnly()
{
    if(calcOn)
        return;
    calcOn=true;
    ui->bCalcOnly->setDisabled(true);
    calc(ui->sCalcOnly->value());
    finCalc();
}

void fenetre::calc(int nb)
{
    ouvrirFichier(nb);
    ui->te_resultat->clear();
    mapStats.clear();
    setCursor(Qt::WaitCursor);
    quint64 start=QDateTime::currentMSecsSinceEpoch();
    quint64 stop=start;
    uintBig r1;
    RSA rsa(this);
    switch (nb)
    {
        case 1://uintBig original
            r1=rsa.chiffrer1(uintBig(ui->intBig_M->text(), 10), uintBig(ui->intBig_D->text(), 10), uintBig(ui->intBig_N->text(), 10), ui->pb_chargement);
            stop=QDateTime::currentMSecsSinceEpoch();
            debug(ui->intBig_M->text()+"^"+ui->intBig_D->text()+"%"+ui->intBig_N->text()+" = "+r1.toString()+
                  " en "+QString::number(stop-start)+" msec", true);
            break;

        default:
            debug("Le calcul "+QString::number(nb)+" est inconnu.", true);
            break;
    }
    addStat(QString::number(nb), start, stop);

    ecrireStats();

    fermerFichier();
}

void fenetre::debug(QString str, bool important)
{
    str=QString::number(QDateTime::currentMSecsSinceEpoch())+" "+str;
    if(important)
    {
        if(ui->te_resultat->toPlainText().count("\n")>1000)//si il fait plus de 1 000 de retour à la lignes
            ui->te_resultat->clear();
        ui->te_resultat->append(str);
        qDebug(str.toStdString().c_str());
    }
    if(fSave->isOpen())
    {
        if(fSave->size()>10000000)//10Mo=10 000 000
        {
            QString fileName=fSave->fileName();
            if(fileName.contains(" (") && fileName.contains(")"))
            {
                int i=fileName.indexOf(" (");
                int i2=fileName.indexOf(").txt");
                fileName = fileName.left(i+2)+QString::number(fileName.mid(i+2, i2-i-2).toInt()+1)+fileName.mid(i2);
            }
            else
            {
                int i=fileName.indexOf(".txt");
                fSave->rename(fileName.left(i)+" (1)"+fileName.mid(i));
                fileName = fileName.left(i)+" (2)"+fileName.mid(i);
            }
            fSave->write(QString("Nouveau fichier : "+fileName).toStdString().c_str());
            fSave->close();
            fSave->setFileName(fileName);
            if(fSave->open(QFile::WriteOnly))
            {
                debug("fichier "+fileName+" ouvert", true);
            }
            else
                debug("Erreur: le fichier "+fileName+" ne peut pas etre ouvert", true);
        }
        fSave->write(QString(str+"\n").toStdString().c_str());
    }
}

void fenetre::addStat(QString type, quint64 start, quint64 stop)
{
    if(mapStats.contains(type))
    {
        mapStats[type][0] += (stop-start);//temps
        mapStats[type][1] += 1;//nb
        if((stop-start) < mapStats[type][2])
            mapStats[type][2] = (stop-start);//min
        if((stop-start) > mapStats[type][3])
            mapStats[type][3] = (stop-start);//max
    }
    else
    {
        mapStats[type].append(stop-start);
        mapStats[type].append(1);
        mapStats[type].append(stop-start);
        mapStats[type].append(stop-start);
    }
}

void fenetre::finCalc()
{
    ui->bCalcAll->setEnabled(true);
    ui->bCalcOnly->setEnabled(true);
    calcOn=false;
    setCursor(Qt::ArrowCursor);
}

void fenetre::ouvrirFichier(int i)
{
    fermerFichier();
    QString fileName=ui->le_saveFile->text()+"/"+QString::number(i)+"-"+QString::number(QDateTime::currentMSecsSinceEpoch())+".txt";
    fSave->setFileName(fileName);
    if(fSave->open(QFile::WriteOnly))
    {
        debug("fichier "+fileName+" ouvert", true);
    }
    else
        debug("Erreur: le fichier "+fileName+" ne peut pas etre ouvert", true);
}

void fenetre::fermerFichier()
{
    if(fSave->isOpen())
    {
        debug("fichier fermé", true);
        fSave->close();
    }
}

void fenetre::parcourir() {ui->le_saveFile->setText(QFileDialog::getExistingDirectory(this, "Ouvrir un dossier", QString(), QFileDialog::ShowDirsOnly));}

void fenetre::ecrireStats()
{
    debug("Statistiques :", true);
    QMapIterator<QString, QList<quint64>> i(mapStats);
    while(i.hasNext())
    {
        i.next();
        debug(i.key()+" à passé "+QString::number(i.value().at(0))+" msec pour "+QString::number(i.value().at(1))+" calculs. min:"+QString::number(i.value().at(2))+" max:"+QString::number(i.value().at(3)), true);
    }
}
