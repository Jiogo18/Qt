#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "GraphicScene3D.h"


class fenetre : public QWidget
{
public:
    fenetre(QWidget *parent=0);
    void bRefreshPress();
    //update quand la fenetre change de taille
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void refreshCoo();
    void avanceX(double speed);
    void avanceY(double speed);
    void avanceZ(double speed);
    void plusX(double x);
    void plusY(double y);
    void plusZ(double z);

private:
    QHBoxLayout *layout;
    GraphicScene3D *scene;
    QLabel *lblScene;
    QFormLayout *layoutOption;
    QDoubleSpinBox *sX;
    QDoubleSpinBox *sY;
    QDoubleSpinBox *sZ;
    QSpinBox *sRX;
    QSpinBox *sRY;
    QDoubleSpinBox *sZoom;
    QDoubleSpinBox *sPerspective;
    QPushButton *bRefresh;
    QTimer *timerRefresh;
    QList<int> listeKey;
    QTimer *timerKey;
    bool changement;
    QPoint cooMouseLast;
};

#endif // FENETRE_H
