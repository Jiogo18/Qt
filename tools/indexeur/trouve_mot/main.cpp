#include <QApplication>
#include "fenetre.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre fen;
    fen.show();

    return app.exec();
}
