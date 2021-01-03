#ifndef MIA_H
#define MIA_H


/*
 * Maths IA
 * Principe :
 * On a un départ et une arrivée
 *  pour le moment on demande des trucs de maths : si c'est un nombre pair, modulo 2, premier...
 * entre les 2 points on rajoute des points au hazard
 * les points sont des OPERATION

 *
 */


#include "ReseauNeurone.h"

class MIA
{
    const int trainOperation = 100;
public:
    MIA();


    void trainForPair();


private:
    ReseauNeurone reseau;
};

#endif // MIA_H
