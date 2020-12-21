#include "CommandHelp.h"

#include "src/common/command/CommandRegister.h"

#include "src/common/widget/Chat.h"
#include "src/common/LogNav.h"

void CommandHelp::registerCmd()
{
#define COMMAND_QUESTION
#define COMMAND_HELP
#define COMMAND_H
#define COMMAND_MAN
    new CommandBase({"help", "?", "h", "man"},
                    "Display commands",
                    CommandBase::ToLocal,
                    [](Command *cmd) {
        Chat *chat = LogNav::findConsole();
        QString ret = "";
        if(cmd->getArg(1) != "") {
            const CommandBase *cmdToDetail = CommandRegister::getCommand(cmd->getArg(1));
            if(cmd != nullptr) {

                ret += " &nbsp; &nbsp; " + getEntries(cmdToDetail->getEntries()) + " : " + cmdToDetail->description();//TODO: detailed description
            }
            else {
                chat->addText(QObject::tr("Help: unkow entry \"") + cmd->getArg(1) + "\"");
                return CommandBase::CodeError;
            }
        }
        else {
            ret = QObject::tr("Help: %n command available", "", CommandRegister::getCommands().size());
            for(CommandBase *cmd : CommandRegister::getCommands()) {
                if(cmd->getEntries().size() == 0)
                    continue;//error
                ret += "\n &nbsp; - " + cmd->getEntries().first() + " : " + cmd->description();
            }
        }
        chat->addText(ret);

        return CommandBase::CodeOk;
    });
}

QString CommandHelp::getEntries(QList<QString> entries)
{
    QString ret = "";
    for(QString entry : entries) {
        if(!ret.isEmpty())
            ret += "|";
        ret += entry;
    }
    return "[" + ret + "]";
}
