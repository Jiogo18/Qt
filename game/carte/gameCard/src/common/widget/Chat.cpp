#include "Chat.h"

#include "src/common/command/CommandMain.h"

Chat::Chat(QWidget *parent) : QWidget(parent)
{
    setAccessibleName("Chat");
    setLayout(new QVBoxLayout);
    chatArea = new QPlainTextEdit;
    chatArea->setReadOnly(true);
    layout()->addWidget(chatArea);
    chatArea->resize(300, 650);

    chatInput = new QLineEdit;
    layout()->addWidget(chatInput);

    connect(chatInput, &QLineEdit::returnPressed, this, &Chat::sendMsg);
}

Chat::~Chat()
{
    if(chatArea) delete chatArea;
    chatArea = nullptr;
    if(chatInput) delete chatInput;
    chatInput = nullptr;
}

void Chat::addMessage(const ClientInfo *author, QString message)
{
    chatArea->appendHtml(getTime()+" <strong>"+author->getPseudo()+"</strong>: "+message);
}

void Chat::addLog(QString origin, QString log, LogLevel level)
{
    QString type = "";
    switch (level) {
    case LogLevel::Debug: type = "/DEBUG"; break;
    case LogLevel::Info: type = "/INFO"; break;
    case LogLevel::Warning: type = "/WARN"; break;
    case LogLevel::Error: type = "/ERROR"; break;
    }
    chatArea->appendHtml(getTime()+ " ["+origin+type+"]: "+log);
}

void Chat::addText(QString text)
{
    text.replace("\n", "<br/>");
    chatArea->appendHtml(text);
}

void Chat::sendMsg()
{
    QString msg=chatInput->text();
    chatInput->clear();
    if(msg.isEmpty()) return;

    const CommandBase *cmd = CommandMain::getCommand(msg);
    if(cmd != nullptr) {
        CommandBase::CodeRetour ret = cmd->exec(msg);
        if((ret & CommandBase::CodeContinue) == 0) {//continue pas
            return;
        }
    }

    emit newMessage(msg);
}
