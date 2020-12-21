#ifndef MC_GUI_H
#define MC_GUI_H

#include <QString>
#include <QImage>

class MC_Gui
{
    public:
        MC_Gui(QString fileContainers);

        void cutContainers();

    public:
        QString file;
};

#endif // MC_GUI_H
