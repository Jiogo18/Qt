#ifndef NEURONE_H
#define NEURONE_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include "Nouriture.h"
#include "Operation.h"



class Neurone : public QObject
{
    Q_OBJECT
public:
    Neurone();
    Neurone(QList<Neurone*> inputs, QList<Neurone*> outputs);
    ~Neurone();
    virtual void doOperation() { qFatal("A simple Neurone can't be created"); }
    virtual double getValue() { return outputValue; }
    virtual void onFriendDead(const Neurone *neurone);
    virtual bool onInputWanted(Neurone *neurone);//neurone veut faire parti des input de this

    //pour NeuroneOperation
    virtual bool isDead() const { return false; }
    virtual void onLapsOver() {}
    virtual void onLapsReussi() {}
    virtual Operation *getOperation() { return nullptr; }
    virtual bool randomMutation() { return false; }


    //pour NeuroneInputOutput
    virtual bool isInputOutput() const { return false; }
    virtual void setValue(double value) { Q_UNUSED(value) }
    virtual double getFinalValue() { return getValue(); }


signals:
    void onDead(const Neurone*);

protected:
    double outputValue = 0;
    QList<Neurone*> inputs;//neurone d'entrée (peut être vide dans le cas d'une entrée constante !)
    QList<Neurone*> outputs;
};




class NeuroneOperation : public Neurone
{
    enum CONSTANTES {
        LASTVAL_MEMORY = 10//retient lastVal pendant Xms
    };

public:
    NeuroneOperation(QList<Neurone *> input, QList<Neurone *> output);
    ~NeuroneOperation();
    void doOperation() override;
    double getValue() override;
    bool isDead() const override { return nouriture.isDead(); }
    void onLapsOver() override;
    void onLapsReussi() override;
    Operation *getOperation() override { return operation; }
    bool randomMutation() override { return operation->randomMutation(); }


private:
    void setOutputValue(double value);
    void resetOutputValue();
    int demandePendantLeTour;
    int lastDemandePendantLeTour;
    int lastValTime = 0;//temps de la dernière sortie de NEURONE

    Nouriture nouriture;
    Operation *operation;
};



class NeuroneInputOutput : public Neurone {
public:
    NeuroneInputOutput();//neurone d'entrée et de sortie, ne meurt jamais
    bool isInputOutput() const override { return true; }

    //le FirstInput se fait sur la partie inputValue, setValue et getValue
    void setValue(double value) override { inputValue = value; }//par le réseau
    double getValue() override { return inputValue; }//pour les neurones

    //le LastOutput se fait sur la partie outputValue, doOperation() et getFinalValue()
    bool onInputWanted(Neurone *neurone) override;//par les neurones
    void doOperation() override;//par les neurones
    double getFinalValue() override { return outputValue; }//pour le réseau

private:
    double inputValue = 0;
};

#endif // NEURONE_H
