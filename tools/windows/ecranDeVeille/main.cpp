#include <QApplication>
#include <QCommandLineParser>

#include "fenetre.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("ecranDeVeille");
    QCoreApplication::setApplicationVersion("1.0");


    QCommandLineParser parser;
    parser.setApplicationDescription("Display a movie when you are AFK");
    parser.addHelpOption();
    parser.addVersionOption();

    // A boolean option with a single name (--hide) -h ne marche pas, réservé pour help
    QCommandLineOption hideApplicationOption(QStringList() << "hide",
                                             QCoreApplication::translate("main", "Hide the application at start."));
    if(!parser.addOption(hideApplicationOption))
        qDebug() << "ne peut pas ajouter l'option 'hide'";

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    // source is args.at(0), destination is args.at(1)

    bool hideApplication = parser.isSet(hideApplicationOption);


    fenetre fen;
    if(!hideApplication)
        fen.show();


    return app.exec();
}
