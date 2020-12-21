#ifndef VIEWPICTURE_H
#define VIEWPICTURE_H

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Ui {
class viewPicture;
}

class viewPicture : public QWidget
{
    Q_OBJECT

public:
    explicit viewPicture(QWidget *parent = 0);
    ~viewPicture();
    void setPicture(QString file);

private:
    Ui::viewPicture *ui;
};

#endif // VIEWPICTURE_H
