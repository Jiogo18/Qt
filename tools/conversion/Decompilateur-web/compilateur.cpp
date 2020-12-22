#include "compilateur.h"

compilateur::compilateur(QObject *parent) : QObject(parent)
{
    thread = new QThread();
    moveToThread(thread);
    thread->start();
}

compilateur::~compilateur()
{
    thread->exit();
    thread->deleteLater();
}

compilateur::Format compilateur::getAutoFormat(QString code) const
{
    Format bestFormat = Format::Auto;
    int bestIndex = -1, i;
    i = code.indexOf(QRegularExpression("<[\\w\\s\"=\\-\\/]*>"));
    if(i != -1) {
        bestFormat = Format::HTML;
        bestIndex = i;
    }
    i = code.indexOf(QRegularExpression("[\\W]*\\s?{[\\s\\w:;\\/,\\-+%]*}"));
    if(i != -1 && i < bestIndex) {
        bestFormat = Format::CSS;
        bestIndex = i;
    }
    else if(i != -1 && i == bestIndex)
        qDebug() << "Confusion entre 2 types:" << bestFormat << Format::CSS;
    i = code.indexOf(QRegularExpression("function\\([\\w,\\s]*\\)\\s*{"));
    if(i != -1 && i < bestIndex) {
        bestFormat = Format::JS;
        bestIndex = i;
    }
    else if(i != -1 && i == bestIndex)
        qDebug() << "Confusion entre 2 types:" << bestFormat << Format::JS;
    i = code.indexOf(QRegularExpression("\"\\w*\"\\s*:\\s*\"\\w*\""));
    if(i != -1 && i < bestIndex) {
        bestFormat = Format::JSON;
        bestIndex = i;
    }
    else if(i != -1 && i == bestIndex)
        qDebug() << "Confusion entre 2 types:" << bestFormat << Format::JSON;

    qDebug() << "Format Auto :" << bestFormat;
    return bestFormat;
}


void compilateur::compiler(QString code, int format)
{
    if(!isAvailable())
        return;
    running = true;
    switch (format) {
    case Format::HTML:
        emit compilationOver(html::compile(code));
        break;
    default:
        emit error("format unknow:" + QString::number(format));
    }
    running = false;
}

void compilateur::decompiler(QString code, int format, QString indentation)
{
    if(!isAvailable())
        return;
    running = true;
    switch (format) {
    case Format::HTML:
        emit compilationOver(html::compile(code));
        break;
    default:
        emit error("format unknow:" + QString::number(format));
    }
    emit decompilationOver(code);
    running = false;
}

