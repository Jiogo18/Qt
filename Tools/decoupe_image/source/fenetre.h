#ifndef FENETRE_H
#define FENETRE_H

#include <QtWidgets>
#include "viewPicture.h"

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = 0);
    ~fenetre();

private:
    Ui::fenetre *ui;
    viewPicture *fenPict;
    void actuInput();
    void fenInput();
    void fenShow();
    void fenOuput();
    void startCut();
};

#endif // FENETRE_H
