#include "RSA.h"

RSA::RSA() {}

bool RSA::isPrimeNumber(const quint64 nb, QProgressBar *ch)
{
    if(nb%2==0)
        return false;
    ch->setMaximum(sqrt(nb));
    ch->setMinimum(0);
    for(quint64 i=3; i<=sqrt(nb); i+=2)
    {
        if(nb % i == 0)
            return false;
        ch->setValue(i);
        QCoreApplication::processEvents();
    }
    return true;
}



bool RSA::arePrime(intBig a, intBig b)
{
    intBig temp;
    /*while(nb2>0)
    {
        temp = nb2%nb1;
        nb1 = nb2;
        nb2 = temp;
    }
    return nb1 == 1;//if 1, they are prime else nb1 = PGCD*/
    //PGCD scratch

    if(a<b)
    {
        temp=a;
        a=b;
        b=temp;
    }
    temp=a;
    while(b>0)
    {
        //while(temp>=b)
            //temp -= b;
        temp %= b;//plus rapide
        a=b;
        b=temp;
        temp=a;
    }
    return temp==1;//temp== pgcd et si c'est 1, ils sont premiers entre eux
}

QMap<QString, QString> RSA::generer(QString nbPremier1, QString nbPremier2, QProgressBar *ch1, QProgressBar *ch2, QProgressBar *ch3)
{
    //qDebug() << "RSA::generer from thread" << QThread::currentThread();
    quint64 start=QDateTime::currentMSecsSinceEpoch();
    ch1->setMaximum(99);
    ch1->setValue(0);//progression des 2 isPrime
    ch2->setMaximum(99);
    ch2->setValue(0);//progression de  l'ajout dans la liste
    ch3->setValue(0);//progression du chiffrement
    debug("Début de la génération.");
    QMap<QString,QString> retour;
    retour.insert("message","Erreur 1");//message (d'erreur) n'est pas chargé
    retour.insert("termine","0");
    retour.insert("n","Erreur 2");//n n'est pas chargé
    retour.insert("e","Erreur 3");//e n'est pas chargé
    retour.insert("d","Erreur 4");//d n'est pas chargé
    retour.insert("nbPremier1", nbPremier1);
    retour.insert("nbPremier2", nbPremier2);

    intBig p(nbPremier1,10);
    intBig q(nbPremier2,10);
    debug("Base 10: p="+p.toString()+", q="+q.toString());
    if(!p.isPrime(ch1))//p mais en quint64 pour pas faire trop long
    {
        retour["message"] = "Le nombre 1 n'est pas premier";
        debug(retour["message"]);
        return retour;
    }
    QCoreApplication::processEvents();
    debug("premier nombre validé");
    if(!q.isPrime(ch1))
    {
        retour["message"] = "Le nombre 2 n'est pas premier";
        debug(retour["message"]);
        return retour;
    }
    ch1->setMaximum(99);
    ch1->setValue(90);
    QCoreApplication::processEvents();
    debug("deuxieme nombre validé");
    if(p == q)
    {
        retour["message"] = "Les nombres premiers doivent ètre différents";
        debug(retour["message"]);
        return retour;
    }
    ch1->setValue(92);
    debug("les 2 nombres sont validés");
    debug("Les nombres donnés sont bons, création de n, e et d.");

    intBig n(p*q);
    retour["n"] = n.toString();
    debug("n="+retour["n"]);
    intBig phi = (p-1) * (q-1);
    debug("phi="+phi.toString());
    intBig e = 0;
    intBig d = 0;
    QList<intBig> *listeE = new QList<intBig>;
    ch1->setValue(99);

    bool trouve=false;
    for(intBig i=phi-1; i>=2 && !trouve; i--)
    {
        if(arePrime(i, phi))//si ils sont premiers entre eux, on les prends
            listeE->append(i);

        ch2->setValue(listeE->size());
        QCoreApplication::processEvents();
        if(listeE->size()>=100)//si on est à 100 nbs, on en choisit un
        {//on test les nb ds la meme boucle pour recalculer si besoin
            while(listeE->size()>=10 && !trouve)//si on dépasse le seuil de 10 nbs, on en recalcul pour avoir de la marge
            {
                e = listeE->at(random64(0, listeE->size()));
                //debug("test de "+e.toString(10));
                d=InverseBModuloN(e, phi);

                if(d>=2)
                    if(chiffrer(n/2, d, n, ch3)!=n/2)
                        //if(chiffrer(n/2, d, n)!=chiffrer(n/2, e, n))//si ça donne un truc à boucle (M^E%N = M^D%N...)
                        {
                            trouve=true;
                            break;
                        }
                listeE->removeAll(e); e=0; d=0;//on test un autre nb
                ch2->setValue(listeE->size());
                QCoreApplication::processEvents();
                //debug("il reste "+QString::number(listeE->size())+" nombres dans la liste");
            }
        }
    }
    if(!trouve)
    {
        while(!trouve)//tant qu'on en a pas trouvé, on continue
        {
            e=listeE->at(random64(0, listeE->size()));
            d=InverseBModuloN(e, phi);

            if(d>=2)//si ça donne un truc à boucle (M^E%N = M^D%N...)
                if(chiffrer(n/2, d, n, ch3)!=n/2)//on pourrait test avec e aussi
                    //if(chiffrer(n/2, d, n)!=chiffrer(n/2, e, n))//on doit garder ça?
                    {//sur un p=13 et q=17, il y a pas asser de choix
                        trouve=true;
                        break;
                    }
            listeE->removeAll(e); e=0; d=0;//on test un autre nb
            ch2->setValue(listeE->size());
            QCoreApplication::processEvents();
            if(listeE->isEmpty())
            {
                retour["message"]="Aucun 'e' ne répond aux critères.";
                debug(retour["message"]);
                return retour;
            }
        }
    }
/*
    int i=phi-1;
    while((i >= 2 || e==0 || d==0) && listeE->size())
    {
        if(i>2)
        {
            i--;
            if(arePrime(i, n))
                listeE->append(i);
        }
        if(listeE->size()>=1000 || i<=2)//si on a asser de nb ou qu'on arrive à la fin
        {
            int i2 = random64(0, listeE->size());
            e=listeE->at(i2);

            intBig phi2=phi;
            intBig e2=e;
            intBig s=0;
            intBig t=1;
            d = phi2/e2;
            intBig r=phi-d*e;
            intBig u=0;
            while(r)
            {
                u = s-d*t;
                u = abs(u)%phi2;
                s = t;
                t = u;
                phi2 = e2;
                e2 = r;
                d = phi2/e2;
                r = phi2-d*e2;
            }
            if(e2==1)//test si ça donne pas un truc a boucle (M^E%N = M^D%N...)
            {
                d = u;
                break;
            }
            else
            {
                listeE->removeAt(i2);
                e=0;
                d=0;
            }
        }
    }*/

    //retour des nbs
    ch2->setValue(ch2->maximum());
    retour["e"] = e.toString();
    retour["d"] = d.toString();
    if(retour["e"] == "0")
    {
        retour.insert("message","Erreur 5");//e est nul
        debug(retour["message"]);
        return retour;
    }
    if(retour["d"] == "0")
    {
        retour.insert("message","Erreur 6");//d et nul
        debug(retour["message"]);
        return retour;
    }

    retour["message"] = "Calcul terminé en " + QString::number( QDateTime::currentMSecsSinceEpoch()-start ) + " msec.";
    retour["termine"] = "1";
    debug(retour["message"]);
    return retour;
}

void RSA::debug(QString str)
{
    qDebug(QString("[" + QTime::currentTime().toString("hh:mm:ss") + "] "+str).toStdString().c_str());
}

quint64 RSA::random64(quint32 min, quint32 max)
{
    QRandomGenerator64 rd=QRandomGenerator64::securelySeeded();
    return rd.bounded(min, max);
}

intBig RSA::InverseBModuloN(intBig b, intBig n)
{
    /*intBig d;
    d = n/b;

    intBig s=0;
    intBig t=1;
    intBig r=n-d*b;
    intBig u=0;
    while(r.isEmpty())
    {
        if(s<d*t)
        {
            u=d*t-s;
        }
        else
            u = s-d*t;//        \ ancienne
        //u = abs(u)%n;//     / version
        u %= n;
        s = t;
        t = u;
        phi = b;
        b = r;
        d = phi/e;
        r = phi-d*e;
    }
    return d;*/
    //https://www.apprendre-en-ligne.net/crypto/rabin/euclide.html
    intBig n0=n;
    intBig b0=b;
    intBig t0=0;
    intBig t=1;
    intBig q=n0/b0;
    intBig r=n0 - q * b0;
    while(r > 0)
    {
          intBig temp = t0 - q * t;
          if(temp >= 0)
          {
              temp = temp % n;
          }
          else
              temp = n - ((intBig(0)-temp) % n);

          t0 = t;
          t = temp;
          n0 = b0;
          b0 = r;
          q = n0/b0;
          r = n0 - q * b0;
    }
    if(b0 != 1)
    {//b n'a pas d'inverse modulo n
        return 0;
    }
    else
        return t;
}

intBig RSA::chiffrer(intBig msg, intBig d_e, intBig n, QProgressBar *ch)
{
    if(msg >= n)
        return msg;
    /*QDateTime start=QDateTime::currentDateTime();
    qDebug(QString("début d'un chiffrement:"+msg.toString()+" ^ "+d_e.toString()+" % "+n.toString()).toStdString().c_str());
    intBig d_e2_1=d_e;
    intBig retour_1=1;
    intBig msg2_1;
    intBig puiss_1(2);//pour pas en creer pleins
    do
    {
        d_e2_1--;
        msg2_1=msg;
        puiss_1=1;
        while((puiss_1) < d_e2_1)
        {
            msg2_1 = (msg2_1^2) % n;
            d_e2_1 -= (puiss_1);
            puiss_1 *= 2;
        }
        retour_1=(retour_1*msg2_1) % n;
        QCoreApplication::processEvents();

    } while(!d_e2_1.isEmpty());
    qDebug(QString("retour chiffrement:"+retour_1.toString()+" en "+QString::number(QDateTime::currentMSecsSinceEpoch()-start.toMSecsSinceEpoch())+" msec").toStdString().c_str());
    //return a;*/
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    QDateTime start2=QDateTime::currentDateTime();
    //qDebug(QString("début d'un chiffrement:"+msg.toString()+" ^ "+d_e.toString()+" % "+n.toString()).toStdString().c_str());
    intBig retour(1);
    intBig msg2;
    intBig puiss;
    intBig d_e2;
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
    //qDebug(QString("fin chiffrement:"+retour.toString()+" en "+QString::number(QDateTime::currentMSecsSinceEpoch()-start2.toMSecsSinceEpoch())+" msec").toStdString().c_str());
    return retour;
}
