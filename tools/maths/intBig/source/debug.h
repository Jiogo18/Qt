#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QApplication>
#include "fenetre.h"
#include <QtWidgets>
#include <QDateTime>

class debug
{
public:
    static void d(QString str, bool important=false);
    static fenetre *findFenetre();
    static void stat(QString type, quint64 start, quint64 stop);

    static qint64 time() { return QDateTime::currentMSecsSinceEpoch(); }
};

#endif // DEBUG_H
