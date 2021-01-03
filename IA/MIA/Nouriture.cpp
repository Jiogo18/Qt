#include "Nouriture.h"

void Nouriture::onOperation()
{
    addReserve(NOURITURE_PAR_OPERATION);
    operationTour++;
}

void Nouriture::onTour()
{
    addReserve(NOURITURE_PAR_TOUR);
}

void Nouriture::onTourReussi(bool demandePendantLeTour)
{
    addReserve(NOURITURE_PAR_TOUR_REUSSI);
    if(demandePendantLeTour) {
        //si il a été demandé alors il a servi à quelque chose => il est heureux
        addReserve(NOURITURE_PAR_OPERATION_REUSSI * operationTour);
    }
}


void Nouriture::addReserve(int nouriture)
{
    if(dead) return;
    reserve += nouriture;
    if(reserve > NOURITURE_MAX) reserve = NOURITURE_MAX;
    if(reserve <= 0) {
        dead = true;//dès qu'il atteint 0, il avait pas assez de réserve donc il meurt
    }
}
