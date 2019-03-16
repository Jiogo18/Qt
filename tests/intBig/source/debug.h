#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QApplication>
#include "fenetre.h"
#include <QtWidgets>

class debug
{
public:
    static void d(QString str, bool important=false);
    static fenetre *findFenetre();
    static void stat(QString type, quint64 start, quint64 stop);
};

#endif // DEBUG_H
