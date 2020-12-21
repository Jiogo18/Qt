#ifndef INTBIG5PARENT_H
#define INTBIG5PARENT_H

#include <QVector>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QProgressBar>

class intBig5Parent
{
    public:
        intBig5Parent(quint64 value=0);
        intBig5Parent(quint64 baseV, quint64 value);
        intBig5Parent(QString value, quint64 base);
        intBig5Parent(QVector<quint64> value, quint64 base);
        intBig5Parent(const intBig5Parent &value);
        intBig5Parent(const intBig5Parent *value);
        void resize();

        static intBig5Parent fromText(QString value, quint64 base=10);
        static intBig5Parent fromVector(QVector<quint64> value, quint64 base);

        void setValue(QVector<quint64> value, quint64 base);
        void setBase(quint64 base);
        void setNegative(bool negative);

        QString toString(quint64 base=10) const;
        QVector<quint64> toVector(quint64 base) const;
        intBig5Parent toBase(quint64 base) const;
        bool isEmpty() const;

        inline quint64 size() const {return (quint64)valeurP.size();}
        quint64 sizeNNul() const;
        inline quint64 base() const {return baseP;}
        inline bool isNegative() const {return negatifP;}

        const quint64 &at(const quint64 i) const;
        quint64 &operator[](const quint64 i);//si c pas ds la case, retour 0
        void operator ++(int);//int pas utile normalement
        void operator --(int);//int pas utile normalement
        intBig5Parent sqrt() const;
        bool isPrime(QProgressBar *ch=nullptr) const;

        intBig5Parent operator ^(intBig5Parent v) const;
        intBig5Parent operator ^(quint64 v) const;
        intBig5Parent operator %(intBig5Parent v) const;
        intBig5Parent operator *(const intBig5Parent &v) const;
        intBig5Parent operator *(quint64 v) const;
        intBig5Parent operator /(const intBig5Parent &v) const;//pas fait
        intBig5Parent operator /(quint64 v) const;
        intBig5Parent operator +(intBig5Parent v) const;
        intBig5Parent operator +(quint64 v) const;
        intBig5Parent operator -(intBig5Parent v) const;
        intBig5Parent operator -(quint64 v) const;

        void operator %=(const intBig5Parent &v);
        void operator %=(quint64 v);
        void operator *=(const intBig5Parent &v);
        void operator *=(quint64 v);
        void operator /=(const intBig5Parent &v);
        void operator /=(quint64 v);
        void operator +=(const intBig5Parent &v);
        void operator +=(quint64 v);
        void operator -=(const intBig5Parent &v);
        void operator -=(quint64 v);

        bool operator !=(const intBig5Parent &v) const;
        bool operator !=(quint64 v) const;
        bool operator ==(const intBig5Parent &v) const;
        bool operator ==(quint64 v) const;
        bool operator <(const intBig5Parent &v) const;
        bool operator <(quint64 v) const;
        bool operator <=(const intBig5Parent &v) const;
        bool operator <=(quint64 v) const;
        bool operator >(const intBig5Parent &v) const;
        bool operator >(quint64 v) const;
        bool operator >=(const intBig5Parent &v) const;
        bool operator >=(quint64 v) const;

        void operator =(const intBig5Parent &v);
        void operator =(quint64 v);


        static const QString strOfBase(quint64 base);
        QString debug() const;
    protected:
        QVector<quint64> valeurP;
        bool negatifP;
        quint64 baseP;
        const quint64 retourNul=0;

};

#endif // INTBIG5PARENT_H
