#ifndef INTBIGP_H
#define INTBIGP_H

#include <QVector>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QProgressBar>
#include <QCoreApplication>
class intBigP
{
    public:
        intBigP(quint64 value=0);
        intBigP(quint64 baseV, quint64 value);
        intBigP(QString value, quint64 base);
        intBigP(QVector<quint64> value, quint64 base);
        intBigP(const intBigP &value);
        intBigP(const intBigP *value);
        void resize();

        static intBigP fromText(QString value, quint64 base=10);
        static intBigP fromVector(QVector<quint64> value, quint64 base);

        void setValue(QVector<quint64> value, quint64 base);
        void setBase(quint64 base);
        void setNegative(bool negative);

        QString toString(quint64 base=10) const;
        QVector<quint64> toVector(quint64 base) const;
        intBigP toBase(quint64 base) const;
        bool isEmpty() const;

        inline quint64 size() const {return (quint64)valeurP.size();}
        quint64 sizeNNul() const;
        inline quint64 base() const {return baseP;}
        inline bool isNegative() const {return negatifP;}

        const quint64 &at(const quint64 i) const;
        quint64 &operator[](const quint64 i);//si c pas ds la case, retour 0
        void operator ++(int);//int pas utile normalement
        void operator --(int);//int pas utile normalement
        intBigP sqrt() const;
        bool isPrime(QProgressBar *ch=nullptr) const;

        intBigP operator ^(intBigP v) const;
        intBigP operator ^(quint64 v) const;
        intBigP operator %(intBigP v) const;
        intBigP operator *(const intBigP &v) const;
        intBigP operator *(quint64 v) const;
        intBigP operator /(const intBigP &v) const;//pas fait
        intBigP operator /(quint64 v) const;
        intBigP operator +(intBigP v) const;
        intBigP operator +(quint64 v) const;
        intBigP operator -(intBigP v) const;
        intBigP operator -(quint64 v) const;

        void operator %=(const intBigP &v);
        void operator %=(quint64 v);
        void operator *=(const intBigP &v);
        void operator *=(quint64 v);
        void operator /=(const intBigP &v);
        void operator /=(quint64 v);
        void operator +=(const intBigP &v);
        void operator +=(quint64 v);
        void operator -=(const intBigP &v);
        void operator -=(quint64 v);

        bool operator !=(const intBigP &v) const;
        bool operator !=(quint64 v) const;
        bool operator ==(const intBigP &v) const;
        bool operator ==(quint64 v) const;
        bool operator <(const intBigP &v) const;
        bool operator <(quint64 v) const;
        bool operator <=(const intBigP &v) const;
        bool operator <=(quint64 v) const;
        bool operator >(const intBigP &v) const;
        bool operator >(quint64 v) const;
        bool operator >=(const intBigP &v) const;
        bool operator >=(quint64 v) const;

        void operator =(const intBigP &v);
        void operator =(quint64 v);



        static const QString strOfBase(quint64 base);
        QString debug() const;
    protected:
        QVector<quint64> valeurP;
        bool negatifP;
        quint64 baseP;
        const quint64 retourNul=0;

};
#endif // INTBIGP_H


#ifndef INTBIG_H
#define INTBIG_H
class intBig : public intBigP
{
    public:
        intBig(quint64 value=0);
        explicit intBig(QString value, quint64 base=10);
        intBig(QVector<quint64> value, quint64 base);
        intBig(const intBig &value);
        intBig(const intBigP &value);
        static const quint64 baseP=2147483648;//max des cases = baseP-1 (2^31)
};
#endif // INTBIG_H
