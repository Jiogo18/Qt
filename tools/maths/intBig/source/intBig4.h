#ifndef INTBIG4_H
#define INTBIG4_H

#include "intBig4Parent.h"
#include <QtMath>

class intBig4 : public intBig4Parent
{
    public:
        intBig4(quint64 value=0);
        explicit intBig4(QString value, quint64 base=10);
        intBig4(QVector<quint64> value, quint64 base);
        intBig4(const intBig4 &value);
        intBig4(const intBig4Parent &value);
        //static const quint64 baseP=pow(2,31);//max des cases = baseP-1
        static const quint64 baseP=2147483648;//2^31
};

#endif // INTBIG4_H
