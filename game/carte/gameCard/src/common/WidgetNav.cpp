#include "WidgetNav.h"
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include "src/common/widget/Chat.h"


QWidget *WidgetNav::find(QString name)
{
    QWidgetList l = QApplication::allWidgets();
    int i=0;
    while(i<l.size())
    {
        if(l.at(i)->accessibleName()==name)
            return l.at(i);
        i++;
    }
    qWarning() << name << "pas trouvé";
    return nullptr;
}

template<typename T>
T *WidgetNav::find(QString name)
{
    QWidget *w = WidgetNav::find(name);
    if(w == nullptr)
        return nullptr;
    return (T*)w;
}


void WidgetNavRegister()
{
    WidgetNav::find<Chat>("Chat");
}
