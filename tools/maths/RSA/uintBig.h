#ifndef UINTBIG_H
#define UINTBIG_H

#include <QVector>
#include <QString>
#include "convertisseurBases.h"
#include <QThread>
#include <QDateTime>
#include <QDebug>

class uintBig
{
    public:
        uintBig(quint64 value=0);
        uintBig(QString value, quint64 base);
        uintBig(QVector<quint64> value, quint64 base);
        //uintBig(const uintBig &value);

        QString toString(quint64 base=10) const;
        QVector<quint64> toVector(quint64 base=2147483648) const;
        static uintBig fromText(QString value, quint64 base=10);

        inline quint64 size() const {return (quint64)valeur.size();}
        bool operator !=(const uintBig &v);
        uintBig operator +(uintBig v);
        void operator +=(const uintBig &v);
        void operator ++(int);//int pas utile normalement
        uintBig operator -(uintBig v);
        uintBig operator -(quint64 v);
        void operator -=(const uintBig &v);
        void operator --(int);//int pas utile normalement
        uintBig operator /(const uintBig &v) const;//pas fait
        uintBig operator /(quint64 v) const;
        void operator /=(const uintBig &v);
        void operator /=(quint64 v);
        uintBig operator *(const uintBig &v) const;
        void operator *=(const uintBig &v);
        uintBig operator %(uintBig v);
        void operator %=(const uintBig &v);
        uintBig operator ^(uintBig v) const;
        uintBig operator ^(quint64 v) const;
        bool operator <(uintBig v);
        bool operator <=(uintBig v);
        void operator =(const uintBig &v);
        void operator =(const QVector<quint64> v);
        bool operator ==(uintBig v);
        bool operator ==(const uintBig &v) const;
        bool operator >(uintBig v);
        bool operator >=(const uintBig &v);
        quint64 &operator[](const quint64 v);//si c pas ds la case, retour 0
        const quint64 &at(const quint64 v) const;
        bool isEmpty() const;
        uintBig sqrt();
        const bool &isNegative() const;
        void setNegative(bool negative);
        void resize();

        static const QString strOfBase(quint64 base);
        static const quint64 baseDesCases=pow(2,31);//max des cases = baseDesCases-1

    private:
        QVector<quint64> valeur;
        bool negatif;

};

#endif // UINTBIG_H
