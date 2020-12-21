//  /!\ ce fichier contient des debugs uniquement pour le test, vous risquez d'etre spammé
#include "intBig4.h"
#include "debug.h"

intBig4::intBig4(quint64 value) : intBig4Parent(baseP, value)
{}

intBig4::intBig4(QString value, quint64 base) : intBig4Parent(value, base)
{setBase(baseP);}

intBig4::intBig4(QVector<quint64> value, quint64 base) : intBig4Parent(value, base)
{setBase(baseP);}

intBig4::intBig4(const intBig4 &value) : intBig4Parent(value.toVector(baseP), baseP)
{
    negatifP=value.isNegative();
    resize();
}

intBig4::intBig4(const intBig4Parent &value) : intBig4Parent(value.toBase(baseP))
{
    negatifP=value.isNegative();
    resize();
}
