#include "intBig.h"

intBigP::intBigP(quint64 value)
{
    baseP=2;//on sait pas quoi mettre d'autre
    operator =(value);
}
intBigP::intBigP(quint64 baseV, quint64 value)
{
    baseP=baseV;
    operator =(value);
}
intBigP::intBigP(QString value, quint64 base)
{baseP=base; negatifP=false; operator =(fromText(value, base));}
intBigP::intBigP(QVector<quint64> value, quint64 base)
{baseP=base; negatifP=false; valeurP=value; resize();}
intBigP::intBigP(const intBigP &value)
{operator =(value);}
intBigP::intBigP(const intBigP *value)
{operator =(*value);}

void intBigP::resize()
{
    if(baseP < 2)
    {
        qDebug("intBigP.cpp : base à 0 ou 1 resize");
        setBase(2);
    }
    //actualise les cases
    quint64 i;
    for(i=size(); i>0; i--)
    {
        if(at(i-1) != 0)
            break;
        valeurP.removeAt(i-1);//si c'est 0 (vide)
    }

    i=0;
    quint64 retenue=0;
    while(i<sizeNNul() || retenue>0)
    {
        while(i >= size())
            valeurP.append(0);
        operator [](i) +=retenue;
        retenue = at(i)/base();
        operator [](i) %= base();
        i++;
    }
}

intBigP intBigP::fromText(QString value, quint64 base)//from text
{
    intBigP retour(base, 0);
    if(value.startsWith("-"))
    {
        retour.setNegative(true);
        value.remove("-");
    }
    else
        retour.setNegative(false);
    if(base == 10 || base==16 || base==64 || base==100)
    {
        for(int i=0; i<value.size(); i++)
            retour[i] += strOfBase(base).indexOf(value[value.size()-i-1]);
    }
    else//en ascii, continuer si besoin de faire avec les séparation points
        for(int i=0; i<value.size(); i++)
            retour[i] += value.at(i).unicode();

    retour.resize();
    return retour;
}
intBigP intBigP::fromVector(QVector<quint64> value, quint64 base)
{
    intBigP retour(base, 0);
    retour.setValue(value, base);
    return retour;
}

void intBigP::setNegative(bool negative) {negatifP=negative;}
void intBigP::setValue(QVector<quint64> value, quint64 base)
{
    valeurP=value;
    baseP=base;
    resize();
}
void intBigP::setBase(quint64 base) {operator =(toBase(base));}

QString intBigP::toString(quint64 base) const
{
    QString retour="";
    intBigP me2(toBase(base));
    if(base==10 || base==16 || base==64 || base==100)
    {
        for(quint64 i=me2.size(); i>0; i--)
            retour.append(strOfBase(base).at(me2.at(i-1)));
    }
    else
        for(quint64 i=0; i<me2.size(); i++)
            retour.replace(i, 1, QString::number(me2.at(me2.size()-i-1), base));
            //en ascii (normalement base ne sert a rien mais sécurité)
    if(retour.isEmpty())
        retour="0";
    if(isNegative())
        retour.push_front("-");
    return retour;
}
QVector<quint64> intBigP::toVector(quint64 base) const
{
    if(base==baseP)
        return valeurP;
    intBigP retour(toBase(base));
    if(base != retour.base())
        qDebug("intBigP.cpp : erreur toBase (toString)");
    return retour.toVector(base);//rappel de la fonction mais juste pour donner valeurP
}
intBigP intBigP::toBase(quint64 base) const
{//conversion de base
    intBigP retour(base, 0);
    if(intBigP::base() == base)//si c les meme bases
        return intBigP(valeurP, base);
    if(isEmpty())//si c'est vide
        return retour;

    intBigP valueCase(base, 0);
    intBigP baseCase(base, 0);
    for(quint64 i=sizeNNul(); i>0; i--)
    {
        valueCase=at(i-1);//trop lent avec le pow()
        if(!valueCase.isEmpty())
        {//si c'est 0 ça sert a rien
            baseCase=intBigP::base();
            baseCase = baseCase^(i-1);
            retour += baseCase * valueCase;
        }
    }
    retour.resize();
    return retour;
}
bool intBigP::isEmpty() const {return sizeNNul() == 0;}

quint64 intBigP::sizeNNul() const
{
    quint64 size=intBigP::size();
    while(size>0 && at(size-1) == 0)
        size--;//si il reste des cases et que celle en size-1 est nulle
    return size;
}


const quint64 &intBigP::at(const quint64 i) const
{
    if(i>=size())//si existe pas, c'est 0
        return retourNul;
    return valeurP.at(i);
}
quint64 &intBigP::operator[](const quint64 i)
{
    while(i >= size())//si existe pas, on agrandit
        valeurP.append(0);//pour quand on set la case avec une new valeur
    return valeurP[i];
}
void intBigP::operator ++(int) {operator +=(1);}
void intBigP::operator --(int) {operator -=(1);}
intBigP intBigP::sqrt() const
{
    if(isNegative())
    {
        qDebug("erreur sqrt, racine d'un nombre négatif");
        return intBigP(base(), 0);
    }
    if(isEmpty())
        return intBigP(base(), 0);//0²=0
    if(operator ==(1))
        return intBigP(base(), 1);//1²=1

    intBigP min(base(), 1);
    intBigP retour(min);//commencer par la
    intBigP fois(operator /(2));
    while(fois>1)
    {
        while(operator >=(retour*retour))
        {//augmentation au fur et à mesure
            min=retour;
            retour=retour*fois;
        }
        retour=min;
        fois /= 20;
    }

    intBigP plus(retour);
    while(plus > 0)
    {
        while(operator >=(retour*retour))
        {//augmentation au fur et à mesure
            min=retour;
            retour += plus;
        }
        retour=min;
        if(plus < 20 && plus>1)
        {
            plus=1;//+1
        }
        else
            plus /= 20;
    }
    //ici retour² est == ou au dessous de this
    return retour;
}
bool intBigP::isPrime(QProgressBar *ch) const
{
    if(toVector(2).at(0)==0)
        return false;
    intBigP sqrtNb=sqrt();
    ch->setMinimum(1);
    ch->setMaximum(sqrtNb.toString().size());
    ch->setValue(1);
    for(intBigP i=3; i<=sqrtNb; i+=2)
    {//les pairs ont déja ete testé
        if(operator %(i) == 0)
            return false;
        ch->setValue(i.toString().size());
        QCoreApplication::processEvents();
    }
    ch->setValue(ch->maximum());
    return true;
}

intBigP intBigP::operator ^(intBigP v) const
{
    intBigP retour(base(), 1);
    intBigP retour2(*this);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}
intBigP intBigP::operator ^(quint64 v) const
{
    intBigP retour(base(), 1);
    intBigP retour2(*this);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}
intBigP intBigP::operator %(intBigP v) const
{
    intBigP retour(base(), 0);
    if(v==1 || v==0)//ça donnera 0 (avec v==0 normalement c'est non défini mais c'est généralement %0.1)
        return retour;
    intBigP v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    retour=operator -( (operator /(v2))*v2 );
    while(retour >= v2)//compense l'opération en haut, normalement pas utile
    {
        qDebug()<<"intBig : erreur %"<<toString()<<v2.toString()<<"donne"<<retour.toString();
        retour -= v2;//(algo de modulo de base mais trop lent donc juste pour resize correctement)
    }
    while(retour.isNegative() && retour<0)
        retour += v2;//si il est encore négatif, on le remonte d'un (ex:-9%4 = -1 => 3)
    retour.resize();
    return retour;
}
intBigP intBigP::operator *(const intBigP &v) const
{
    intBigP retour(base(), 0);
    intBigP v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    for(quint64 i=0; i<size(); i++)
    {
        for(quint64 i2=0; i2<v2.size(); i2++)
        {
            quint64 mult=at(i)*v2.at(i2);
            quint64 retenueCase=0;
            while(mult>0)
            {
                retour[i+i2+retenueCase] += mult % base();
                mult /= base();//restant
                retenueCase++;
            }
        }
    }
    retour.setNegative(isNegative() != v2.isNegative());
    retour.resize();
    return retour;
}
intBigP intBigP::operator *(quint64 v) const
{
    //convertir en base v et ensuite ajouter une case
    QVector<quint64> retour = toVector(v);
    retour.insert(0, 0);//ajoute au début
    intBigP retour2(retour, v);//on le repasse en intBigP
    retour2.setNegative(isNegative());//il y a qu moi qui peut etre -
    retour2.setBase(base());
    return retour2;
}
intBigP intBigP::operator /(const intBigP &v) const
{//division
    intBigP retour(base(), 0);
    if(isEmpty())//forcement 0
        return retour;
    if(v.isEmpty())//1/0
    {
        qDebug("%s", QString("erreur division, le dénominateur vaut 0 "+v.toString()).toStdString().c_str());
        return retour;
    }

    intBigP v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    //quint64 i=sizeNNul();//le plus grand quint64
    //quint64 i2=v2.sizeNNul();//le plus grand quint64
    if(operator <(v2))
    {//si c plus grand la division fera 0.xxx
        //si seulement un seul est -, c'est -
        retour.setNegative(isNegative() != v2.isNegative());
        return retour;
    }

    intBigP plus(base(), 0);
    intBigP min(base(), 0);
    plus[sizeNNul()-1] = at(sizeNNul()-1);
    retour=min;
    while(plus > 0)
    {
        intBigP retour2(retour*v2);
        intBigP plus2(v2*plus);//plus optimisé
        while(operator >=(retour2))
        {
            min=retour;
            //retour[plus.sizeNNul()-1] += plus[plus.sizeNNul()-1];
            retour += plus;
            retour2 += plus2;
        }
        retour=min;

        if(plus.sizeNNul() > 1 || plus >= 20)
        {
            quint64 i3=plus.sizeNNul();
            if(plus[i3-1] >= 20 || i3==1)
            {
                plus[i3-1] /= 20;//divise juste le quint64 de la case
            }
            else
            {
                plus[i3-2] += (base()*plus[i3-1]) / 20;
                plus[i3-1] = 0;
            }
        }
        else if(plus==1)
        {
            plus=0;//si on est déja en +1 on s'arrete
        }
        else//faire du +1
            plus=1;
    }
    //si seulement un seul est -, c'est -
    retour.setNegative(isNegative() != v2.isNegative());
    return retour;
}
intBigP intBigP::operator /(quint64 v) const
{
    //convertir en base v et ensutie retirer la premiere case
    QVector<quint64> retour = toVector(v);
    retour.pop_front();//retire le premier
    intBigP retour2(retour, v);//on le repasse en intBigP

    retour2.setNegative(isNegative());//il y a qu moi qui peut etre -
    retour2.setBase(base());

    return retour2;
}
intBigP intBigP::operator +(intBigP v) const
{
    if(isNegative() && v.isNegative())
    {// - "+" - = -(+ "+" +)
        intBigP t(valeurP, base());
        t.setNegative(!isNegative());
        v.setNegative(!v.isNegative());
        t+=v;
        t.setNegative(!t.isNegative());//change d'etat
        return t;
    }
    if(isNegative())//- "+" + = + "-" +
    {
        intBigP t(valeurP, base());
        t.setNegative(!isNegative());
        return v - t;
    }
    if(v.isNegative())//+ "+" - = + "-" +
    {
        v.setNegative(!v.isNegative());
        return operator -(v);
    }
    intBigP v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    intBigP retour(base(), 0);
    for(quint64 i=0; i<sizeNNul() || i<v2.sizeNNul() || i<retour.sizeNNul(); i++)
    {
        retour[i] += at(i) + v2.at(i);
        if(retour[i] >= base())//si c trop grand
        {
            retour[i+1] += retour.at(i)/base();//retenue
            retour[i] %= base();//retire les retenues
        }
    }
    return retour;
}
intBigP intBigP::operator +(quint64 v) const
{
    intBigP retour(this);
    retour[0] += v;
    retour.resize();
    return retour;
}
intBigP intBigP::operator -(intBigP v) const
{
    if(isNegative() || (isNegative() && v.isNegative()))
    {//inverser ou - "-" - = -(+ "-" +)
        intBigP t(valeurP, base());
        t.setNegative(!isNegative());
        v.setNegative(!v.isNegative());//on le retourne
        t-=v;
        t.setNegative(!t.isNegative());
        return t;
    }
    if(v.isNegative())
    {//+ "-" - = + "+" +
        v.setNegative(!v.isNegative());
        return operator +(v);
    }

    intBigP v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    if(operator <(v2))
    {//si v est plus grand  + "-" + = -(- "-" -) =-(- "+" +)= -(+"-"+)
        intBigP t(valeurP, base());
        t=v2-t;
        t.setNegative(!t.isNegative());
        return t;
    }
    intBigP retour(valeurP, base());
    quint64 retenue=0;
    for(quint64 i=0; i<sizeNNul() ; i++)
    {//je suis forcement plus grand donc size() est le max
        if(retour.at(i) < v2.at(i)+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v2.at(i)+retenue)
            {
                retour[i] += base();
                retenueT++;
            }
            retour[i] -= (v2.at(i) + retenue);
            retenue = retenueT;
        }
        else
        {
            retour[i] -= (v2.at(i) + retenue);
            retenue=0;
        }
    }
    return retour;
}
intBigP intBigP::operator -(quint64 v) const
{
    if(isNegative())
    {//si je suis negatif
        intBigP t(valeurP, base());//     - "-" +
        t.setNegative(!isNegative());        //=> -(+ "+" -)
        t=t-v;                          //=> -(+ "-" +)
        t.setNegative(!t.isNegative());
        return t;
    }
    intBigP retour(valeurP, base());
    quint64 retenue=0;
    for(quint64 i=0; i<size() ; i--)
    {
        if(retour.at(i) < v+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v+retenue)
            {
                retour[i] += base();
                retenueT++;
            }
            retour[i] -= (v + retenue);
            v=0;//ici un quint64 on a du le vider entièrement
            retenue = retenueT;
        }
        else
        {
            retour[i] -= (v + retenue);
            retenue=0;
            v=0;//pareil
            break;//si on arrive la c'est surrement parce que il y a plus rien
        }
    }
    return retour;
}


void intBigP::operator %=(const intBigP &v) {operator =(operator %(v));}
void intBigP::operator %=(quint64 v) {operator =(operator %(v));}
void intBigP::operator *=(const intBigP &v) {operator =(operator *(v));}
void intBigP::operator *=(quint64 v) {operator =(operator *(v));}
void intBigP::operator /=(const intBigP &v) {operator = (operator /(v));}
void intBigP::operator /=(quint64 v) {operator = (operator /(v));}
void intBigP::operator +=(const intBigP &v) {operator =(operator +(v));}
void intBigP::operator +=(quint64 v) {operator =(operator +(v));}
void intBigP::operator -=(const intBigP &v) {operator =(operator -(v));}
void intBigP::operator -=(quint64 v) {operator =(operator -(v));}

bool intBigP::operator !=(const intBigP &v) const
{
    return !operator ==(v);
    /*if(isNegative() != v.isNegative())
        return true;
    intBigP v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
        return true;
    for(quint64 i=0; i<sizeNNul(); i++)
        if(at(i) != v2.at(i))//si ils sont diff à un moment, c'est bon
            return true;
    return false;//si ils sont diff nul part, ils sont pareils*/
}
bool intBigP::operator !=(quint64 v) const {return operator !=(intBigP(base(), v));}
bool intBigP::operator ==(const intBigP &v) const
{
    if(isNegative()!=v.isNegative())
        return false;
    intBigP v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
        return false;
    for(quint64 i=0; i<sizeNNul(); i++)
        if(at(i) != v2.at(i))
            return false;
    return true;
}
bool intBigP::operator ==(quint64 v) const {return operator ==(intBigP(base(), v));}
bool intBigP::operator <(const intBigP &v) const
{
    if(isNegative() != v.isNegative())
    {
        return isNegative() && !v.isNegative();
    }
    intBigP v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
    {
        if(isNegative() && v2.isNegative())
        {
            return !(sizeNNul() < v2.sizeNNul());
        }
        else return sizeNNul() < v2.sizeNNul();
    }

    for(quint64 i=sizeNNul(); i>0; i--)
    {
        if(at(i-1) != v2.at(i-1))
        {
            if(isNegative() && v2.isNegative())
            {
                return !(at(i-1) < v2.at(i-1));
            }
            else return at(i-1) < v2.at(i-1);
        }
    }
    return false;//est egal
}
bool intBigP::operator <(quint64 v) const {return operator <(intBigP(base(), v));}
bool intBigP::operator <=(const intBigP &v) const {return operator <(v) || operator ==(v);}
bool intBigP::operator <=(quint64 v) const {return operator <=(intBigP(base(), v));}
bool intBigP::operator >(const intBigP &v) const {return !operator <=(v);}
bool intBigP::operator >(quint64 v) const {return operator >(intBigP(base(), v));}
bool intBigP::operator >=(const intBigP &v) const {return !operator <(v);}
bool intBigP::operator >=(quint64 v) const {return operator >=(intBigP(base(), v));}

void intBigP::operator =(const intBigP &v)
{
    baseP=v.base();
    if(baseP < 2)
    {
        qDebug("intBigP : base à 0 ou 1 (operator =)");
        baseP=2;
    }
    valeurP=v.toVector(baseP);
    negatifP=v.isNegative();
    resize();
}
void intBigP::operator =(quint64 v)
{
    valeurP.clear();
    negatifP=false;
    operator [](0) = v;
    resize();
}

const QString intBigP::strOfBase(quint64 base)
{
    switch (base) {
    case 10:
        return "0123456789";
    case 16:
        return "0123456789abcdef";
    case 64:
        return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&";
    case 100:
        return"☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !e#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[f]^_`abcd";
    default:
        return "";
    }
}

QString intBigP::debug() const
{
    QString retour="Base:"+QString::number(base())+", ";
    if(isNegative())
        retour += "-";
    retour += "{";
    for(quint64 i=0; i<size(); i++)
    {
        retour += QString::number(at(i));
        if(i+1 < size())
            retour += ", ";
    }
    return "intBigP { "+retour+"} }";
}



intBig::intBig(quint64 value) : intBigP(baseP, value)
{}

intBig::intBig(QString value, quint64 base) : intBigP(value, base)
{setBase(baseP);}

intBig::intBig(QVector<quint64> value, quint64 base) : intBigP(value, base)
{setBase(baseP);}

intBig::intBig(const intBig &value) : intBigP(value.toVector(baseP), baseP)
{
    negatifP=value.isNegative();
    resize();
}

intBig::intBig(const intBigP &value) : intBigP(value.toBase(baseP))
{
    negatifP=value.isNegative();
    resize();
}
