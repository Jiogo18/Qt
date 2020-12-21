#ifndef INTBIG5_H
#define INTBIG5_H

#include "intBig5Parent.h"
#include <QtMath>

class intBig5 : public intBig5Parent
{
    public:
        intBig5(quint64 value=0);
        explicit intBig5(QString value, quint64 base=10);
        intBig5(QVector<quint64> value, quint64 base);
        intBig5(const intBig5 &value);
        intBig5(const intBig5Parent &value);
        static const quint64 baseP=pow(2,31);//max des cases = baseP-1
};

#endif // INTBIG5_H
