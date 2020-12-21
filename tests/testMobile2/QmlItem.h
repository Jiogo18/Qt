#ifndef QMLITEM_H
#define QMLITEM_H

#include <QString>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QQuickItem>

class QmlItem
{
public:
    template<typename T> static T *createItem(QQmlApplicationEngine *engine, QString name)
    {//on peut pas mettre de template dans les cpp
        if(engine == 0) return 0;
        QQmlComponent component(engine, QUrl("qrc:/"+name+".qml"));
        QQuickItem *object = qobject_cast<QQuickItem*>(component.create());
        if(object == 0) {
            qWarning() << "Error createItem:" << component.errorString();
            return 0;
        }
        QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
        QQuickWindow *window = qobject_cast<QQuickWindow*>(engine->rootObjects().at(0));
        object->setParentItem(window->contentItem());
        object->setParent(engine);

        T *object2 = qobject_cast<T*>(object);
        if(object2 == 0)
            qWarning() << "Error createItem, ce type d'objet n'est pas pris en compte";

        return object2;
    }
};

#endif // QMLITEM_H
