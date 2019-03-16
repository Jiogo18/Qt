#include "debug.h"

void debug::d(QString str, bool important)
{
    findFenetre()->debug(str, important);
}

fenetre *debug::findFenetre()
{
    QWidgetList l = QApplication::allWidgets();
    int i=0;
    while(i<l.size())
    {
        if(l.at(i)->accessibleName()=="fenetre")
            return (fenetre*)l.at(i);
        i++;
    }
    qDebug("\"fenetre\" pas trouvé");
    return 0;
}

void debug::stat(QString type, quint64 start, quint64 stop)
{
    findFenetre()->addStat(type, start, stop);
}
