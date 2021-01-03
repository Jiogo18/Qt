#include "ReseauNeurone.h"

ReseauNeurone::ReseauNeurone()
{
    //on créé un FirstInput et un LastOutput pour commencer
    neurones.append(new NeuroneInputOutput());
    for(int i=0; i<Neurones_Start; i++)
        createRandomNeurone();
}


QList<double> ReseauNeurone::doLaps(QList<double> inputs)
{
    setInputs(inputs);

    //on calcul !
    for(int i=0; i<Operation_Per_Laps; i++) {
        doOneOperation();
    }
    for(int i=0; i<neurones.size(); i++) {
        neurones.at(i)->onLapsOver();
    }
    return getOutputs();
}

void ReseauNeurone::lapsWasASucess()
{
    for(int i=0; i<neurones.size(); i++) {
        neurones.at(i)->onLapsReussi();
    }
}



void ReseauNeurone::createRandomNeurone()
{
    Neurone *input = getRandomNeurone();
    Neurone *output= getRandomNeurone();
    Neurone *neurone = new NeuroneOperation({input}, {output});
    neurones.append(neurone);
    connect(neurone, &Neurone::onDead, this, &ReseauNeurone::onNeuroneDead);

    if(Operation::getRandomBool(Chance_Create_New_Neurone/100.0))
        createRandomNeurone();
}

void ReseauNeurone::muteRandomNeurone()
{
    Neurone *neurone = nullptr;
    int i=0;
    do {
        i++;
        if(i>1000) {
            createRandomNeurone();
            return;//pas de mutation
        }
        neurone = getRandomNeurone();
    } while(!neurone || neurone->isInputOutput());

    bool muted = neurone->randomMutation();
    qDebug() << "muteRandomNeurone" << muted;
}

void ReseauNeurone::onNeuroneDead(const Neurone *neurone)
{ neurones.removeAll(neurone); }

Neurone *ReseauNeurone::getRandomNeurone() const
{
    if(Operation::getRandomBool(.3))//30% de chance de pas cibler
        return nullptr;
    return neurones.at(QRandomGenerator::securelySeeded().bounded(neurones.size()));
}





void ReseauNeurone::setInputs(QList<double> inputs)
{
    //on met en place les inputs (NeuroneInputOutput)
    int iInp = 0;
    for(int iNeur=0; iNeur<neurones.size(); iNeur++) {
        if(iInp >= inputs.size()) break;

        Neurone *neurone = neurones.at(iNeur);
        if(!neurone->isInputOutput()) break;

        neurone->setValue(inputs.at(iInp));
        iInp++;
    }
    if(iInp < inputs.size()) {
        qWarning("Il restait des valeurs d'input mais il n'y a plus de NeuroneInputOutput");
    }
}

void ReseauNeurone::doOneOperation()
{
    //faire un tour
    for(int i=0; i<neurones.size(); i++) {
        neurones.at(i)->doOperation();
    }
}

QList<double> ReseauNeurone::getOutputs() const
{
    //on compte les outputs (NeuroneInputOutput)
    QList<double> retour;
    for(int iNeur=0; iNeur<neurones.size(); iNeur++) {
        Neurone *neurone = neurones.at(iNeur);
        if(!neurone->isInputOutput()) continue;

        neurone->doOperation();
        retour.append(neurone->getFinalValue());
    }
    return retour;
}
