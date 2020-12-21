#include <QApplication>
#include "fenetre.h"
#include <QTimer>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre fen;
    fen.show();

    /*Sleep(500);
    Screenshot::shootScreen();

    QTimer close;
    close.setInterval(100);
    QObject::connect(&close, &QTimer::timeout, qApp, &QCoreApplication::quit);
    close.start();*/

    return app.exec();
}
