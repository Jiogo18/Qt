#include "convertisseurBases.h"
#include <QDateTime>

convertisseurBases::convertisseurBases() {}

QVector<quint64> convertisseurBases::CustomToCustom(QVector<quint64> value, quint64 base1, quint64 base2)//on prend des vecteurs qui ont ete créé avec les conversions QString en bas
{
    if(base1==base2)//si c les meme bases, on s'en occupe pas
        return value;
    QVector<quint64> retour(1);
    retour[0]=0;
    if(isNull(value))//pas besoin de caluler plus
        return retour;
    QVector<quint64> valueCase(1);

    for(quint64 i=value.size(); i>0; i--)
    {
        valueCase=toVector(value[i-1], base2);//trop lent avec le pow()
        if(!isNull(valueCase))//si c'est 0 ça sert a rien (correction pour pas ralentir de trop)
            retour = plus(retour, fois(pow(base1, i-1, base2), valueCase, base2), base2);
    }
    while(retour.last()==0)
        retour.removeLast();
    return retour;


    //return DecToCustom(CustomToDec(value, base1), base2);
}

//plus utile
QVector<quint64> convertisseurBases::DecToCustom(QVector<quint64> value, quint64 baseCustom)
{
    /*QVector<quint64> retour(1);
    retour[0]=0;
    if(baseCustom==10)
        return value;

    if(isNull(value))//pas besoin de caluler plus
        return retour;

    quint64 i=0;
    while(estPetitOuEgal(pow(baseCustom,i, 10), value))
        i++;//cb de "cases" pour tout stocker*/
// ! modifs à continuer//   ^ peut etre mettre plus rapide que ++?
    //quint64 nbDePow=0;

    /*while(i > 0)
    {
        i--;
        nbDePow=0;
        while(estGrandOuEgal(value, pow(baseCustom, i, 10)))
        {
            value = moins(value, pow(baseCustom, i, 10), 10);
            nbDePow++;
        }
        QString nbDePow2 = QString::number(nbDePow);

        retour.push_front(nbDePow2.toULongLong());
    }*/


    /*while(!isNull(value))
    {
        while(estGrandOuEgal(value, pow(baseCustom, i, 10)))
        {
            value = moins(value, pow(baseCustom, i, 10), 10);
            while(i >= (quint64)retour.size())
                retour.append(0);
            retour[i] += 1;
        }
        if(i>0)
            i--;
    }*/
    QVector<quint64> retour(1);
    retour[0]=0;
    if(isNull(value))//pas besoin de caluler plus
        return retour;
    if(baseCustom==10)//conversion10-10 etait déja bonne, petit raccourcis
        return value;
    QVector<quint64> valueCase(1);

    for(quint64 i=value.size(); i>0; i--)
    {
        valueCase=toVector(value[i-1], baseCustom);//trop lent avec le pow()
        if(!isNull(valueCase))//si c'est 0 ça sert a rien (correction pour pas ralentir de trop)
            retour = plus(retour, fois(pow(10, i-1, baseCustom), valueCase, baseCustom), baseCustom);
    }
    while(retour.last()==0)
        retour.removeLast();
    return retour;
}

QVector<quint64> convertisseurBases::CustomToDec(QVector<quint64> value, quint64 baseCustom)
{
    QVector<quint64> retour(1);
    retour[0]=0;
    if(isNull(value))//pas besoin de caluler plus
        return retour;
    if(baseCustom==10)//conversion10-10 etait déja bonne, petit raccourcis
        return value;
    QVector<quint64> valueCase(1);

    for(quint64 i=value.size(); i>0; i--)
    {
        valueCase=toVector(value[i-1], 10);//trop lent avec le pow()
        if(!isNull(valueCase))//si c'est 0 ça sert a rien (correction pour pas ralentir de trop)
            retour = plus(retour, fois(pow(baseCustom, i-1, 10), valueCase, 10), 10);
    }//                                            ^value.size()-i remplacé pour test
    while(retour.last()==0)
        retour.removeLast();
    return retour;
}

QVector<quint64> convertisseurBases::toVector(QString value, QString base)
{
    QVector<quint64> retour(1);
    retour[0]=0;
    if(base.isEmpty()||base.isNull())//on va dire que c'est en ascii
    {
        for(int i=0; i<value.size(); i++)
        {
            while(i >= retour.size())
                retour.append(0);
            retour[i]=value[value.size()-i-1].unicode();
        }
    }
    else//on fait a partir du str
    {
        for(int i=0; i<value.size(); i++)
        {
            while(i >= retour.size())
                retour.append(0);
            retour[i]=base.indexOf(value[value.size()-i-1]);
        }
    }
    return retour;
}

QVector<quint64> convertisseurBases::toVector(quint64 value, quint64 baseRetour)
{
    //return toVector(QString::number(value), base);//base n'est pas un str
    QVector<quint64> retour(1);
    quint64 pos=0;
    while(value > 0)
    {
        if(pos >= (quint64)retour.size())
            retour.append(0);
        retour[pos] = value%baseRetour;
        value=value/baseRetour;//retenue
        pos++;
    }
    return retour;
}

QString convertisseurBases::toString(QVector<quint64> value)//str n'a pas de char/base défini, retourne str avec le code ascii
{
    QString retour;
    for(int i=0; i<value.size(); i++)
        retour[i] = QChar::unicodeVersion(value[i]);
    return retour;
}

bool convertisseurBases::isNull(QVector<quint64> value)
{
    if(value.isEmpty())
        return true;
    for(quint64 i=0; i<(quint64)value.size(); i++)
        if(value[i]!=0)
            return false;
    return true;
    /*QString str=toString(value);
    while(str.contains("0"))
        str.remove("0");
    return str=="" || str.isNull() || str.isEmpty();*/
}

QVector<quint64> convertisseurBases::pow(quint64 a, quint64 b, quint64 base)
{
    QVector<quint64> retour(1);
    retour[0]=1;
    QVector<quint64> a2=toVector(a, base);
    /*a2[0]=a;
    quint64 pos=0, retenue=0;
    while(retenue!=0 || pos==0)//repete si tu as des retenu ou que c'est le début
    {//toVector? ^corrigé
        a2[pos] += retenue;
        retenue=a2[pos] / base;
        a2[pos] %= base;
        if(retenue!=0)//sinon pas besoin d'agrandir pour rien
        {
            pos++;
            while(pos >= (quint64)a2.size())
                a2.append(0);
        }
        else if(pos==0)
            pos++;
    }*/

    for(quint64 i=0; i<b; i++)//si a^0
    {// doit etre plus rapide...
        retour = fois(retour, a2, base);
    }
    return retour;
}

bool convertisseurBases::estPetitOuEgal(QVector<quint64> a, QVector<quint64> b)
{
    if(a.size() != b.size())//si ils ont une taille diff, regarder
        return a.size() < b.size();

    for(quint64 i=a.size(); i>0; i--)//eviter d'aller en dessous de 0, quint64 n'est pas negatif
    {
        if(a[i-1] != b[i-1])
            return a[i-1] < b[i-1];
    }
    return true;//est egal
}

QVector<quint64> convertisseurBases::moins(QVector<quint64> a, QVector<quint64> b, quint64 base)
{
    while(a.size() < b.size())
        a.append(0);
    while(a.size() > b.size())
        b.append(0);
    quint64 retenue=0, retenueT;
    for(quint64 i=0; i<(quint64)a.size(); i++)
    {
        retenueT=0;
        while(a[i] < retenue+b[i])
        {
            retenueT++;
            a[i] += base;
        }
        a[i] -= retenue+b[i];
        retenue=retenueT;
        retenueT += a[i]/base;//si il en reste, on a peut etre mal calculé
        a[i] %= base;
    }
    if(retenue)
    {
        qDebug("convertisseurBases::moins a est plus petit que b, il reste la retenue la valeur de retour est donc fausse");
        return QVector<quint64>();
    }
    while(a.last()==0 && a.size() > 1)//faudrait pas qu'il soit vide
        a.removeLast();
    return a;
}

QVector<quint64> convertisseurBases::plus(QVector<quint64> a, QVector<quint64> b, quint64 base)
{
    while(a.size() < b.size())
        a.append(0);
    while(a.size() > b.size())
        b.append(0);

    quint64 retenue=0;
    for(quint64 i=0; i<(quint64)a.size() || retenue!=0; i++)
    {
        while(i>=(quint64)a.size())
            a.append(0);
        while(i>=(quint64)b.size())
            b.append(0);
        a[i]+=retenue;
        retenue=0;

        if((a[i] + b[i]) >= base)
        {
            retenue=(a[i]+b[i])/base;//normalement on est en base 10
            a[i]=(a[i]+b[i]) % base;
        }
        else
            a[i]=a[i]+b[i];
    }
    while(a.last()==0 && a.size() > 1)//faudrait pas qu'il soit vide
        a.removeLast();
    return a;
}

QVector<quint64> convertisseurBases::fois(QVector<quint64> a, QVector<quint64> b, quint64 base)
{//multiplie les 2 vecteurs
    QVector<quint64> retour(1);
    retour[0]=0;
    for(quint64 i=0; i<(quint64)a.size(); i++)
    {
        for(quint64 i2=0; i2<(quint64)b.size(); i2++)
        {
            /*quint64 mult=a[i]*b[i2];
            quint64 retenueCase=0;
            while(mult>0)
            {
                while(i+i2+retenueCase >= (quint64)retour.size())
                    retour.append(0);
                retour[i+i2+retenueCase] += mult % base;
                mult /= base;//restant
                if(retour[i+i2+retenueCase]>=base)
                {
                    quint64 retenueCaseT=retour[i+i2+retenueCase]/base;
                    retour[i+i2+retenueCase]%=base;
                    retenueCase+=retenueCaseT;
                }
                retenueCase++;
            }marche paaaaaasssss*/
            QVector<quint64> mult(i+i2+1);//le mettre  la taille min pr que ça passe
            mult[i+i2] = a[i]*b[i2];//on passe le calcul ds un vector
            retour=plus(retour, mult, base);//qu'on additionne
        }
    }
    return retour;
}

bool convertisseurBases::estGrandOuEgal(QVector<quint64> a, QVector<quint64> b)
{
    if(a.size() != b.size())//si ils ont une taille diff, regarder
        return a.size() > b.size();

    for(quint64 i=a.size(); i>0; i--)//eviter d'aller en dessous de 0, quint64 n'est pas negatif
    {
        if(a[i-1] != b[i-1])
            return a[i-1] > b[i-1];
    }
    return true;//est egal
}
