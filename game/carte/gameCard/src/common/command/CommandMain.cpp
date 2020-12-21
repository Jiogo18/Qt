#include "CommandMain.h"


const CommandBase *CommandMain::getCommand(QString line)
{
    QString entry = CommandManager::getEntry(line);
    return CommandRegister::getCommand(entry);
}
