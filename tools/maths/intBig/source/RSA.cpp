//importé le 10/03/2019 à 12:17
#include "RSA.h"
#include "debug.h"

RSA::RSA(fenetre *parent) : QObject(parent)
{
    //connect(this, SIGNAL(debugS(QString)), parent, SLOT(debugSlot(QString)));//envoie les debugs de rsa vers fenetre
}//fonctionne

uintBig RSA::chiffrer1(uintBig msg, uintBig d_e, uintBig n, QProgressBar *ch)
{
    if(msg >= n)
    {
        debug::d("le message est trop grand par rapport à N, stop", true);
        return msg;
    }
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    quint64 startChiffre=QDateTime::currentMSecsSinceEpoch();
    debug::d("début d'un chiffrement:"+msg.toString()+" ^ "+d_e.toString()+" % "+n.toString()+" à "+QString::number(startChiffre));
    uintBig retour(1);
    uintBig msg2;
    uintBig puiss;
    uintBig d_e2;
    ch->setMinimum(0);
    ch->setMaximum(d_e.toString().size());//la longeur de d_e
    ch->setValue(0);
    QCoreApplication::processEvents();
    while(!d_e.isEmpty())
    {
        puiss=1;
        msg2=msg;
        d_e2=d_e/2;
        while(puiss<=d_e2)
        {
            puiss *= 2;//on incrémente de 2
            msg2 = (msg2*msg2) % n;
            if(msg2 >= n)
                QThread::msleep(0);
        }
        d_e%=(puiss);//plus besoin de ces puissances
        retour *= msg2;
        retour %=n;
        ch->setValue(ch->maximum()-d_e.toString().size());
        QCoreApplication::processEvents();
    }
    ch->setValue(ch->maximum());
    quint64 stopChiffre=QDateTime::currentMSecsSinceEpoch();
    debug::d("fin de "+QString::number(startChiffre)+" : "+retour.toString()+" en "+QString::number(stopChiffre-startChiffre)+" msec");
    debug::stat("chiffrement1", startChiffre, stopChiffre);
    return retour;
}
