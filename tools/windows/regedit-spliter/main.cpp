#include <QApplication>
#include "fenetre.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    fenetre fen;
    fen.show();

    return app.exec();
}


/*
 * 1.1) ouvrir un fichier reg => et faire une sauvegarde => V0
 * 1.2) enregistrer 2 fichiers reg avec:
 *      les élements qui correspondent
 *      les élements non selectionnés (pour pas les reprendres)
 *  => on affiche le nb de clefs final dans chaque fichiers
 * 1.3) rouvrir le 2eme fichier => suppr l'ancien reg (mais vec sauvegarde) => V1, V2, ...
 *  => on arrive avec 2 dossiers: "nom-whitelist" et "nom-blacklist"
 *      soit on fait 2 fichiers (dans les dossiers respectifs) et on triera manuellement
 *      soit on fait 2 fichiers, on donne le fichier source avec le blacklist
 *  => on donne les élements whitelist nouveau notables ("aze"="eza" on donne aze et eza si pas dans whitelist déjà)
 *      note: on clear pas la whitelist et blacklist (c'est mieux)
 * 2.1) mélanger des fichiers pour en faire qu'un
 * 2.2) trier un fichier (comme regedit le fait)
 * 3.1) executer le fichier
 * 3.2) trouver la ligne qui va pas dans le registre (la ligne qui met une erreur d'autorisation)
 *      séparer le fichier petit à petit avec ce qui va
 *      => on peut executer uniquement les clefs ça bug pareil
 *      donner la liste des élements qui passent pas
*/
