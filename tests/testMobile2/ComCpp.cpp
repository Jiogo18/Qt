#include "ComCpp.h"
#include <QTime>

ComCpp::ComCpp(QObject *parent) : QObject(parent)
{}



QString ComCpp::txt(){
    QString r = "Click on QML Button\n("+QTime::currentTime().toString()+")";
    return r;
}

void ComCpp::actionButton(QString name)
{
    //qDebug()<<"button clicked:"<<name;
    if(name=="button_left")
    {
        qDebug()<<"button clicked :"<<"gauche";
        //turnCar(true);
    }
    else if(name=="button_right")
    {
        qDebug()<<"button clicked :"<<"droite";
        //turnCar(false);
    }
    else if(name=="button_forward")
    {
        qDebug()<<"button clicked :"<<"avance";
        //moveCar(true);
    }
    else if(name=="button_back")
    {
        qDebug()<<"button clicked :"<<"recule";
        //moveCar(false);
    }
}

