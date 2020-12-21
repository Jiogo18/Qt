#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDateTime>
#include "src/common/client/ClientInfo.h"

#include "src/common/command/commands/Command.h"


class Chat : public QWidget
{
    Q_OBJECT
public:
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };
    Chat(QWidget *parent = nullptr);
    ~Chat();
    void addMessage(const ClientInfo *author, QString message);
    void addLog(QString origin, QString log, LogLevel level);
    void addText(QString text);
    void showEvent(QShowEvent *event) override { Q_UNUSED(event) chatInput->setFocus(); };

private slots:
    void sendMsg();

signals:
    void newMessage(QString);

private:
    static QString getTime() { return QDateTime::currentDateTime().toString("[HH:mm:ss]"); }
    QPlainTextEdit *chatArea = nullptr;
    QLineEdit *chatInput = nullptr;
};

#endif // CHAT_H
