#include "ServerCommands.h"

#include "commands/CommandBlacklist.h"

void ServerCommands::registerCmds() {
    CommandBlacklist::registerCmd();
}
