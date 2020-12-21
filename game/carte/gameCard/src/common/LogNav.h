#ifndef LOGNAV_H
#define LOGNAV_H

#include <QString>
#include "src/common/widget/Chat.h"


namespace LogNav {
    void addLog(QString origin, QString log, Chat::LogLevel level);
    void addText(QString text);

    Chat *findConsole();
};

#endif // LOGNAV_H
