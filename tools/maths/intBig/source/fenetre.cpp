#include "fenetre.h"
#include "ui_fenetre.h"
#include "RSA.h"
#include "intBig4.h"
#include "intBig5.h"
#include "intBig6.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    setAccessibleName("fenetre");
    calcOn=false;
    fSave = new QFile;
    fStats = new QFile;
    connect(ui->bCalcAll, &QPushButton::clicked, this, &fenetre::calcAll);
    connect(ui->bCalcOnly, &QPushButton::clicked, this, &fenetre::calcOnly);
    connect(ui->bParcourir, &QPushButton::clicked, this, &fenetre::parcourir);
    connect(ui->bIConvertir, &QPushButton::clicked, this, &fenetre::calcInfo1);
    connect(ui->lineConsole, &QLineEdit::editingFinished, this, &fenetre::calcConsole);
}

fenetre::fenetre(const fenetre &fen) : QWidget()
{
    fSave = fen.fSave;
}//on a pas grand chose à faire, c'est juste pour le test

fenetre::~fenetre() {delete ui;}

void fenetre::closeEvent(QEvent *event)
{
    fSave->close();
    fStats->close();
    event->accept();
    qApp->quit();
}

void fenetre::calcAll()
{
    if(calcOn)
        return;
    calcOn=true;
    ui->bCalcAll->setDisabled(true);
    fStats->close();
    fStats->setFileName(ui->le_saveFile->text()+"/Stats.txt");
    fStats->open(QFile::WriteOnly);

    for(int i=1; i<=ui->sCalcOnly->maximum(); i++)
    {
        calc(i);
    }
    finCalc();
}

void fenetre::calcOnly()
{
    if(calcOn)
        return;
    calcOn=true;
    ui->bCalcOnly->setDisabled(true);
    fStats->close();
    fStats->setFileName(ui->le_saveFile->text()+"/Stats.txt");
    fStats->open(QFile::WriteOnly);

    calc(ui->sCalcOnly->value());
    finCalc();
}

void fenetre::calc(int nb)
{
    fStats->write(QString(QString::number(nb)+":\n").toStdString().c_str());
    ouvrirFichier(nb);
    ui->te_resultat->clear();
    ui->te_resultat->setText(QString::number(nb)+"/"+QString::number(ui->sCalcOnly->maximum()));
    mapStats.clear();
    setCursor(Qt::WaitCursor);
    quint64 start=QDateTime::currentMSecsSinceEpoch();
    quint64 stop=start;
    RSA rsa(this);

    intBig4 resultat4;
    intBig5 resultat5;
    intBig6 resultat6;
    switch (nb)
    {

    case 1://intBig4
        resultat4=rsa.chiffrer4(intBig4(ui->intBig_M->text(), 10), intBig4(ui->intBig_D->text(), 10), intBig4(ui->intBig_N->text(), 10), ui->pb_chargement);
        stop=QDateTime::currentMSecsSinceEpoch();
        debug(QString::number(nb)+": "+ui->intBig_M->text()+"^"+ui->intBig_D->text()+"%"+ui->intBig_N->text()+" = "+resultat4.toString()+
              " en "+QString::number(stop-start)+" msec", true);
        break;

    case 2://intBig5
        resultat5=rsa.chiffrer5(intBig5(ui->intBig_M->text(), 10), intBig5(ui->intBig_D->text(), 10), intBig5(ui->intBig_N->text(), 10), ui->pb_chargement);
        stop=QDateTime::currentMSecsSinceEpoch();
        debug(QString::number(nb)+": "+ui->intBig_M->text()+"^"+ui->intBig_D->text()+"%"+ui->intBig_N->text()+" = "+resultat5.toString()+
              " en "+QString::number(stop-start)+" msec", true);
        break;

    case 3://intBig6
        resultat6=rsa.chiffrer6(intBig6B(ui->intBig_M->text(), 10), intBig6B(ui->intBig_D->text(), 10), intBig6B(ui->intBig_N->text(), 10), ui->pb_chargement);
        stop=QDateTime::currentMSecsSinceEpoch();
        debug(QString::number(nb)+": "+ui->intBig_M->text()+"^"+ui->intBig_D->text()+"%"+ui->intBig_N->text()+" = "+intBig6B(resultat6).toString(10)+
              " en "+QString::number(stop-start)+" msec", true);
        break;

    default:
        debug("Le calcul "+QString::number(nb)+" est inconnu.", true);
        break;
    }
    addStat(QString::number(nb), start, stop);

    ecrireStats();

    fermerFichier();
    fStats->write("\n\n\n");
}

void fenetre::debug(QString str, bool important)
{
    str=QString::number(QDateTime::currentMSecsSinceEpoch())+" "+str;
    if(important)
    {
        if(ui->te_resultat->toPlainText().count("\n")>1000)//si il fait plus de 1 000 de retour à la lignes
            ui->te_resultat->clear();
        ui->te_resultat->append(str);
        qDebug() << str;
    }
    if(fSave->isOpen() && (important||false))//pas tout noter quand on veut optimiser
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
        //perds 300msec sur 900 quand on le désactive
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
    fStats->close();
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
    debug("\n\n\nStatistiques :", true);
    QMapIterator<QString, QList<quint64>> i(mapStats);
    while(i.hasNext())
    {
        i.next();
        //if(i.key().startsWith("chiffrement"))
            debug(i.key()+" à passé "+QString::number(i.value().at(0))+" msec pour "+QString::number(i.value().at(1))+" calculs. min:"+QString::number(i.value().at(2))+" max:"+QString::number(i.value().at(3)), true);
        if(fStats->isOpen())
            fStats->write(QString(i.key()+" à passé "+QString::number(i.value().at(0))+" msec pour "+QString::number(i.value().at(1))+" calculs. min:"+QString::number(i.value().at(2))+" max:"+QString::number(i.value().at(3))+"\n").toStdString().c_str());
    }
}

void fenetre::calcInfo1()
{
    quint64 b1=ui->sIBase1->value();
    quint64 b2=ui->sIBase2->value();
    if(true) {
        ui->intBig_I2->setText(intBig6B(ui->intBig_I1->text(), b1).toString(b2));
    }
    /*wait what appened? ui->cICalcSeparation existe pas?
    else if(ui->cICalcSeparation->isChecked())//faire comme un tableau
    {
        intBig4Parent retour(b1,0);
        if(ui->intBig_I1->text().startsWith("-"))
        {
            retour.setNegative(true);
            ui->intBig_I1->text().remove(0,1);
        }
        QList<QString> str1;
        if(b1>10 || ui->intBig_I1->text().contains("|"))
        {
            str1=ui->intBig_I1->text().split("|");
        }
        else//si il y en a pas, on fait l'autre méthode
            str1=QStringList(ui->intBig_I1->text());

        for(int i=0; i<str1.size(); i++)
        {
            retour[str1.size()-i-1] += str1.at(i).toULongLong();
            if(retour[str1.size()-i-1] >= retour.base())
                ui->te_resultat->setText("La valeur en "+QString::number(i)+" est trop grande :"+str1.at(i));
        }
        retour.resize();
        QString retour2="";
        if(retour.isNegative())
            retour2="-";
        retour.setBase(b2);
        for(quint64 i=0; i<retour.sizeNNul(); i++)
        {
            retour2 = QString::number(retour.at(i))+retour2;
            if(i+1 < retour.sizeNNul() && b2>10)
                retour2 = "|"+retour2;
        }
    }*/
    else
    {
        ui->intBig_I2->setText(intBig4Parent(ui->intBig_I1->text(), b1).toString(b2));
    }
}

void fenetre::calcConsole()
{
    QString command = ui->lineConsole->text();
    ui->lineConsole->setText("");
    console.addCmd(ui->textConsole, command);
}
