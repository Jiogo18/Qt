#ifndef NOURITURE_H
#define NOURITURE_H


class Nouriture
{

public:
    //peut être paramètre pour savoir *par exemple* si un apprentissage "doux" ou "exigent" est mieux pour MIA

    enum CONSTANTES {
        NOURITURE_BASE = 10,
        NOURITURE_PAR_OPERATION = -1,//travailler ça donne faim
        NOURITURE_PAR_TOUR = -1,
        NOURITURE_PAR_OPERATION_REUSSI = +2,//si il a servi à qq chose, récompense
        NOURITURE_PAR_TOUR_REUSSI = +1,//si il fait rien il consomme 0
        NOURITURE_MAX = 100//ne dépasse pas
    };


    Nouriture() {}
    void onOperation();
    void onTour();
    void onTourReussi(bool demandePendantLeTour);
    bool isDead() const { return dead; }


private:
    int reserve = NOURITURE_BASE;
    int operationTour = 0;
    bool dead = false;
    void addReserve(int nouriture);
    void removeReserve(int nouriture) { return addReserve(-nouriture); }
};

#endif // NOURITURE_H
