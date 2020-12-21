#ifndef CONVERTISSEURBASES_H
#define CONVERTISSEURBASES_H

#include <QString>
#include <QVector>
#include "intBig.h"
#include <QtMath>

class convertisseurBases
{
    public:
        convertisseurBases();
        static QVector<quint64> CustomToCustom(QVector<quint64> value, quint64 base1, quint64 base2);
        static QVector<quint64> DecToCustom(QVector<quint64> value, quint64 baseCustom);
        static QVector<quint64> CustomToDec(QVector<quint64> value, quint64 baseCustom);
        static QVector<quint64> toVector(QString value, QString base="");
        static QVector<quint64> toVector(quint64 value, quint64 baseRetour=10);
        static QString toString(QVector<quint64> value);
        static bool isNull(QVector<quint64> value);
        static QVector<quint64> pow(quint64 a, quint64 b, quint64 base);
        static bool estPetitOuEgal(QVector<quint64> a, QVector<quint64> b);
        static bool estGrandOuEgal(QVector<quint64> a, QVector<quint64> b);
        static QVector<quint64> moins(QVector<quint64> a, QVector<quint64> b, quint64 base);
        static QVector<quint64> plus(QVector<quint64> a, QVector<quint64> b, quint64 base);
        static QVector<quint64> fois(QVector<quint64> a, QVector<quint64> b, quint64 base);
};

#endif // CONVERTISSEURBASES_H
