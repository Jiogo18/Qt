#include "parametre.h"
#include "ui_parametre.h"

parametre::parametre()
{
    setupUi(this);
    QFile file("options.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString fichier = file.readAll();

        checkErreur->setChecked(getValeurFichier(&fichier, "chatErreur")==QString("true"));
        checkCarteOuRien->setChecked(getValeurFichier(&fichier, "chatCarteOuRien")==QString("true"));
        checkCartes->setChecked(getValeurFichier(&fichier, "chatMilieu")==QString("true"));
        checkJoueur->setChecked(getValeurFichier(&fichier, "chatJoueur")==QString("true"));
        checkPass->setChecked(getValeurFichier(&fichier, "chatPass")==QString("true"));
        checkReset->setChecked(getValeurFichier(&fichier, "chatReset")==QString("true"));
        checkUltimeCarte->setChecked(getValeurFichier(&fichier, "chatUltimeCarte")==QString("true"));
        volumeStart->setValue(getValeurFichier(&fichier, "sonVolumeStart").toInt());
        volumeNext->setValue(getValeurFichier(&fichier, "sonVolumeCarte").toInt());
        volumeWin->setValue(getValeurFichier(&fichier, "sonVolumeWin").toInt());
        volumeChat->setValue(getValeurFichier(&fichier, "sonVolumeChat").toInt());
        volumeAlerte->setValue(getValeurFichier(&fichier, "sonVolumeAlerte").toInt());
        tempAlerte->setValue(getValeurFichier(&fichier, "sonTempAlerte").toInt());
    }
    else
        emit addErreur("Erreur options", "Le fichier de paramètres \"options.txt\" est introuvable, les options sont mises a zéro");

    file.close();

    connect(checkCarteOuRien, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(checkCartes, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(checkJoueur, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(checkPass, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(checkReset, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(checkUltimeCarte, SIGNAL(stateChanged(int)), this, SLOT(updateFichier()));
    connect(volumeStart, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
    connect(volumeNext, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
    connect(volumeWin, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
    connect(volumeChat, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
    connect(volumeAlerte, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
    connect(tempAlerte, SIGNAL(valueChanged(int)), this, SLOT(updateFichier()));
}

bool parametre::getChatErreur()
{
    return checkErreur->isChecked();
}

bool parametre::getChatCarteOuRien()
{
    return checkCarteOuRien->isChecked();
}

bool parametre::getChatCartes()
{
    return checkCartes->isChecked();
}
bool parametre::getChatJoueur()
{
    return checkJoueur->isChecked();
}

bool parametre::getChatPass()
{
    return checkPass->isChecked();
}

bool parametre::getChatReset()
{
    return checkReset->isChecked();
}

bool parametre::getChatUltimeCarte()
{
    return checkUltimeCarte->isChecked();
}

qreal parametre::getSonVolumeStart()
{
    return volumeStart->value();
}
qreal parametre::getSonVolumeNext()
{
    return volumeNext->value();
}
qreal parametre::getSonVolumeWin()
{
    return volumeWin->value();
}
qreal parametre::getSonVolumeChat()
{
    return volumeChat->value();
}
qreal parametre::getSonVolumeAlerte()
{
    return volumeAlerte->value();
}
int parametre::getSonIntervalAlerte()
{
    return tempAlerte->value();
}

void parametre::updateFichier()
{
    QFile file("options.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "chatErreur=" << getStrValeur(0, checkErreur->isChecked(), 0) << "\n";
        out << "chatCarteOuRien=" << getStrValeur(0, checkCarteOuRien->isChecked(), 0) << "\n";
        out << "chatMilieu=" << getStrValeur(0, checkCartes->isChecked(), 0) << "\n";
        out << "chatJoueur=" << getStrValeur(0, checkJoueur->isChecked(), 0) << "\n";
        out << "chatPass=" << getStrValeur(0, checkPass->isChecked(), 0) << "\n";
        out << "chatReset=" << getStrValeur(0, checkReset->isChecked(), 0) << "\n";
        out << "chatUltimeCarte=" << getStrValeur(0, checkUltimeCarte->isChecked(), 0) << "\n";
        out << "sonVolumeStart=" << getStrValeur(1, false, volumeStart->value()) << "\n";
        out << "sonVolumeCarte=" << getStrValeur(1, false, volumeNext->value()) << "\n";
        out << "sonVolumeWin=" << getStrValeur(1, false, volumeWin->value()) << "\n";
        out << "sonVolumeChat=" << getStrValeur(1, false, volumeChat->value()) << "\n";
        out << "sonVolumeAlerte=" << getStrValeur(1, false, volumeAlerte->value()) << "\n";
        out << "sonTempAlerte=" << getStrValeur(1, false, tempAlerte->value()) << "\n";
        file.close();
    }
    else
    {
        emit addErreur("Erreur options", "Le fichier de paramètres \"options.txt\" est introuvable pour l'enregistrement.");
    }

}

QString parametre::getValeurFichier(QString *fichier, QString variable)
{
    QTextStream in(fichier);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith(variable+"="))
        {
            return line.remove(variable+"=");//ne retourne que la valeur
        }
    }
    return "0";
}

QString parametre::getStrValeur(int type, bool activate, int value)
{
    switch (type)
    {
        case 0:
            if(activate)
                return "true";
            else
                return "false";

        case 1:
            return QString::number(value);

        default:
            return "";
    }
    return "";
}
