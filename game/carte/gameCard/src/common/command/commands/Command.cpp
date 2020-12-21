#include "Command.h"

#include "src/common/command/CommandManager.h"


Command::Command(QString line)
{
    content = CommandManager::splitEntry(line);
    if(content.size() == 0)
        return;
}
