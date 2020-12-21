#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include "registre.h"
#include <QFileDialog>

#include <QDebug>

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = nullptr);
    ~fenetre();

    //split tab
    void onBSplitOpen();
    void onBSplit();
    void onBSplitAndNext();

private:
    Ui::fenetre *ui;
    registre reg;
};

#endif // FENETRE_H
