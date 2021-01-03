#ifndef OPERATION_H
#define OPERATION_H


/*
 * OPERATION est un point, un neurone
 *  OPERATION commence avec 10n (nouriture)
 *  à chaque calcul OPERATION : n--
 *  si n=0 alors le neurone meurt (même pendant un tour)
 *      ça évite les boucles
 *
 *  à chaque tour tous OPERATION : n-- (pour "survivre")
 *  à chaque tour réussi
 *      tous OPERATION : n++
 *          => tous les OPERATION qui ne servent pas gagnent 0n
 *      OPERATION utilisés : n+=2 par opération
 *          => tous les OPERATION gui servent gagnent 1n pour une opération réussie
 *  réserve de nouriture : max 100 (n>100 n=100)
 *
 *  je vois une limitation : on peut pas stocker une varible
 *      donc faire *1.5 peut se faire par *1.5 mais pas par *1+*.5
 *      ça limite lorsque l'on a des trucs plus complexes :
 *      X^n + X (sauf si il trouve une formule approchée...)
 *      ou sinon il me faut un système de tick ou un système d'entrée pour modifier opConst dynamiquement
 *      ah ou non, toruvé:
 *          pour faire un calcul le neurone va faire *l'opération* avec toutes les entrées
 *          toutes les entrées ont donc leurs dernière valeur stockée
 *          pb: entre 0 pour addition et 1 pour multiplication je prends quoi?
 *          OPERATION oublie sa dernière valeur après un certain temps pour revenir à opBase
*/

#include <QObject>
#include <QRandomGenerator>
#include <QDebug>

class Operation
{
    enum CONSTANTES {
        OPTYPE_COUNT = 4,
        OPCONST_MUTATION = 10,
        OPCONST_MAX = 1000
    };
public:
    enum OperationType {
        Addition, Multiplication, Modulo, Exposant
    };


    Operation();


    double doOperation(const QList<double> input);

    bool randomMutation();
    OperationType getOperationType() const { return opType; }
    double getConst() const { return opConst; }


    static bool getRandomBool(double chance = .5) { return getRandomDouble() < chance; }
    static double getRandomDouble() { return QRandomGenerator().securelySeeded().bounded(1.0); }
private:
    void setRandomOperationType();
    static OperationType getRandomOperationType();
    void setRandomOperationConst();
    static double getRandomOperationConst();

    OperationType opType;//l'opération de OPERATION
    double opConst;//la sortie par défaut de OPERATION
};

#endif // OPERATION_H
