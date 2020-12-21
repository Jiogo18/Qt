#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include "Screenshot.h"
#include <QTimer>
#include <QRgb>
#include "MC_Gui.h"

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

    public:
        explicit fenetre(QWidget *parent = 0);
        ~fenetre();
        void actuSec();
        void actuCooMouse();
        void saveCase();
        void reconnaissance1(QImage imgScreen);
        void saveInfoBulle();
        void saveGui();

    private:
        Ui::fenetre *ui;
        QTimer TActuSec;
        QTimer TActuCooMouse;
};

#endif // FENETRE_H
