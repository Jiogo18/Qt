#include "fen.h"

fen::fen()
{//set widget
    {
    setWindowTitle("LumClavier.exe");

    QFormLayout *layout = new QFormLayout;

    Vitesse = new QDoubleSpinBox;
    Vitesse->setMaximum(100);
    Vitesse->setValue(1);
    layout->addRow(new QLabel("Vitesse (Hz)"), Vitesse);

    boucles = new QSpinBox;
    boucles->setMinimum(1);
    boucles->setValue(10);
    layout->addRow(new QLabel("Nombres de repetitions "), boucles);


    type = new QComboBox();
    type->addItem("Gauche-Droite");
    type->addItem("Droite-Gauche");
    type->addItem("Personnaliser");
    connect(type, SIGNAL(currentTextChanged(QString)), this, SLOT(typeChange()));
    layout->addRow(new QLabel("Type "), type);

    perso = new QTextEdit;
    perso->setDisabled(true);
    perso->append("010");
    perso->append("001");
    perso->append("100");
    layout->addRow(perso);

    bLancer = new QPushButton("Lancer");
    connect(bLancer, SIGNAL(clicked(bool)), this, SLOT(lancer()));
    layout->addRow(bLancer);

    setLayout(layout);
    }



    lumActive[0] = true;
    lumActive[1] = false;
    lumActive[2] = false;

}


void fen::lancer()
{
    bLancer->setText("Lancé...");
    bLancer->setDisabled(true);
    setDisabled(true);
    lumActive[0] = true;
    lumActive[1] = false;
    lumActive[2] = false;
    actuBoucle();
}


void fen::actuBoucle()
{
    for(int i=0; i<boucles->value(); i++)
    {
        if(type->currentText() == QString("Gauche-Droite"))
        {
            lumActive[0] = false;
            lumActive[1] = true;
            lumActive[2] = false;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
            lumActive[0] = false;
            lumActive[1] = false;
            lumActive[2] = true;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
            lumActive[0] = true;
            lumActive[1] = false;
            lumActive[2] = false;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
        }
        else if(type->currentText() == QString("Droite-Gauche"))
        {
            lumActive[0] = false;
            lumActive[1] = false;
            lumActive[2] = true;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
            lumActive[0] = false;
            lumActive[1] = true;
            lumActive[2] = false;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
            lumActive[0] = true;
            lumActive[1] = false;
            lumActive[2] = false;
            ActuLum();
            Sleep(1000/Vitesse->value() / 3);
        }
        else if(type->currentText() == QString("Personnaliser"))
        {
            QString texte = perso->toPlainText();
            for(int y=0; y <= ( (texte.size()-3) / 4 ); y++)
            {
                QString ligne = texte.mid(y*4, 3);
                for(int x=0; x<3; x++)
                    lumActive[x] = ligne[x] == QChar('1');
                ActuLum();
                Sleep(1000/Vitesse->value());
            }
        }
    }
    bLancer->setText("Lancer");
    bLancer->setEnabled(true);
    setEnabled(true);

}

void fen::ActuLum()
{
    BYTE touche;

    for(int i=0; i<3; i++)
    {
        switch (i)
        {
        case 0:
            touche = VK_NUMLOCK;
            break;
        case 1:
            touche = VK_CAPITAL;
            break;
        case 2:
            touche = VK_SCROLL;
            break;
        }

        BYTE keyState[256];
        GetKeyboardState((LPBYTE)&keyState);

        if( (lumActive[i] && !(keyState[touche] & 1)) ||
                (!lumActive[i] && (keyState[touche] & 1)) )
        {
           keybd_event(touche, 0x45, 0, 0);
           keybd_event(touche, 0x45, 2, 0);
        }
        QCoreApplication::processEvents();
    }
}

void fen::typeChange()
{
    if(type->currentText() == QString("Personnaliser"))
    {
        perso->setEnabled(true);
    }
    else
        perso->setDisabled(true);

}


