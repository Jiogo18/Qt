#include "cartes.h"

cartes::cartes()
{}

QVector<int> *cartes::Trieur(QVector<int> *tab)
{
    QVector<int> *tab2 = new QVector<int>;
    int nb, j;
    for(int i=0; i<tab->size(); i++)
    {
        nb = carteSansCouleur(tab->at(i));
        j = 0;
        while(j < tab2->size() && nb >= carteSansCouleur(tab2->at(j)))
        {
            j++;
        }
        if(tab->at(i))
        {
            tab2->insert(j, tab->at(i));
        }
        else
            tab2->append(0);

    }
    return tab2;
}

int cartes::carteSansCouleur(int carte)
{
    int retour = (carte-1)%13+1;
    if(retour == 1 || retour == 2)
        retour += 13;//as et 2 au dessus du roi
    return retour;
}

QVector<int> *cartes::generer52Cartes()
{
    QVector<int> *packet52Cartes = new QVector<int>;
    for(int i=1; i<=52; i++)
        packet52Cartes->append(i);
    return packet52Cartes;
}

bool cartes::cartesJoueurVide(QVector<int> *cartes)//si il est vide c'est qu'il contient 0 dans 13 cases
{
    for(int i=0; i<cartes->size(); i++)
        if(cartes->at(i) != 0)
            return false;
    return true;
}

QString cartes::carteEnText(int carte)
{
    QString carteMTemp="";

    switch (carteSansCouleur(carte))//nombre en mots
    {
    case 3:
        carteMTemp = "Trois";
        break;
    case 4:
        carteMTemp = "Quatres";
        break;
    case 5:
        carteMTemp = "Cinq";
        break;
    case 6:
        carteMTemp = "Six";
        break;
    case 7:
        carteMTemp = "Sept";
        break;
    case 8:
        carteMTemp = "Huit";
        break;
    case 9:
        carteMTemp = "Neuf";
        break;
    case 10:
        carteMTemp = "Dix";
        break;
    case 11:
        carteMTemp = "Valet";
        break;
    case 12:
        carteMTemp = "Dame";
        break;
    case 13:
        carteMTemp = "Roi";
        break;
    case 14:
        carteMTemp = "As";
        break;
    case 15:
        carteMTemp = "Deux";
        break;
    }
    return carteMTemp + QString(" (") + QString::number( (carte-1) %13 +1 ) + QString(")");
}
