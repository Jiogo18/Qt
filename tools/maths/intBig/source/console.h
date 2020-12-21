#ifndef CONSOLE_H
#define CONSOLE_H

#include "intBig6.h"
#include <QMap>
#include <QTextEdit>
#include <QtWidgets>

class TreeOperator
{
public:
    TreeOperator(int v1, int op, int v2);
    int getPos() const { return op; }
private:
    int v1;
    int op;
    int v2;
};

class TreeCommand
{
public:
    TreeCommand(QString data);
private:
    QString data;

};


class Console : public QObject
{
public:
    Console();
    void addCmd(QTextEdit *console, QString command);
    static QList<QString> cutCmd(QString command);
private:
    QMap<QString,intBig6> vars;
};



#endif // CONSOLE_H
