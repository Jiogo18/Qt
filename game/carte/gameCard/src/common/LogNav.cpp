#include "LogNav.h"

#include "src/common/WidgetNav.h"

void LogNav::addLog(QString origin, QString log, Chat::LogLevel level)
{
    Chat *chat = findConsole();
    if(chat) chat->addLog(origin, log, level);
}

Chat *LogNav::findConsole()
{
    return WidgetNav::find<Chat>("Chat");
}

void LogNav::addText(QString text)
{
    Chat *chat = findConsole();
    if(chat) chat->addText(text);
}
