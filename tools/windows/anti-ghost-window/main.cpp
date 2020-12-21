#include <QApplication>
#include <QCommandLineParser>

#include "anti_ghost_window.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("anti-ghost-window");
    QCoreApplication::setApplicationVersion("1.0");


    QCommandLineParser parser;
    parser.setApplicationDescription("See and save which window is in the forground");
    parser.addHelpOption();
    parser.addVersionOption();

    // A boolean option with a single name (--hide) -h ne marche pas, réservé pour help
    QCommandLineOption hideApplicationOption(QStringList() << "hide",
                                             QCoreApplication::translate("main", "Hide the application at start."));
    if(!parser.addOption(hideApplicationOption))
        qDebug() << "ne peut pas ajouter l'option 'hide'";

    // boolean to clean log (-c, --clean)
    QCommandLineOption cleanLogOption(QStringList() << "c" << "clean",
                                      QCoreApplication::translate("main", "Clean logs of the Application."));
    if(!parser.addOption(cleanLogOption))
        qDebug() << "ne peut pas ajouter l'option 'clean'";

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    bool hideApplication = parser.isSet(hideApplicationOption);
    bool cleanLog = parser.isSet(cleanLogOption);


    anti_ghost_window agw(hideApplication);
    if(cleanLog)
        agw.cleanLog();


    return app.exec();

}
