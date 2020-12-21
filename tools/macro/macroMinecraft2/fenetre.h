#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include "debug.h"
#include "minecraft.h"

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = nullptr);
    ~fenetre();

public slots:
    void setOpacity(int v);
    void setIcon(QString ico);
    void bStartClicked();
    void bStopClicked();
    void actuCooMouse();

private:
    Ui::fenetre *ui;
    debug *deb;
    minecraft *mc;
    QTimer tStopMC;
    QTimer tActuMouse;
};

#endif // FENETRE_H
