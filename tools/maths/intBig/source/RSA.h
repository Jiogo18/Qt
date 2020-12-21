#ifndef RSA_H
#define RSA_H
//importé le 10/03/2019 à 12:17

#include <qmath.h>
#include <QMap>
#include <QDateTime>
#include <QRandomGenerator>
#include <QtWidgets>
#include "fenetre.h"
#include "intBig4.h"
#include "intBig5.h"
#include "intBig6.h"

class RSA : public QObject
{
    Q_OBJECT
public:
    RSA(fenetre *parent);
    intBig4 chiffrer4(intBig4 msg, intBig4 d_e, intBig4 n, QProgressBar *ch=new QProgressBar);
    intBig5 chiffrer5(intBig5 msg, intBig5 d_e, intBig5 n, QProgressBar *ch=new QProgressBar);
    intBig6 chiffrer6(intBig6 msg, intBig6 d_e, intBig6 n, QProgressBar *ch=new QProgressBar);

};

#endif // RSA_H
