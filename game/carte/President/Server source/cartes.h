#ifndef CARTES_H
#define CARTES_H

#include <QtWidgets>
#include <QString>

class cartes
{
    public:
        cartes();
        static QVector<int> *Trieur(QVector<int> *tab);
        static int carteSansCouleur(int carte);
        static QVector<int> *generer52Cartes();
        static bool cartesJoueurVide(QVector<int> *cartes);
        static QString carteEnText(int carte);

};

#endif // CARTES_H
