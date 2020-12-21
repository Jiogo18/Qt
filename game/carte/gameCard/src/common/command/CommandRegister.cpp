#include "CommandRegister.h"

#include <QDebug>
#include <QRegExp>
#include "src/common/LogNav.h"

CommandBase::CommandBase(QList<QString> entries, QString desc,
                         NetworkTarget target,
                         std::function<CodeRetour(Command*)> func)
{
    this->entries = entries;
    this->desc = desc;
    this->target = target;
    this->func = func;
    if(!entries.isEmpty()) {
        CommandRegister::registerCommand(this);
    }
}

CommandBase::CodeRetour CommandBase::exec(QString line) const
{
    LogNav::addText(">> " + line);
    Command *cmd = new Command(line);
    CodeRetour ret=exec(cmd);
    delete cmd;
    return ret;
}


bool CommandRegister::registerCommand(CommandBase *cmd)
{
    if(cmd->getNetTarget() == CommandBase::ToNobody) {
        qWarning() << "CommandBase no target set for" << cmd->description();
        return false;
    }
    QList<QString> entries = cmd->getEntries();
    if(entries.isEmpty()) {
        qWarning() << "CommandBase no entry set for " << cmd->description();
        return false;
    }
    for(QString entry : entries) {
        if(!QRegExp("[0-9a-z?_:]+").exactMatch(entry)) {
            entries.removeAll(entry);
            qWarning() << "CommandBase entry not valid: " << entry << "should match the regexp \"[0-9a-z]+\"";
        }
    }
    if(entries.isEmpty()) {
        qWarning() << "CommandBase no entry valid for " << cmd->description();
        return false;
    }

    for(CommandBase *cmd2 : commands) {
        for(QString entry : entries) {
            if(cmd2->isEntry(entry)) {
                entries.removeAll(entry);
                qWarning() << "CommandBase entry already used: " << entry << entries << commands.size();
            }
        }
    }
    if(entries.isEmpty()) {
        qWarning() << "CommandBase no entry valid for " << cmd->description();
        return false;
    }

    commands.append(cmd);
    return true;
}


bool CommandRegister::isEntry(QString entry) {
    for(CommandBase *cmd : commands) {
        if(cmd->isEntry(entry))
            return true;
    }
    return false;
}

const CommandBase *CommandRegister::getCommand(QString entry)
{
    for(const CommandBase *cmd : commands) {
        if(cmd->isEntry(entry))
            return cmd;
    }
    return nullptr;
}

const QList<CommandBase *> CommandRegister::getCommands() { return commands; }
