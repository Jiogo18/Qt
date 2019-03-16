#ifndef INFORMATION_H
#define INFORMATION_H

#include <QtWidgets>
#include "ui_information.h"
#include <QString>
#include <QFile>

class information : public QDialog, private Ui::information
{
    Q_OBJECT

    public:
        information();

    private slots:
        void actuVersion();


};

#endif // INFORMATION_H
