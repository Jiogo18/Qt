//  /!\ ce fichier contient des debugs uniquement pour le test, vous risquez d'etre spammé
#include "intBig5.h"
#include "debug.h"

intBig5::intBig5(quint64 value) : intBig5Parent(baseP, value)
{}

intBig5::intBig5(QString value, quint64 base) : intBig5Parent(value, base)
{setBase(baseP);}

intBig5::intBig5(QVector<quint64> value, quint64 base) : intBig5Parent(value, base)
{setBase(baseP);}

intBig5::intBig5(const intBig5 &value) : intBig5Parent(value.toVector(baseP), baseP)
{
    negatifP=value.isNegative();
    resize();
}

intBig5::intBig5(const intBig5Parent &value) : intBig5Parent(value.toBase(baseP))
{
    negatifP=value.isNegative();
    resize();
}
