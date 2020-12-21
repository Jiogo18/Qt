#ifndef COMCPP_H
#define COMCPP_H


#include <QString>
#include <QObject>
//#include <QQmlContext>
#include "Voiture.h"
#include "Button.h"
#include "Plateau.h"
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "QmlItem.h"
#include <QDebug>


class ComCpp : public QObject  // IMPORTANT : pour être utilisable dans le code QML, votre classe doit dériver de QObject ou QQuickItem.
{
    Q_OBJECT
public:
    explicit ComCpp(QObject *parent = 0);


public slots:
    QString txt();
    void actionButton(QString name);

private:
    Plateau *m_plateau;
};

#endif // COMCPP_H
