#include "MIA.h"

MIA::MIA()
{
    qDebug("Démarrage de MIA");


    for(int i=0; i<trainOperation; i++) {
        trainForPair();
        reseau.muteRandomNeurone();
    }

}

void MIA::trainForPair()
{
    double input = QRandomGenerator::securelySeeded().bounded(100);//des entiers svp
    QList<double> output = reseau.doLaps({input});

    qDebug() << "trainForPair" << input << output;
}
