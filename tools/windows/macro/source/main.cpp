#include <QApplication>
#include "fenetre.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre fenMacro;
    fenMacro.show();

    return app.exec();
}
