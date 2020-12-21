#include <QApplication>
#include <QTranslator>
#include "fenetre.h"

#include "src/common/command/CommonCommands.h"
#include "src/server/command/ServerCommands.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("lang/server_fr");
    app.installTranslator(&translator);

    CommonCommands::registerCmd();
    ServerCommands::registerCmds();

    fenetre fen;

    return app.exec();
}
