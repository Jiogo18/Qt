#ifndef COMMANDHELP_H
#define COMMANDHELP_H


#include <QString>
#include <QList>

namespace CommandHelp {
    void registerCmd();
    QString getEntries(QList<QString> entries);
}

#endif // COMMANDHELP_H
