#include "Console.h"
#include <iostream>

Console::Console() : QObject() {}

void Console::addCmd(QTextEdit *console, QString command)
{
    console->append(">> "+command);
    QList<QString> args = cutCmd(command);
    if(args.isEmpty())
        return;
    QList<QString> argsS = args;
    argsS.removeAll("");

    args.first() = args.first().toLower();
    if(args.first() == "help") {
        console->append("HELP            affiche cette aide");
        console->append("[VALUE]=[VALUE] opérations");
        console->append("                  de bases: + - * / %");
        console->append("                  affectation: =");
        console->append("                  comparaison: == != < <= > >=");
        console->append("                  ^ (uniquement avec qint64 en exposant)");
        console->append("");
        return;
    }
}
/*comment faire ?
 * il faut séparer selon les opérateurs
 * un block ne contient que 2 nbs et un opérateur
 * (= n'est pas un opérateur mais il peut y en avoir après)
 * on identifie les opérateurs par leurs positions
 * 1) on identifie l'arbre actuel (parenthèses)
 * 2) on met des parenthèses autour des arguments (ordre *+) de chaque ligne
 * (avec les plus haut d'abord) => augmenter le nb de branches
 * on commence par les opérateurs de la ligne de g vers d
 *
*/

QList<QString> Console::cutCmd(QString command)
{
    QString currentArg = "";
    QList<QString> retour = {};
    bool onString = false;
    for(int i=0; i<command.size(); i++) {
        QChar currentChar = command.at(i);
        if(currentChar == '\\') {
            i++; continue;
        }
        if(currentChar == '"' || currentChar == ' ') {
            if(currentChar == '"') {
                onString = !onString;
                if(onString)
                    continue;
            }
            else if(onString)
                continue;
            retour.append(currentArg);
            currentArg = "";
            continue;
        }
        else
            currentArg.append(currentChar);
    }
    while(!retour.isEmpty() && retour.first() == "")
        retour.removeFirst();
    return retour;
}
