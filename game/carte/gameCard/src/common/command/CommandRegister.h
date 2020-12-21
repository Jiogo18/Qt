#ifndef COMMANDREGISTER_H
#define COMMANDREGISTER_H


#include <QList>
#include <QString>
#include "commands/Command.h"

class CommandBase
{
public:
    enum NetworkTarget {
        ToNobody,
        ToServer,
        ToClient,
        ToLocal

    };
    enum CodeRetour {
        CodeOk = 1,
        CodeError = 2,
        CodeContinue = 4,
        ErrorIncomplete = 8|CodeError,
    };

    CommandBase(QList<QString> entries, QString desc, NetworkTarget target,
                std::function<CodeRetour(Command*)> func);
    QList<QString> getEntries() const { return entries; }
    bool isEntry(QString entry) const { return entries.contains(entry.toLower()); }
    NetworkTarget getNetTarget() const  { return target; }
    QString description() const { return desc; }

    CodeRetour exec(Command *args) const { return func ? func(args) : ErrorIncomplete; }
    CodeRetour exec(QString line) const;
private:
    QList<QString> entries = {};
    QString desc = "No Description";
    NetworkTarget target = ToNobody;
    std::function<CodeRetour(Command*)> func;
};

namespace CommandRegister {

    static QList<CommandBase*> commands;

    bool registerCommand(CommandBase *cmd);
    bool isEntry(QString entry);
    const CommandBase *getCommand(QString entry);
    const QList<CommandBase*> getCommands();
};

#endif // COMMANDREGISTER_H
