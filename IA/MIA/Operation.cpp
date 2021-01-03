#include "Operation.h"

Operation::Operation()
{
    setRandomOperationType();
    setRandomOperationConst();
}

double Operation::doOperation(const QList<double> input)
{
    if(input.isEmpty()) return opConst;
    double retour;
    //règle importante : toujours utiliser le 1er terme comme valeur spéciale
    //car sinon input.last change dès qu'un nouveau neurone se connecte en input
    switch (opType) {
        case OperationType::Addition: {//addition de tous
            retour = 0;
            for(int i=0; i<input.size(); i++) retour += input.at(i);
            break;
        }
        case OperationType::Multiplication: {//multiplication de tous
            retour = 1;
            for(int i=0; i<input.size(); i++) retour *= input.at(i);
            break;
        }
        case OperationType::Modulo: {//addition des premiers modulo le dernier
            retour = 0;
            for(int i=1; i<input.size(); i++) retour += input.at(i);
            std::fmod(retour,input.first());
            break;
        }
        case OperationType::Exposant: {//addition des derniers termes ^ premier terme
            retour = 0;
            for(int i=1; i<input.size(); i++) retour += input.at(i);
            retour = qPow(retour, input.first());
            break;
        }

    }
    return retour;
}






bool Operation::randomMutation()
{
    OperationType oldType = opType;
    double oldConst = opConst;
    if(getRandomBool(.8)) {//mutation de opType
        setRandomOperationType();
    }
    if(getRandomBool(.2)) {//mutation de opConst
        opConst += getRandomOperationConst() * CONSTANTES::OPCONST_MUTATION;//relatif à la valeur actuelle
    }

    return oldType != opType || oldConst != opConst;
}





void Operation::setRandomOperationType()
{
    opType = getRandomOperationType();
}
Operation::OperationType Operation::getRandomOperationType()
{
    int randomOp = QRandomGenerator().securelySeeded().bounded(CONSTANTES::OPTYPE_COUNT);
    switch (randomOp) {
        case OperationType::Addition: return OperationType::Addition;
        case OperationType::Multiplication: return OperationType::Multiplication;
        case OperationType::Modulo: return OperationType::Modulo;
        case OperationType::Exposant: return OperationType::Exposant;
    }
    qWarning() << "Operation Type" << randomOp;
    qFatal("Invalid Operation Type");
    return Operation::Addition;
}

void Operation::setRandomOperationConst()
{
    opConst = getRandomOperationConst() * CONSTANTES::OPCONST_MAX;//max (au pire il fera plusieurs neurones)
}
double Operation::getRandomOperationConst()
{
    double number = getRandomDouble();
    number *= number;//les nombres sont plus proche de 0
    if(getRandomBool()) {//negatif?
        number = -number;
    }
    return number;
}


