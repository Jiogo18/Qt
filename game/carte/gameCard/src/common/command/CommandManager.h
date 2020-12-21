#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QString>
#include <QList>

namespace CommandManager {
    bool isCommand(QString line);
    QString getEntry(QString line);
    QList<QString> splitEntry(QString line);
};

#endif // COMMANDMANAGER_H
