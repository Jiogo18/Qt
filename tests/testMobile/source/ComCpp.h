#ifndef COMCPP_H
#define COMCPP_H


#include <QString>
#include <QObject>


class ComCpp : public QObject  // IMPORTANT : pour être utilisable dans le code QML, votre classe doit dériver de QObject ou QQuickItem.
{
    Q_OBJECT
    public:
        explicit ComCpp(QObject *parent = 0);

    public slots:
        QString txt();
};

#endif // COMCPP_H
