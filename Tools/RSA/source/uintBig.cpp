#include "uintBig.h"

uintBig::uintBig(quint64 value)
{
    valeur = QVector<quint64>(64);//si besoin, agrandir la taille du vecteur
    /* chaques quint64 ne dépassera pas 2^31-1
     * 2^63-1=9223372036854775807
     * sqrt(2^63-1)=3 037 000 499,9
     * 2^31-1=2147483648, il y a la place*/
    valeur[0]=value;
    negatif=false;
    resize();
}

uintBig::uintBig(QString value, quint64 base)
{
    if(value.startsWith("-"))
    {
        negatif=true;
        value.remove("-");
    }
    else
        negatif=false;
    valeur = QVector<quint64>(64);
    operator =(fromText(value, base));
}

uintBig::uintBig(QVector<quint64> value, quint64 base)
{
    negatif=false;
    valeur=convertisseurBases::CustomToCustom(value, base, baseDesCases);
    resize();//le vetor est forcement trop petit
}

/*uintBig::uintBig(const uintBig &value)
{
    valeur=value.toVector(baseDesCases);
}*/

QString uintBig::toString(quint64 base) const
{
    QString retour="";
    switch (base)
    {
        case 10:
        {//base10 est null, le convertisseurBases::CustomToCustom n'est pas près
            QVector<quint64> base10=convertisseurBases::CustomToCustom(valeur, baseDesCases, 10);
            for(quint64 i=base10.size(); i>0; i--)
                retour.append(strOfBase(10).at(base10.at(i-1)));
            break;
        }

        case 16:
        {
            QVector<quint64> base16=convertisseurBases::CustomToCustom(valeur, baseDesCases, 16);
            for(quint64 i=base16.size(); i>0; i--)
                retour.append(strOfBase(16).at(base16.at(i-1)));
            break;
        }

        case 64:
        {
            QVector<quint64> base64=convertisseurBases::CustomToCustom(valeur, baseDesCases, 64);

            for(quint64 i=base64.size(); i>0; i--)
                retour.append(strOfBase(64).at(base64.at(i-1)));
            break;
        }

        case 100:
        {
            QVector<quint64> base100=convertisseurBases::CustomToCustom(valeur, baseDesCases, 100);
            for(quint64 i=base100.size(); i>0; i--)
                retour.append(strOfBase(100).at(base100.at(i-1)));
            break;
        }


        default:
            QVector<quint64> baseC=convertisseurBases::CustomToCustom(valeur, baseDesCases, base);
            for(quint64 i=0; i<(quint64)baseC.size(); i++)
                //retour.replace(i, 1, QString::fromUtf16((ushort)baseC.at(i)));//en ascii
                retour.replace(i, 1, QString::number(baseC.at(baseC.size()-i-1), base));//en ascii (normalement base ne sert a rien mais sécurité)
            break;
    }
    if(negatif)
        retour.push_front("-");
    return retour;
}

QVector<quint64> uintBig::toVector(quint64 base) const
{
    if(base==baseDesCases)
        return valeur;
    return convertisseurBases::CustomToCustom(valeur, baseDesCases, base);
}

uintBig uintBig::fromText(QString value, quint64 base)//from text
{
    uintBig retour;
    if(value.startsWith("-"))
    {
        retour.setNegative(true);
        value.remove("-");
    }
    else
        retour.setNegative(false);
    switch (base)
    {
        case 10:
            retour = convertisseurBases::CustomToCustom(convertisseurBases::toVector(value, strOfBase(10)), 10, baseDesCases);
            break;

        case 16:
            retour = convertisseurBases::toVector(value, strOfBase(16));
            break;

        case 64:
            retour = convertisseurBases::toVector(value, strOfBase(64));
            break;

        case 100:
            retour = convertisseurBases::toVector(value, strOfBase(100));
            break;

        default://en ascii, continuer si besoin de faire avec les séparation points
            for(int i=0; i<value.size(); i++)
            {
                retour += value.at(i).unicode();//pas fini, loins de la // ou pas
            }
            break;
    }
    return retour;
}


bool uintBig::operator !=(const uintBig &v)
{
    if(size() != v.size())//normalement impossible
        return true;
    if(negatif!=v.isNegative())
        return true;//si l'un est negatif et l'autre positif, ils sont !=
    for(quint64 i=0; i<size(); i++)
    {
        if(valeur.at(i) != v.at(i))//si ils sont diff à un moment, c'est bon
            return true;
    }
    return false;//si ils sont diff nul part, ils sont pareils
}

uintBig uintBig::operator +(uintBig v)
{
    if(negatif && v.isNegative())
    {// - "+" - = -(+ "+" +)
        uintBig t(valeur, baseDesCases);
        t.setNegative(!t.isNegative());
        v.setNegative(!v.isNegative());
        t+=v;
        t.setNegative(!t.isNegative());//change d'etat, pourrais etre prédéterminé mais c'est mieux comme ça
        return t;
    }
    if(negatif)//- "+" + = + "-" +
    {
        uintBig t(valeur, baseDesCases);
        t.setNegative(!negatif);
        return v - t;
    }
    if(v.isNegative())//+ "+" - = + "-" +
    {
        v.setNegative(!v.isNegative());
        return operator -(v);
    }
    uintBig retour;
    for(quint64 i=0; i<size() || i<size(); i++)
    {
        retour[i] += valeur.at(i) + v.at(i);
        if(retour[i] >= baseDesCases)//si c trop grand
        {
            retour[i+1] += retour.at(i)/baseDesCases;//retenue
            retour[i] %= baseDesCases;//retire les retenues
        }
    }
    return retour;
}

void uintBig::operator +=(const uintBig &v) {operator =(operator +(v));}

void uintBig::operator ++(int) {operator +=(1);}

uintBig uintBig::operator -(uintBig v)
{
    if((negatif && v.isNegative()) || negatif)
    {//- "-" - = -(+ "-" +)
        uintBig t(valeur, baseDesCases);
        t.setNegative(!negatif);
        v.setNegative(!v.isNegative());//on le retourne
        t-=v;
        t.setNegative(!t.isNegative());
        return t;
    }
    if(v.isNegative())//+ "-" - = + "+" +
    {
        v.setNegative(!v.isNegative());
        return operator +(v);
    }
    if(operator <(v))
    {//si v est plus grand  + "-" + = -(- "-" -) =-(- "+" +)= -(+"-"+)
        uintBig t(valeur, baseDesCases);
        t=v-t;
        t.setNegative(!t.isNegative());
        return t;
    }
    uintBig retour(valeur, baseDesCases);
    quint64 retenue=0;
    for(quint64 i=0; i<size() ; i++)
    {
        if(retour.at(i) < v.at(i)+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v.at(i)+retenue)
            {
                retour[i] += baseDesCases;
                retenueT++;
            }
            retour[i] -= (v.at(i) + retenue);
            retenue = retenueT;
        }
        else
        {
            retour[i] -= (v.at(i) + retenue);
            retenue=0;
        }
    }
    return retour;
}

uintBig uintBig::operator -(quint64 v)
{
    if(negatif)
    {//si je suis negatif
        uintBig t(valeur, baseDesCases);//     - "-" +
        t.setNegative(!negatif);        //=> -(+ "+" -)
        t=t-v;                          //=> -(+ "-" +)
        t.setNegative(!t.isNegative());
        return t;
    }
    uintBig retour(valeur, baseDesCases);
    quint64 retenue=0;
    for(quint64 i=0; i<size() ; i--)
    {
        if(retour.at(i) < v+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v+retenue)
            {
                retour[i] += baseDesCases;
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

void uintBig::operator -=(const uintBig &v) {operator =(operator -(v));}

void uintBig::operator --(int) {operator -=(1);}

uintBig uintBig::operator /(const uintBig &v) const
{//division
    //marche pas//convertir en base v et ensutie retirer la premiere case (marche pas si v est un uintBig car trop gros)
    //(convertisseurBases::CustomToCustom(valeur, baseDesCases, v), baseDesCases);

    /*for(quint64 i=0; i<size(); i++)
    {
        for(quint64 i2=0; i2<v.size(); i2++)
        {
            quint64 div=valeur[i]/v[i2];//si bug parce que x/0, tt la fonction est fausse
            quint64 retenueCase=0;
            while(div>0)
            {
                retour[i-i2+retenueCase]=div % baseDesCases;
                div /= baseDesCases;//restant
                retenueCase++;
            }
        }
    }*/

    uintBig retour;
    if(v.isEmpty())
    {
        qDebug(QString("erreur division, le dénominateur vaut 0 "+v.toString()).toStdString().c_str());
        return retour;
    }
    if(isEmpty())
        return retour;

    quint64 i=size();
    while(valeur.at(i-1)==0 && i>0)//on veut le plus grand quint64
        i--;
    quint64 i2=v.size();
    while(v.at(i2-1)==0 && i2>0)//on veut le plus grand quint64
        i2--;
    if(i2 > i)//si c plus grand la division sera trop importante
    {
        retour.setNegative((negatif && !v.isNegative()) || (!negatif && v.isNegative()));//si seulement un est -, c'est -
        return retour;
    }


    uintBig plus;//plus=this
    uintBig min;
    min[(i-1)-(i2-1)]=valeur.at(i-1)/v.at(i2-1);//1,1945557583 / 3=>1/3 en [1]
    if(min.at((i-1)-(i2-1))==0)//possible si le case ^
    {
        plus = *this;
    }
    else
        plus=min;

        /*if(i>=2)//sinon c'est 0 (ex:1/2)
        {
            if(valeur[i-2]!=0)
            {
                min[(i-2)-(i2-1)]=((baseDesCases-1)*valeur[i-2])/v[i2-1];
            }
            else//sinon on considère que c'est 1, mais on réduira le plus après si besoin
                min[(i-2)-(i2-1)]=(baseDesCases-1)/v[i2-1];*/
        //}//on fait rien en fait, pour test le new constructeur de plus (avant plus=retour)
    retour=min;
    while(plus > 0)
    {
        while(retour*v < *this)
        {
            min=retour;
            retour+=plus;
        }
        retour=min;
        //plus /= 10;
        //division plus simple
        if(plus >=10)
        {
            quint64 i3=v.size();
            while(plus.at(i3-1)==0 && i3>0)//on veut le plus grand quint64
                i3--;
            plus[i3-1] /= 10;//divise juste le quint64 de la case
            if(plus.at(i3-1)==0 && i3>1)//si c'est la premiere case, pas besoin
            {
                plus[i3-2] /= 10;
                if(plus.at(i3-2)==0)//si c'est encore le cas
                    plus[i3-2] = baseDesCases-1;//tester ça
            }
        }
        else if(plus==1)//si c'est déja 1
        {
            plus=0;//si on est déja en +1 on s'arrete
        }
        else//faire du +1
            plus=1;
    }
    min.setNegative((negatif && !v.isNegative()) || (!negatif && v.isNegative()));

    return min;
}

uintBig uintBig::operator /(quint64 v) const
{
    //convertir en base v et ensutie retirer la premiere case
    QVector<quint64> retour = convertisseurBases::CustomToCustom(valeur, baseDesCases, v);
    retour.pop_front();//retire le premier

    uintBig retour2(retour, v);
    retour2.setNegative(negatif);//il y a qu moi qui peut etre -
    return retour2;//on le repasse en uintBig
}

void uintBig::operator /=(const uintBig &v) {operator = (operator /(v));}

void uintBig::operator /=(quint64 v) {operator = (operator /(v));}

uintBig uintBig::operator *(const uintBig &v) const
{
    uintBig retour;
    for(quint64 i=0; i<size(); i++)
    {
        for(quint64 i2=0; i2<v.size(); i2++)
        {
            quint64 mult=valeur.at(i)*v.at(i2);
            quint64 retenueCase=0;
            while(mult>0)
            {
                retour[i+i2+retenueCase] += mult % baseDesCases;
                mult /= baseDesCases;//restant
                retenueCase++;
            }
        }
    }
    retour.setNegative((negatif && !v.isNegative()) || (!negatif && v.isNegative()));
    retour.resize();
    return retour;
}

void uintBig::operator *=(const uintBig &v) {operator =(operator *(v));}

uintBig uintBig::operator %(uintBig v)
{
    resize();
    v.resize();
    //qDebug()<<toString()<<"%"<<v.toString()<<"ou"<<toVector()<<"%"<<v.toVector();
    /*uintBig retour(valeur, baseDesCases);//faire le calcul un peut mieux...
    while(retour>=v)
        retour-=v;
    return retour;*/
    uintBig retour;
    if(v==1 || v==0)//ça donnera 0 (avec v==0 normalement c'est non défini mais c'est généralement %0.1)
        return retour;
    retour=operator -( (operator /(v))*v );
    while(retour.isNegative())
        retour+=v;//si il est encore négatif, on le remonte d'un (ex:-9%4)
    retour.resize();
    //qDebug()<<"="<<retour.toString()<<"ou"<<retour.toVector();
    return retour;
}

void uintBig::operator %=(const uintBig &v) {operator =(operator %(v));}

uintBig uintBig::operator ^(uintBig v) const
{
    uintBig retour(1);
    uintBig retour2(valeur, baseDesCases);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}

uintBig uintBig::operator ^(quint64 v) const
{
    uintBig retour(1);
    uintBig retour2(valeur, baseDesCases);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}

bool uintBig::operator <(uintBig v)
{
    resize();
    v.resize();
    if(negatif && !v.isNegative())
        return true;
    if(!negatif && v.isNegative())
        return false;
    if(negatif && v.isNegative())//si ils sont tous les 2 negatifs
    {//on inverse par rapport à l'autre
        if(size() != v.size())
            return !(size() < v.size());
        for(quint64 i=size(); i>0; i--)
        {
            if(valeur.at(i-1) != v.at(i-1))
                return !(valeur.at(i-1) < v.at(i-1));
        }
        return false;//est egal
    }
    else
    {
        if(size() != v.size())
            return size() < v.size();
        for(quint64 i=size(); i>0; i--)
        {
            if(valeur.at(i-1) != v.at(i-1))
                return valeur.at(i-1) < v.at(i-1);
        }
    }
    return false;//est egal
}

bool uintBig::operator <=(uintBig v) {return operator <(v) || operator ==(v);}

void uintBig::operator =(const uintBig &v)
{
    valeur=v.toVector(baseDesCases);
    negatif=v.isNegative();
    resize();
}

void uintBig::operator =(const QVector<quint64> v)
{
    valeur=v;
    negatif=false;
    resize();
}

bool uintBig::operator ==(uintBig v)
{
    resize();
    v.resize();
    if(negatif!=v.isNegative())
        return false;
    if(size() != v.size())
        return false;
    for(quint64 i=0; i<size(); i++)
    {
        if(valeur.at(i) != v.at(i))
            return false;
    }
    return true;
}

bool uintBig::operator ==(const uintBig &v) const
{//comme au dessus mais sans verif de resize()
    if(negatif!=v.isNegative())
        return false;
    if(size() != v.size())
        return false;
    for(quint64 i=0; i<size(); i++)
    {
        if(valeur.at(i) != v.at(i))
            return false;
    }
    return true;
}

bool uintBig::operator >(uintBig v)
{
    resize();
    v.resize();
    if(negatif && !v.isNegative())
        return false;
    if(!negatif && v.isNegative())
        return true;
    if(negatif && v.isNegative())//si ils sont tous les 2 negatifs
    {//on inverse par rapport à l'autre
        if(size() != v.size())
            return !(size() > v.size());
        for(quint64 i=size(); i>0; i--)
        {
            if(valeur.at(i-1) != v.at(i-1))
                return !(valeur.at(i-1) > v.at(i-1));
        }
    }
    else
    {
        if(size() != v.size())
            return size() > v.size();
        for(quint64 i=size(); i>0; i--)
        {
            if(valeur.at(i-1) != v.at(i-1))
                return valeur.at(i-1) > v.at(i-1);
        }
    }
    return false;//est egal
}

bool uintBig::operator >=(const uintBig &v) {return operator >(v) || operator ==(v);}

quint64 &uintBig::operator[](const quint64 v)
{
    quint64 retourNul=0;//passer en variable sinon bug
    if(v>=size())//si existe pas, c'est 0
        return retourNul;
    return valeur[v];
}

const quint64 &uintBig::at(const quint64 v) const
{
    quint64 retourNul=0;//passer en variable sinon bug
    if(v>=size())//si existe pas, c'est 0
        return retourNul;
    return valeur.at(v);
}

bool uintBig::isEmpty() const
{
    for(quint64 i=0; i<size(); i++)
        if(valeur.at(i) != 0)
            return false;
    return true;
}

uintBig uintBig::sqrt()
{
    resize();
    if(isNegative())
    {
        qDebug("erreur sqrt, racine d'un nombre négatif");
        return 0;
    }
    if(isEmpty() || isNegative())
        return 0;//0²=0
    uintBig retour(1);
    if(operator ==(retour))
        return 1;//1²=1

    uintBig min=0;
    retour=2;//commencer par la
    while((retour^2) < *this)//modules pour affiner la valeur (augmentation par ²)
    {
        min=retour;
        retour=retour*retour;
    }

    retour=min;
    while((retour^2) < *this)//augmentation par *2
    {
        min=retour;
        retour=retour*2;
    }

    retour=min;
    while((retour^2) < *this)//augmentation par +2^31
    {
        min=retour;
        retour=retour+baseDesCases;
    }
    retour=min;

    while((retour^2) < *this)//augmentation par +128
    {
        min=retour;
        retour=retour+128;
    }

    retour=min;
    while((retour^2) < *this)//augmentation par +1
    {
        min=retour;
        retour=retour+1;
    }
    //ici retour² est == ou au dessus de this
    if((retour^2) == *this)//si c'est lui, on l'envoie
    {
        return retour;
    }
    else//sinon c'est que c'est légerement plus bas
        return retour-1;//on envoie donc à l'entier inférieur
}

const bool &uintBig::isNegative() const{return negatif;}

void uintBig::setNegative(bool negative) {negatif=negative;}

void uintBig::resize()
{
    while(valeur.size()<64)//pas bien cadré
        valeur.append(0);
    if(valeur.size()>64)
        qDebug(QString("un unintBig fait plus de 64 cases ("+QString::number(valeur.size())+")").toStdString().c_str());

    //actualise les cases
    quint64 retenue=0;
    for(quint64 i=0; i<(quint64)valeur.size(); i++)
    {
        valeur[i]+=retenue;
        retenue = valeur.at(i)/baseDesCases;
        valeur[i] %= baseDesCases;
    }
}

const QString uintBig::strOfBase(quint64 base)
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
