#include <QApplication>
#include "FenServeur.h"

//serveur du programme président
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FenServeur fenetre;
    fenetre.show();
    if(!fenetre.serveurIsListenning())//si il y a une erreur
    {
        fenetre.close();
        return 0;
    }

    return app.exec();
};
