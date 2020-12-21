#ifndef COMMANDMAIN_H
#define COMMANDMAIN_H

#include "commands/Command.h"
#include "CommandManager.h"
#include "CommandRegister.h"

#include <QString>



namespace CommandMain {


    const CommandBase *getCommand(QString line);

};

#endif // COMMANDMAIN_H
