#include <QApplication>
#include "fenGraphique.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenGraphique fenP;
    fenP.show();


    return app.exec();
}
