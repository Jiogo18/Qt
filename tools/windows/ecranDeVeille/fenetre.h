#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QString>
#include "toastIcon.h"
#include <QMessageBox>
#include <QCloseEvent>
#include "ecranDeVeille.h"
#include <QtMath>

namespace Ui { class fenetre; }

class fenetre : public QWidget
{
    Q_OBJECT
public:
    explicit fenetre(QWidget *parent = 0);
    ~fenetre();
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
    void show();
    void hide();

    void updateConfig();
    void updateNbVideo();

    void debug(QString str);
private:

    Ui::fenetre *ui=nullptr;
    toastIcon *toastI=nullptr;//toastIcon
    ecranDeVeille EDV;

    QTimer timerActu;
    EDVConfig lastConfig;
};

#endif // FENETRE_H
