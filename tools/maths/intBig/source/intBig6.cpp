#include "intBig6.h"
#include "debug.h"

intBig6::intBig6() { operator =(0); }
intBig6::intBig6(qint64 v) { operator =(v); }
intBig6::intBig6(const intBig6 &ib) { operator =(ib); }


intBig6 *intBig6::operator =(qint64 v) { value = {}; addAt(0, v); return this; }
intBig6 *intBig6::operator =(const intBig6 &ib)
{ value = ib.value; retenue = ib.retenue; return this; }

intBig6 *intBig6::operator +=(qint64 v) { addAt(0, v); reduceValue(); return this; }
intBig6 *intBig6::operator -=(qint64 v) { addAt(0, -v); reduceValue(); return this; }
intBig6 *intBig6::operator %=(qint64 v) { return operator -=((*this/v) * v); }
intBig6 intBig6::operator *(qint64 v) const
{
    if(v == 0)
        return 0;
    intBig6 retour;
    for(int i=0; i<value.size(); i++)
        retour.addAt(i, value.at(i) * v);
    return retour;
}
intBig6 intBig6::operator /(qint64 v) const
{
    intBig6 retour;
    if(v == base) {//optimisé
        QList<qint64> v2 = value;
        v2.removeFirst();
        retour.value = v2;
        return retour;
    }
    qint64 currentV = 0;
    for(int i=value.size()-1; i>=0; i--) {
        currentV += value.at(i);
        retour.addAt(i, currentV / v);
        currentV %= v;
        currentV *= base;
    }
    retour.reduceValue();
    return retour;
}

intBig6 *intBig6::operator +=(const intBig6 &ib)
{
    for(int i=0; i < ib.value.size(); i++)
        addAt(i, ib.value.at(i));
    reduceValue();
    return this;
}
intBig6 *intBig6::operator -=(const intBig6 &ib)
{
    for(int i=0; i < ib.value.size(); i++)
        addAt(i, -ib.value.at(i));
    reduceValue();
    return this;
}
intBig6 *intBig6::operator %=(const intBig6 &ib)
{
    qint64 start = debug::time();
    operator -=((operator /(ib)) * ib);
    debug::stat("%=intBig", start, debug::time());
    return this;
}
intBig6 intBig6::operator *(const intBig6 &ib) const
{
    intBig6 retour;
    for(int i=0; i < ib.value.size(); i++)
        for(int i2=0; i2 < value.size(); i2++)
            retour.addAt(i+i2, ib.value.at(i) * value.at(i2));
    retour.reduceValue();
    return retour;
}
intBig6 intBig6::operator /(intBig6 denominateur) const
{
    qint64 start = debug::time();
    if(operator <(denominateur))
        return 0;
    if(denominateur == 1)
        return *this;
    intBig6 mult;
    intBig6 numerateur(*this);
    int exp = 0;
    int value_coef = 8;
    //(7<vc<15) obj: etre <170 voir <160 (moyenne:165)
    //min: 10:150 9:153 8:148 7:156 6:157
    //=> le plus opti est un base 8 (peut etre car *2)
    intBig6 denominateur2(denominateur*value_coef);
    while(numerateur >= denominateur2) {
        denominateur = denominateur2;
        denominateur2 *= value_coef;
        exp++;
    }
    //division
    while(exp >= 0) {
        int current_mult = 0;
        intBig6 denominateur2(denominateur);
        while(denominateur2 <= numerateur) {
            denominateur2 += denominateur;
            current_mult++;
        }
        numerateur -= denominateur * current_mult;//il y a pas plus opti
        mult *= value_coef;//on décale
        mult.addAt(0, current_mult);
        denominateur /= value_coef;
        exp--;
    }
    debug::stat("/intBig", start, debug::time());
    return mult;
}

bool intBig6::operator <(const intBig6 &ib) const
{
    if(retenue && ib.retenue) {//tous les 2 -
        if(value.size() != ib.value.size())
            return value.size() > ib.value.size();
    }
    else if(retenue || ib.retenue) {//l'un des 2 -
        return retenue;//this < 0 < ib sinon c'est l'inverse
    }
    else {//tous les 2 +
        if(value.size() != ib.value.size())
            return value.size() < ib.value.size();
    }
    for(int i=value.size()-1; i>=0; i--)
        if(value.at(i) != ib.value.at(i))
            return value.at(i) < ib.value.at(i);
    return false;//c'est ==

}
bool intBig6::operator >(const intBig6 &ib) const
{
    if(retenue && ib.retenue) {//tous les 2 -
        if(value.size() != ib.value.size())
            return value.size() < ib.value.size();
    }
    else if(retenue || ib.retenue) {//l'un des 2 -
        return !retenue;//this > 0 > ib sinon c'est l'inverse
    }
    else {//tous les 2 +
        if(value.size() != ib.value.size())
            return value.size() > ib.value.size();
    }
    for(int i=value.size()-1; i>=0; i--)
        if(value.at(i) != ib.value.at(i))
            return value.at(i) > ib.value.at(i);
    return false;//c'est ==
}
bool intBig6::operator ==(const intBig6 &ib) const
{
    if(isEmpty() && ib.isEmpty())
        return true;
    if(value.size() != ib.value.size())
        return false;
    for(int i=value.size()-1; i>=0; i--)
        if(value.at(i) != ib.value.at(i))
            return false;
    return true;
}

intBig6 intBig6::operator ^(qint64 v) const
{
    intBig6 retour(1);
    for(int i=0; i<v; i++)
        retour *= *this;
    return retour;
}

bool intBig6::isEmpty() const
{
    for(int i=0; i<value.size(); i++)
        if(value.at(i) != 0)
            return false;
    return true;
}



void intBig6::addAt(int i, qint64 v)
{
    if(value.size() <= i) {
        if(retenue) {
            while(value.at(value.size()-1) < 0) {
                v -= 1;
                value[value.size()-1] += base;
            }
            retenue = false;
        }
        while(value.size() <= i)
            value.insert(i, 0);
            //n'insère pas jusqu'à la case...
    }

    value[i] += v;
    if(base <= value[i]) {
        addAt(i+1, qFloor(value[i] / base));
        value[i] %= base;
    }
    if(value[i] < 0) {
        if(value.size()-1 != i || value[i] <= -base) {
            addAt(i+1, value[i] / base - 1);
            value[i] -= base * (value[i] / base - 1);
        }
        else
            retenue = true;
    }
}
void intBig6::setAt(int i, qint64 v)
{
    qint64 start = debug::time();
    if(value.size() <= i) {
        if(retenue) {
            while(value.at(value.size()-1) < 0) {
                v -= 1;
                value[value.size()-1] += base;
            }
            retenue = false;
        }
        while(value.size() <= i)
            value.insert(i, 0);
    }
    value[i] = 0;
    addAt(i, v);
    debug::stat("setAt", start, debug::time());
}
void intBig6::reduceValue()
{
    while(!value.isEmpty() && value.last() == 0)
        value.removeLast();
}



intBig6B::intBig6B(const intBig6 &ib) : intBig6(ib) {}
intBig6B::intBig6B(QString v, qint64 base) : intBig6(0)
{
    QList<qint64> retour = {};
    bool negative = v.startsWith("-");
    if(negative)
        v.remove("-");
    if(base==10 || base==16 || base==36) {
        while(!v.isEmpty()) {
            retour.push_back(valueOfBase(QString(v.back()), base));
            v.remove(v.size()-1, 1);
        }
    }
    else {
        QString v2 = "";
        while(!v.isEmpty()) {
            while(!v.isEmpty() && v.back() != '|') {
                v2 += v.back();
                v.remove(v.size()-1, 1);
            }
            if(!v.isEmpty())//le |
                v.remove(v.size()-1, 1);
            retour.push_back(valueOfBase(v2, base));
        }
    }
    retour = toBase(retour, base, intBig6::base);
    if(negative) {
        if(retour.size() > 0 && retour.back() == 1)
            retour.back() = -1;
        else
            retour.push_back(-1);
    }
    value = retour;
}

QString intBig6B::toString(qint64 base) const
{
    QList<qint64> withBase = toBase(value, intBig6::base, base);
    QString d="";
    if(value.size() > 0 && value.back() < 0) {
        d = "-";//negative
        qint64 retenue = 0;
        int baseSize = withBase.size();
        for(int i=0; i<withBase.size() || retenue != 0; i++) {
            if(withBase.size() <= i)
                withBase.insert(i, 0);
            else
                withBase[i] *= -1;
            withBase[i] += retenue;
            if(withBase.at(i) < 0) {
                withBase[i] += base;
                retenue--;
                if(i + 100 > baseSize) {//bug on a ajouté trop de cases
                    qDebug() << "intBig6B::toString ERROR it is a positive array ?";
                    break;
                }
            }
        }
    }
    for(int i=withBase.size()-1; i>=0; i--) {
        if(i != withBase.size()-1 && base > 10 && !QList<int>({16,36}).contains(base))
            d += "|";
        d += valueOfBase(withBase.at(i), base);
    }
    return d;
}
QList<qint64> intBig6B::toBase(QList<qint64> v, qint64 baseFrom, qint64 baseTo)
{
    QList<qint64> retour = {};
    if(v.isEmpty())
        return {};
    if(baseFrom == baseTo)//si c les meme base
        return v;
    for(qint64 i=v.size()-1; i>=0; i--) {

        qint64 baseCase = qPow(baseFrom, i);//TODO: à terme il faudrait une liste
        QList<qint64> retourPlus = {};
        int i2=0;
        while(baseCase > 0) {
            while(retourPlus.size() <= i2)
                retourPlus.insert(i2, 0);
            retourPlus[i2] += baseCase;
            baseCase = retourPlus[i2] / baseTo;
            retourPlus[i2] = retourPlus[i2] % baseTo;
            i2++;
        }
        //on fait: intBig6B baseCase(intBig6B(intBig6::base, base)^i);
        //on fait: retour += pow(baseintBig6::base, ) * value.at(i);
        qint64 retenue = 0;
        for(int i2=0; i2<retourPlus.size() || retenue != 0; i2++) {
            while(retour.size() <= i2)
                retour.insert(i2, 0);
            if(i2 < retourPlus.size())
                retour[i2] += retourPlus.at(i2) * v.at(i);
            retour[i2] += retenue;
            retenue = retour[i2] / baseTo;
            retour[i2] = retour[i2] % baseTo;

        }
    }

    return retour;
}

QString intBig6B::valueOfBase(qint64 v, qint64 base)
{
    try {
        switch (base) {
        case 16:
            return QString("0123456789ABCDEF").at(v);
        case 36:
            return QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").at(v);
        }
    } catch(const std::exception &error) {}
    return QString::number(v);
}
qint64 intBig6B::valueOfBase(QString v, qint64 base)
{
    int retour = -1;
    try {
        switch (base) {
        case 16:
            retour = QString("0123456789ABCDEF").indexOf(v);
            break;
        case 36:
            retour = QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").indexOf(v);
            break;
        }
    } catch(const std::exception &error) {}
    if(retour != -1)
        return retour;
    return v.toLongLong();
}

QDebug operator <<(QDebug debug, const intBig6 &ib)
{
    debug << intBig6B(ib).toString().toStdString().c_str();
    return debug;
}

