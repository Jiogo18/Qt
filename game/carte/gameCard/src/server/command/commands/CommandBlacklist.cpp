#include "CommandBlacklist.h"

#include "src/common/command/CommandRegister.h"

#include "src/server/command/ServNav.h"
#include "src/server/connection/Server.h"

void CommandBlacklist::registerCmd()
{
#define COMMAND_BLACKLIST
#define COMMAND_BL
    new CommandBase({"blacklist", "bl"},
                    "Manage the blacklist",
                    CommandBase::ToServer,
                    [](Command *args) {
        Server *server = ServNav::findServer();
        if(!server) {
            LogNav::addText(QObject::tr("Unable to find the blacklist"));
            return CommandBase::CodeError;
        }

        QString arg = args->getArg(1).toLower();
        if(arg == "list") {
            server->blacklist.debugList();
        }
        else if(arg == "add") {
            if(server->blacklist.addBlacklist(args->getArg(2), Blacklist::CommandBlacklist | Blacklist::ByOperator))
                LogNav::addText(QObject::tr("Blacklisted !"));
            else
                LogNav::addText(QObject::tr("Can't blacklist ") + "\"" + args->getArg(2) + "\"");
        }
        else if(arg == "remove") {
            if(server->blacklist.unBlacklist(args->getArg(2)))
                LogNav::addText(QObject::tr("Unblacklisted !"));
            else
                LogNav::addText(QObject::tr("Can't unblacklist ") + "\"" + args->getArg(2) + "\"");
        }
        else
            LogNav::addText(QObject::tr("Usage: ") + "/blacklist &lsaquo;list|add|remove&rsaquo;");
        return CommandBase::CodeOk;
    });
}
