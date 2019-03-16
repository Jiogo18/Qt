#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include <QString>
#include "convertisseurBases.h"

class fenetre : public QWidget
{
    Q_OBJECT
    public:
        fenetre();
        QString DecToBin(QString valeur);
        QString BinToHexa(QString valeur);
        QString HexaToBin(QString valeur);
        QString BinToDec(QString valeur);
        QString DecToCustom(QString valeur, int custom);
        QString CustomToDec(QString valeur);
        void calculCustomTxt();

    public slots:
        void calculDecimal();
        void calculBinaire();
        void calculHexadecimal();
        void calculCustom();

    private:
        QLabel *lblDecimal;
        QLineEdit *lineDecimal;
        QLabel *lblBinaire;
        QLineEdit *lineBinaire;
        QLabel *lblHexadecimal;
        QLineEdit *lineHexadecimal;

        QLabel *lblCustom;
        QSpinBox *spinCustom;
        QLineEdit *lineCustom;
        QLabel *lblCustomTxt;
        QLineEdit *lineCustomTxt;
        bool calcul;

};

#endif // FENETRE_H
