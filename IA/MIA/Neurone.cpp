#include "Neurone.h"

Neurone::Neurone()
{}

Neurone::Neurone(QList<Neurone *> inputs, QList<Neurone *> outputs) : QObject()
{
    inputs.removeAll(nullptr);
    this->inputs = inputs;
    outputs.removeAll(nullptr);
    for(int i=outputs.size()-1; i>=0; i--) {
        if(!outputs.at(i)->onInputWanted(this))
            outputs.removeAt(i);
    }
    this->outputs = outputs;
}

Neurone::~Neurone()
{
    qDebug() << "Un neurone est mort";
    for(int i=0; i<inputs.length(); i++) {
        if(inputs.at(i)) inputs.at(i)->onFriendDead(this);
    }
    for(int i=0; i<outputs.length(); i++) {
        if(outputs.at(i)) outputs.at(i)->onFriendDead(this);
    }
    emit onDead(this);//même si this est détruit, c'est un pointeur qui permet de comparer
}


void Neurone::onFriendDead(const Neurone *neurone)
{
    inputs.removeAll(neurone);
    outputs.removeAll(neurone);
    //si output est vide il va mourir de par lui même (car il est plus demandé donc nouriture n'augmente pas assez)
}

bool Neurone::onInputWanted(Neurone *neurone)
{
    if(inputs.contains(neurone)) {
        return false;
    }
    inputs.append(neurone);
    return true;
}



NeuroneOperation::NeuroneOperation(QList<Neurone *> input, QList<Neurone *> output) : Neurone(input, output)
{
    operation = new Operation();
    resetOutputValue();
    demandePendantLeTour = 0;

}

NeuroneOperation::~NeuroneOperation()
{
    delete operation;
}



void NeuroneOperation::doOperation()
{
    if(isDead()) return;//va être delete... quand le process sera entièrement terminé... TODO
        //qFatal("already dead");

    QList<double> inputVal;
    for(int i=0; i<inputs.size(); i++) {
        inputVal.append(inputs.at(i)->getValue());
    }
    setOutputValue(operation->doOperation(inputVal));

    nouriture.onOperation();
    if(isDead()) deleteLater();
}

double NeuroneOperation::getValue() {
    demandePendantLeTour++;
    if(lastValTime != 0 && lastValTime+10 < QDateTime::currentMSecsSinceEpoch()) {
        //si le temps est coulé alors on retourne la valeur par défaut
        //moins de calculs de date (mais du coup pas une fonction const)
        resetOutputValue();
    }
    return outputValue;
}

void NeuroneOperation::onLapsOver() {
    nouriture.onTour();
    lastDemandePendantLeTour = demandePendantLeTour;
    demandePendantLeTour = 0;
    resetOutputValue();
}

void NeuroneOperation::onLapsReussi() {
    nouriture.onTourReussi(lastDemandePendantLeTour);
    //executé après onLapsOver() (on peut pas mettre les calculs de reset ici parceque Reussi uniquement)
}




void NeuroneOperation::setOutputValue(double value)
{
    outputValue = value;
    lastValTime = QDateTime::currentMSecsSinceEpoch();
}

void NeuroneOperation::resetOutputValue()
{
    outputValue = operation->getConst();
    lastValTime = 0;
}





NeuroneInputOutput::NeuroneInputOutput() : Neurone()
{}

bool NeuroneInputOutput::onInputWanted(Neurone *neurone)
{
    if(!inputs.isEmpty()) return false;//place déjà occupée (le LastOutput ne peut avoir qu'une input)
        //le reste passera à la trappe de tt façon
    return Neurone::onInputWanted(neurone);
}

void NeuroneInputOutput::doOperation()
{
    //prend la valeur du premier neurone stocké
    outputValue = inputs.isEmpty() ? 0 : inputs.first()->getValue();
}

