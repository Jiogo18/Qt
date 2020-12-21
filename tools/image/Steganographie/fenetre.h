#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QImage>
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

    void chiffrer();
    void dechiffrer();
    void saveImage(QImage out);
    QColor maskColor(QColor colorBack, QColor colorIn);
    QColor demaskColor(QColor colorIn);
    QColor msgColor(QColor color);//le reste selon la précision
    QColor noMsgColor(QColor color);//le reste selon la précision

    void debug(QString str);

private:
    Ui::fenetre *ui;
};

#endif // FENETRE_H
