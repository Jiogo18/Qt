#ifndef RSA_H
#define RSA_H
//importé le 10/03/2019 à 12:17

#include <qmath.h>
#include <QMap>
#include <QDateTime>
#include <QRandomGenerator>
#include "uintBig.h"
#include <QtWidgets>
#include "fenetre.h"

class RSA : public QObject
{
    Q_OBJECT
public:
    RSA(fenetre *parent);
    uintBig chiffrer1(uintBig msg, uintBig d_e, uintBig n, QProgressBar *ch=new QProgressBar);

};

#endif // RSA_H
