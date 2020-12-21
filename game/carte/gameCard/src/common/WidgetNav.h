#ifndef WIDGETNAV_H
#define WIDGETNAV_H

#include <QString>
#include <QWidget>

namespace WidgetNav
{
    QWidget *find(QString name);
    template<typename T> T *find(QString name);
};

#endif // WIDGETNAV_H
