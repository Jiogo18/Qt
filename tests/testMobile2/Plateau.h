#ifndef PLATEAU_H
#define PLATEAU_H

#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include "Voiture.h"
#include "Button.h"
#include <QList>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QString>
#include "QmlItem.h"

class Plateau : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Plateau(const Plateau &p);
    Plateau(QQuickItem *parent=nullptr);
    Plateau(QQmlApplicationEngine *engine);
    Plateau &operator=(const Plateau &p);
    ~Plateau();
    void paint(QPainter *painter);
    template<typename T> T *createItem(QString name)
    {
        return QmlItem::createItem<T>(m_engine, name);
    }



private:
    QList<Button*> *m_listButton=nullptr;
    QQmlApplicationEngine *m_engine=nullptr;
    Voiture *m_Voit=nullptr;
    Button *m_buttonLeft=nullptr;
    Button *m_buttonRight=nullptr;
    Button *m_buttonUp=nullptr;
    Button *m_buttonDown=nullptr;
};

#endif // PLATEAU_H
