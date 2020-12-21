#include "fenetre.h"

fenetre::fenetre()
{
    QVBoxLayout *layout = new QVBoxLayout;

    lblFichiers = new QLabel("Fichiers président à update :");
    layout->addWidget(lblFichiers);

    releaseClient = new QCheckBox("Client");
    layout->addWidget(releaseClient);
    releaseClient->setChecked(true);

    releaseServeur = new QCheckBox("Serveur");
    layout->addWidget(releaseServeur);
    releaseServeur->setChecked(true);

    winrar = new QCheckBox("Winrar");
    layout->addWidget(winrar);
    winrar->setChecked(false);

    bLancer = new QPushButton("Lancer l'update");
    layout->addWidget(bLancer);
    connect(bLancer, SIGNAL(clicked(bool)), this, SLOT(lancer()));

    setLayout(layout);
}

int fenetre::erreur(QString fichier)
{
    return QMessageBox::question(this, "Erreur update de fichier président", "Le fichier "+fichier+" n'a pas été chargé.", "Recommencer", "Annuler");
}


void fenetre::copier(QString fileIn, QString fileOut, QString fichier, QString updateFichier)
{
    bool recommencer;
    do
    {
        if(QFile::exists(fileOut))
            recommencer = !QFile::remove(fileOut);
        if(!recommencer)//pas besoin de redemender
            recommencer = !QFile::copy(fileIn, fileOut);//si ça c mal passé, on recommence (peut etre)
        if(!recommencer)
            updateMsgFichier(updateFichier);
        if(recommencer)//si ça c mal passé
            recommencer = erreur(fichier)==0;

    } while(recommencer);
}


void fenetre::updateMsgFichier(QString updateFichier)
{
    int version=0;
    QFile file(updateFichier);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if(line.startsWith("version d'update:"))//fait 17 de long
            {
                version=line.mid(17).toInt();
                break;//pas besoin de continuer
            }
        }
        file.close();
    }
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(QString("date d'update:"+QDateTime::currentDateTime().toString()+"\n").toStdString().c_str());
        file.write(QString("version d'update:"+QString::number(version+1)).toStdString().c_str());
        file.close();
    }
}

void fenetre::touche(int touche, int etat)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = touche;
    ip.ki.wScan = MapVirtualKeyEx(touche, 0, (HKL)0xf0010413);;

    if(etat == 1 || etat == 0)
    {
        ip.ki.dwFlags = 0; //key press
        SendInput(1, &ip, sizeof(INPUT));
    }
    if(etat == -1 || etat == 0)
    {
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
    }
}

void fenetre::ecrire(QString texte)
{
    for(int i=0; i<texte.size(); i++)
    {
        if(QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890°+¨£%µ?./§>").contains(texte[i]))
            touche(VK_LSHIFT,1);
        if(QString("~#{[|`\\^@]}¤€").contains(texte[i]))
            touche(VK_RMENU,1);//alt gr

        touche(VkKeyScan(texte[i].toLatin1()));

        if(QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890°+¨£%µ?./§>").contains(texte[i]))
            touche(VK_LSHIFT,-1);
        if(QString("~#{[|`\\^@]}¤€").contains(texte[i]))
            touche(VK_RMENU,-1);//alt gr
    }
}


void fenetre::lancer()
{
    if(releaseClient->isChecked())
    {
        copier("C:/Qt/MesProg/cartes/president/build-client-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/client.exe", "C:/Qt/MesProg/cartes/president/Final 2/client/client.exe", "Client", "C:/Qt/MesProg/cartes/president/Final 2/client/info.txt");
        copier("C:/Qt/MesProg/cartes/president/build-client-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/client.exe", "C:/Qt/MesProg/cartes/president/Final 2/client - perso/client.exe", "Client perso", "C:/Qt/MesProg/cartes/president/Final 2/client - perso/info.txt");
    }

    if(releaseServeur->isChecked())
    {
        copier("C:/Qt/MesProg/cartes/president/build-serveur-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/serveur.exe", "C:/Qt/MesProg/cartes/president/Final 2/serveur/serveur.exe", "Serveur", "C:/Qt/MesProg/cartes/president/Final 2/serveur/info.txt");
    }

    if(winrar->isChecked())
    {
        touche(VK_LWIN,1);
        touche(VkKeyScan('r'));
        touche(VK_LWIN,-1);
        Sleep(1000);
        ecrire("C:\\Qt\\MesProg\\cartes\\president\\Final 2");
        touche(VK_RETURN);
        Sleep(1000);

        Sleep(1000);
        touche(VK_DOWN);
        touche(VK_APPS);
        Sleep(500);
        touche(VkKeyScan('a'));
        touche(VkKeyScan('a'));
        touche(VK_RETURN);
        Sleep(2000);
        for(int i=0; i<6; i++)
            touche(VK_TAB);
        ecrire("5000");
        touche(VK_TAB);
        touche(VK_DOWN);
        touche(VK_RETURN);
        Sleep(10000);//att que ça charge
        touche(VK_RIGHT);
        touche(VK_RETURN);
        Sleep(5000);//le rar a pas fini de charger


        Sleep(1000);
        touche(VK_DOWN);
        touche(VK_DOWN);
        touche(VK_APPS);
        Sleep(500);
        touche(VkKeyScan('a'));
        touche(VkKeyScan('a'));
        touche(VK_RETURN);
        Sleep(2000);
        for(int i=0; i<6; i++)
            touche(VK_TAB);
        ecrire("5000");
        touche(VK_TAB);
        touche(VK_DOWN);
        touche(VK_RETURN);
        Sleep(10000);//att que ça charge
        touche(VK_RIGHT);
        touche(VK_RETURN);
    }
    if(QFile::exists("temp.txt"))
        QFile::remove("temp.txt");//si il existe on le retire
    QFile file("temp.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write("fichier temporraire");
    file.close();//permet d'empecher l'apparition du msg d'erreur

    close();
}
