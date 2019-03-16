#include "fenetre.h"

fenetre::fenetre()
{
    setupUi(this);
    execution=false;
    connect(bParcourir, SIGNAL(clicked(bool)), this, SLOT(parcourir()));
    connect(bCalc, SIGNAL(clicked(bool)), this, SLOT(calculer()));
    connect(bStop, SIGNAL(clicked(bool)), this, SLOT(stop()));
    connect(nbCharsMin, SIGNAL(editingFinished()), this, SLOT(actuNbChrs()));
    connect(nbCharsMax, SIGNAL(editingFinished()), this, SLOT(actuNbChrs()));

    //connect(this, SIGNAL(analyse(QString)), this, SLOT(testMot(QString)));
    mots = new QStringList;
}

void fenetre::testMot(QString mot)
{
    if(mot.size() < nbCharsMin->value() || nbCharsMax->value() < mot.size())
        return;//le mot a la bonne taille

    if(!motsComposes->isChecked() && mot.contains("-"))
        return;//le mot est composé ou non

    for(int i=0; i<mot.size() && i<charsDef->text().size(); i++)//le mot correspond au squelet donné
    {
        if(charsDef->text().toUpper()[i] != mot[i] && charsDef->text().toUpper()[i] != '*')
            return;//qDebug(QString(mot+" : contient un mauvais char en "+QString::number(i)).toStdString().c_str());
    }


    if(!lContient->text().remove("*").isEmpty())//si on demande les lettres precises(presentes/non)
    {
        //qDebug(QString(mot+" : lContientVide = false").toStdString().c_str());
        if(bContient->currentIndex() == 0 || bContient->currentIndex() == 1)//untiquement les lettres precises (remettre ds l'ordre) ou peut etre uniquement(des  lettres peuvent ne pas etre utilisées)
        {
            QString tempContient=lContient->text().toUpper();
            for(int i=0; i<mot.size(); i++)
            {
                if(tempContient.contains(mot[i]))
                {
                    if(!plusieursChars->isChecked())
                        tempContient.remove(tempContient.indexOf(mot[i]),1);
                }
                else
                    return;
            }
            if(bContient->currentIndex()==0 && !tempContient.isEmpty() && !plusieursChars->isChecked())//il faut que tt soit utilisé
                return;
        }
        else if(bContient->currentIndex() == 2)//ne contient pas
        {
            QString tempContient=lContient->text().toUpper();
            for(int i=0; i<mot.size(); i++)
                if(tempContient.contains(mot[i]))
                    return;
        }
    }

    resultat->append(mot);
}

void fenetre::parcourir()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Fichier de recherche");
    if(!fileName.isEmpty() && QFile::exists(fileName))
    {
        fichier->setText(fileName);
    }
    else
        QMessageBox::warning(this, "Fichier de recherche invalide", "Le fichier selectionné est invalide, veuillez réessayer.");
}

void fenetre::calculer()
{
    if(execution)
        return;
    if(fichier->text().isEmpty())
    {
        QMessageBox::warning(this, "Recherche", "Le fichier selectionné est invalide, veuillez réessayer.");
        return;
    }
    QFile file(fichier->text());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Recherche", "Le fichier selectionné est invalide, veuillez réessayer.");
        return;
    }
    execution=true;
    QTextStream in(&file);
    pbChargement->setMaximum(file.size());
    pbChargement->setValue(0);
    QString line = in.readLine();
    resultat->clear();
    mots->clear();

    QTime timer;
    timer=QTime::currentTime();
    while (!line.isNull() && execution)//chargement elements
    {
        mots->append(line.toUpper());
        line = in.readLine();
        if(timer.addMSecs(100) < QTime::currentTime())//tout les 100ms, actu de la fenetre
        {
            timer=QTime::currentTime();
            pbChargement->setValue(in.pos());
            pbChargement->setFormat("%p% ("+QString::number(mots->size())+" éléments chargés)");
            QCoreApplication::processEvents();
        }
    }
    file.close();

    pbChargement->setMaximum(mots->size());
    for(int i=0; i<mots->size(); i++)//analyse des elements
    {
        if(!execution)
            break;
        testMot(mots->at(i));
        if(timer.addMSecs(100) < QTime::currentTime())//tout les 100ms, actu de la fenetre
        {
            timer=QTime::currentTime();
            pbChargement->setValue(i);
            pbChargement->setFormat("%p% ("+QString::number(i)+" éléments analysés)");
            QCoreApplication::processEvents();
        }
    }
    mots->clear();

    execution=false;
    pbChargement->setValue(pbChargement->maximum());
}

void fenetre::stop()
{
    execution=false;
}

void fenetre::actuNbChrs()
{
    nbCharsMin->setMaximum(nbCharsMax->value());
    while(charsDef->text().size() < nbCharsMin->value())
        charsDef->setText(charsDef->text()+"*");
    while(charsDef->text().size() > nbCharsMax->value())
        charsDef->setText(charsDef->text().remove(charsDef->text().size()-1, 1));
}
