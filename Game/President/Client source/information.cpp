#include "information.h"

information::information()
{
    setupUi(this);
    QFont *fontVersion = new QFont;
    fontVersion->setPixelSize(15);
    TxtVersion->setFont(*fontVersion);
    versions->addItem("1.3");
    connect(versions, SIGNAL(currentIndexChanged(int)), this, SLOT(actuVersion()));
    actuVersion();
}

void information::actuVersion()
{
    QString txt="";

    QFile file("changelog.txt");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        QTextStream in(&file);
        while (!in.atEnd())
        {
            if(in.readLine()==QString("#Version "+versions->currentText()+":"))
                break;
        }
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if(line.startsWith("#Version "))//on est a la fin du changelog de la version
                break;
            if(line.startsWith("#") && txt!=QString(""))
                txt+="\n";
            txt += line+"\n";
        }
        if(txt==QString(""))
            txt="La version n'a pas été trouvée.";
    }
    else
        txt="Le fichier de changelog n'a pas été trouvé ("+versions->currentText()+")";

    file.close();


    if(txt==QString(""))
        txt="Une erreur est survenue, impossible de trouver/lire le changelog.";

    TxtVersion->setText(txt);
}

