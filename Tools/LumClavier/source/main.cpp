#include <QApplication>
#include "fen.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fen fenetre;
    fenetre.show();


    return app.exec();
}
