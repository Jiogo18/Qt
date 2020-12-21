#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QList>


class Command {
public:
    Command(QString line);
    QString getArg(int i) const { return i < content.size() ? content.at(i) : ""; }

private:
    QList<QString> content = {};
};



#endif // COMMAND_H
