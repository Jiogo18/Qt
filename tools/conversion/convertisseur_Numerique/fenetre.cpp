#include "fenetre.h"

fenetre::fenetre()
{
    QVBoxLayout *dLayout = new QVBoxLayout;
    QFormLayout *layout = new QFormLayout;

    lblDecimal = new QLabel("Décimal : ");
    lineDecimal = new QLineEdit;
    connect(lineDecimal, SIGNAL(textEdited(QString)), this, SLOT(calculDecimal()));
    layout->addRow(lblDecimal, lineDecimal);

    lblBinaire = new QLabel("Binaire : ");
    lineBinaire = new QLineEdit;
    connect(lineBinaire, SIGNAL(textEdited(QString)), this, SLOT(calculBinaire()));
    layout->addRow(lblBinaire, lineBinaire);

    lblHexadecimal = new QLabel("Hexadécimal : ");
    lineHexadecimal = new QLineEdit;
    connect(lineHexadecimal, SIGNAL(textEdited(QString)), this, SLOT(calculHexadecimal()));
    layout->addRow(lblHexadecimal, lineHexadecimal);

    dLayout->addLayout(layout);

    QHBoxLayout *layoutCustom = new QHBoxLayout;
    lblCustom = new QLabel("Custom : ");
    layoutCustom->addWidget(lblCustom);
    spinCustom = new QSpinBox;
    spinCustom->setMinimum(2);
    spinCustom->setMaximum(1000);//max 999
    spinCustom->setMaximum(1000000);//grace au QString::number
    spinCustom->setValue(10);
    layoutCustom->addWidget(spinCustom);
    lineCustom = new QLineEdit;
    connect(lineCustom, SIGNAL(textEdited(QString)), this, SLOT(calculCustom()));
    layoutCustom->addWidget(lineCustom);
    dLayout->addLayout(layoutCustom);

    QHBoxLayout *layoutCustomTxt = new QHBoxLayout;
    lblCustomTxt = new QLabel("Custom 2 : ");
    layoutCustomTxt->addWidget(lblCustomTxt);
    lineCustomTxt = new QLineEdit;
    lineCustomTxt->setReadOnly(true);
    layoutCustomTxt->addWidget(lineCustomTxt);
    dLayout->addLayout(layoutCustomTxt);


    setLayout(dLayout);
    calcul=false;
}


QString fenetre::DecToBin(QString valeur)
{
    int valeur2=valeur.toInt();
    if(valeur2==0)
        return "0";

    QString retour;
    int i=0;
    while(pow(2,i) <= valeur2)//avec 8, il faut 4 bytes
    {
        i++;
        retour += "0";
    }

    while(valeur2 > 0)
    {
        if(valeur2 >= pow(2,i))
        {
            valeur2 -= pow(2, i);
            retour[retour.size()-i-1] = '1';
        }
        i--;
    }
    return retour;
}

QString fenetre::BinToHexa(QString valeur)
{
    QString retour;
    while(valeur.size()%4 != 0)
        valeur = "0" + valeur;

    for(int i=0; i<valeur.size()/4; i++)
    {
        QString hexa="0123456789abcdef";
        int nbPartie=BinToDec(QString(valeur.mid(valeur.size()-i*4-4, 4))).toInt();
        retour = hexa.mid(nbPartie, 1) + retour;
    }

    while(retour.startsWith("0"))
        retour = retour.remove(0,1);
    return retour;
}

QString fenetre::HexaToBin(QString valeur)
{
    QString retour;


    for(int i=valeur.size()-1; i>=0; i--)
    {
        QString hexa="0123456789abcdef";
        QString binPartie = DecToBin(QString::number(hexa.lastIndexOf(valeur[i])));
        while(binPartie.size() < 4)
                binPartie = "0"+binPartie;
        retour = binPartie + retour;
    }
    while(retour.startsWith("0"))
        retour = retour.remove(0, 1);

    return retour;
}

QString fenetre::BinToDec(QString valeur)
{
    int valeur2=0;
    for(int i=0; i<valeur.size(); i++)
    {
        if(valeur[i] == '1')
            valeur2 += pow(2,valeur.size()-i-1);
    }
    return QString::number(valeur2);
}

QString fenetre::DecToCustom(QString valeur, int custom)
{
    int valeur2=valeur.toInt();
    if(valeur2==0)
        return "000";

    QString retour;
    int i=0;
    while(pow(custom,i+1) <= valeur2)
        i++;

    int nbDePow;
    while(i >= 0)
    {
        nbDePow=0;
        while(valeur2 >= pow(custom,i))
        {
            valeur2 -= pow(custom, i);
            nbDePow++;
        }
        QString nbDePow2 = QString::number(nbDePow);
        while(nbDePow2.size() < log10(custom))//avec 99, il faudra un str de 2 chars
            nbDePow2 = nbDePow2.insert(0, "0");

        retour.append(nbDePow2 + ".");
        i--;
    }
    retour.remove(retour.size()-1, 1);

    QVector<quint64> valueV2=convertisseurBases::toVector(valeur, "0123456789");
    QVector<quint64> retourV2=convertisseurBases::CustomToCustom(valueV2, 10, spinCustom->value());
    QString retourV2_2=convertisseurBases::toString(retourV2);
    qDebug() << valeur << retour << retourV2_2 << valueV2 << retourV2;
    return retour;
}

QString fenetre::CustomToDec(QString valeur)
{
    int valeurRetour=0;
    int custom=spinCustom->value();
    QString nbChar;
    int nbDeCharPasse=0;

    for(int i=valeur.size()-1; i>=0; i--)
    {
        nbChar="";
        while(i>=0 && valeur[i] != '.')
        {
            nbChar = valeur[i] + nbChar;
            i--;
        }
        valeurRetour += pow(custom, nbDeCharPasse)*nbChar.toInt();
        nbDeCharPasse++;
    }
    return QString::number(valeurRetour);
}

void fenetre::calculCustomTxt()
{
    QString strCustom=QString::number(lineDecimal->text().toInt(), spinCustom->value());
    QString retour=strCustom + " (";
    for(int i=0; i<strCustom.size(); i++)
    {
        retour+=QString::number(strCustom[i].toLatin1());
        if(i<strCustom.size()-1)
            retour+=";";
    }

    lineCustomTxt->setText(retour + ")");
}


void fenetre::calculDecimal()
{
    if(calcul)
        return;
    calcul=true;

    lineBinaire->setText("0");
    lineHexadecimal->setText("0");
    lineCustom->setText("0");

    lineBinaire->setText(DecToBin(lineDecimal->text()));
    lineHexadecimal->setText(BinToHexa(lineBinaire->text()));
    lineCustom->setText(DecToCustom(lineDecimal->text(), spinCustom->value()));

    calculCustomTxt();


    calcul=false;
}

void fenetre::calculBinaire()
{
    if(calcul)
        return;
    calcul=true;

    lineDecimal->setText("0");
    lineHexadecimal->setText("0");
    lineCustom->setText("0");

    lineDecimal->setText(BinToDec(lineBinaire->text()));
    lineHexadecimal->setText(BinToHexa(lineBinaire->text()));
    lineCustom->setText(DecToCustom(lineDecimal->text(), spinCustom->value()));

    calculCustomTxt();

    calcul=false;
}

void fenetre::calculHexadecimal()
{
    if(calcul)
        return;
    calcul=true;

    lineDecimal->setText("0");
    lineBinaire->setText("0");
    lineCustom->setText("0");

    lineBinaire->setText(HexaToBin(lineHexadecimal->text()));
    lineDecimal->setText(BinToDec(lineBinaire->text()));
    lineCustom->setText(DecToCustom(lineDecimal->text(), spinCustom->value()));

    calculCustomTxt();

    calcul=false;
}

void fenetre::calculCustom()
{
    if(calcul)
        return;
    calcul=true;

    lineDecimal->setText("0");
    lineBinaire->setText("0");
    lineHexadecimal->setText("0");

    lineDecimal->setText(CustomToDec(lineCustom->text()));
    lineBinaire->setText(DecToBin(lineDecimal->text()));
    lineHexadecimal->setText(BinToHexa(lineBinaire->text()));

    calculCustomTxt();

    calcul=false;
}
