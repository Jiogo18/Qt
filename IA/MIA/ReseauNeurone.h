#ifndef RESEAUNEURONE_H
#define RESEAUNEURONE_H

#include <QList>
#include "Neurone.h"


class ReseauNeurone : public QObject
{
    enum CONSTANTES {
        Neurones_Start = 10,//10
        Operation_Per_Laps = 4,//4
        Chance_Create_New_Neurone = 10//10% de chance d'avoir un autre neurone
    };

public:
    ReseauNeurone();

    QList<double> doLaps(QList<double> inputs);
    void lapsWasASucess();

    void createRandomNeurone();
    void muteRandomNeurone();
    void onNeuroneDead(const Neurone *neurone);

private:
    Neurone *getRandomNeurone() const;

    void setInputs(QList<double> inputs);
    void doOneOperation();
    QList<double> getOutputs() const;
    QList<Neurone*> neurones;
};

#endif // RESEAUNEURONE_H
