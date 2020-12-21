#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include "ComCpp.h"
#include "Voiture.h"
#include "Button.h"
#include "Plateau.h"

#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QDebug>


/*
 *
 * In a object don't call QQuickPaintedItem::mouseXXXEvent(event);
 * https://stackoverflow.com/questions/29772736/qquickpainteditem-mousereleaseevent-gets-only-called-after-doubleclick
 * For QQuickPaintedItem l'event n'est pas utile et meme, il fait tout bugger !
 *
 */


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<Voiture>("Car", 1, 0, "Voiture");//import Car 1.0 avec l'objectName "Voiture"
    qmlRegisterType<Button>("myLib", 1, 0, "ButtonM");
    qmlRegisterType<Plateau>("myLib", 1, 0, "Plateau");

    QQmlApplicationEngine engine;
    ComCpp compcpp;//fonctions externes
    engine.rootContext()->setContextProperty("compcpp", &compcpp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    if (!window) {
        qFatal("Error: Your root item has to be a window.");
        return -1;
    }
    window->show();//peut etre inutile, mais garde au cas ou

    Plateau plateau(&engine);
    QString *str1 = new QString("str1");
    QString *str2 = str1;
    *str2 = "hello";
    qDebug()<<"main"<<str1<<*str1<<"   "<<str2<<*str2;



    return app.exec();
}
