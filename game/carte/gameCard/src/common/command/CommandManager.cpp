#include "CommandManager.h"

QString CommandManager::getEntry(QString line)
{
    if(line.startsWith("/"))
        line.remove(0, 1);
    if(!line.contains(" "))
        return line;
    return line.left(line.indexOf(" "));
}

bool CommandManager::isCommand(QString line)
{
    if(line.startsWith("/")) {
        return true;
    }
    return false;
}

QList<QString> CommandManager::splitEntry(QString line)
{
    if(line.startsWith("/"))
        line.remove(0, 1);
    QList<QString> ret = line.split(" ");
    return ret;
}
