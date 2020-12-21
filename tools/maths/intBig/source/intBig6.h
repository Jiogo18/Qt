#ifndef INTBIG6_H
#define INTBIG6_H

#include <QList>
#include <QtMath>
#include <QDebug>

class intBig6
{
public:
    intBig6();
    intBig6(qint64 v);
    intBig6(const intBig6 &ib);

    intBig6 *operator =(qint64 v);
    intBig6 *operator =(const intBig6 &ib);

    intBig6 *operator +=(qint64 v);
    intBig6 *operator -=(qint64 v);
    intBig6 *operator *=(qint64 v) { return operator =(operator *(v)); }
    intBig6 *operator /=(qint64 v) { return operator =(operator /(v)); }
    intBig6 *operator %=(qint64 v);
    intBig6 operator +(qint64 v) const { intBig6 retour; retour += v; return retour; }
    intBig6 operator -(qint64 v) const { intBig6 retour; retour -= v; return retour; }
    intBig6 operator *(qint64 v) const;
    intBig6 operator /(qint64 v) const;
    intBig6 operator %(qint64 v) const { intBig6 retour; retour %= v; return retour; }

    intBig6 *operator +=(const intBig6 &ib);
    intBig6 *operator -=(const intBig6 &ib);
    intBig6 *operator *=(const intBig6 &ib) { return operator =(operator *(ib)); }
    intBig6 *operator /=(const intBig6 &ib) { return operator =(operator /(ib)); }
    intBig6 *operator %=(const intBig6 &ib);
    intBig6 operator +(const intBig6 &ib) const { intBig6 retour(*this); retour += ib; return retour; }
    intBig6 operator -(const intBig6 &ib) const { intBig6 retour(*this); retour -= ib; return retour; }
    intBig6 operator *(const intBig6 &ib) const;
    intBig6 operator /(intBig6 denominateur) const;
    intBig6 operator %(const intBig6 &ib) const { intBig6 retour(*this); retour %= ib; return retour; }

    bool operator <(const intBig6 &ib) const;
    bool operator >(const intBig6 &ib) const;
    bool operator ==(const intBig6 &ib) const;
    bool operator !=(const intBig6 &ib) const { return !operator ==(ib); }
    bool operator <=(const intBig6 &ib) const { return operator <(ib) || operator ==(ib); }
    bool operator >=(const intBig6 &ib) const { return operator >(ib) || operator ==(ib); }

    intBig6 operator ^(qint64 v) const;

    bool isEmpty() const;

protected:
    static const qint64 base = 2147483648;//2^31
    QList<qint64> value;
    bool retenue = false;//si retenue négative sur la derniere case
    void addAt(int i, qint64 v);
private:
    void setAt(int i, qint64 v);
    void reduceValue();

    friend QDebug operator <<(QDebug debug, const intBig6 &ib);
};

class intBig6B : public intBig6 {
    //obj:
    //intBig6 => string en base spé
    //string en base spé => intBig6
public:
    intBig6B(const intBig6 &ib);
    intBig6B(QString v, qint64 base);

    QString toString(qint64 base = 10) const;
private:
    static QList<qint64> toBase(QList<qint64> v, qint64 baseFrom, qint64 baseTo);
    static QString valueOfBase(qint64 v, qint64 base);
    static qint64 valueOfBase(QString v, qint64 base);
};

QDebug operator <<(QDebug debug, const intBig6 &ib);



#endif // INTBIG6_H
