#include "ComCpp.h"
#include <QTime>

ComCpp::ComCpp(QObject *parent) : QObject(parent)
{

}

QString ComCpp::txt(){
    QString r = "Click on QML Button\n("+QTime::currentTime().toString()+")";
    return r;
}
