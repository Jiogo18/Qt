#ifndef RSA_H
#define RSA_H


#include <qmath.h>
#include <QMap>
#include <QDateTime>
#include <QRandomGenerator>
#include "intBig.h"
#include <QtWidgets>

class RSA : public QObject
{
public:
    RSA();
    static bool isPrimeNumber(const quint64 nb, QProgressBar *ch=new QProgressBar);
    static bool isPrimeNumber(intBig nb);
    static bool arePrime(intBig nb1, intBig b);
    static QMap<QString, QString> generer(QString nbPremier1, QString nbPremier2, QProgressBar *ch1=new QProgressBar, QProgressBar *ch2=new QProgressBar, QProgressBar *ch3=new QProgressBar);
    static void debug(QString str);
    static quint64 random64(quint32 min, quint32 max);
    static intBig InverseBModuloN(intBig b, intBig n);
    static intBig chiffrer(intBig msg, intBig d_e, intBig n, QProgressBar *ch=new QProgressBar);
};

#endif // RSA_H
