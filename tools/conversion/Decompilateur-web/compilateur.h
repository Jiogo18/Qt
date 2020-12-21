#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include <QString>
#include <QThread>
#include <QProgressBar>
#include <QMap>

#include "html.h"

class compilateur : public QObject
{
    Q_OBJECT

public:
    enum Format {
        Auto,
        JS, HTML, CSS,
        JSON
    };
    compilateur(QObject *parent = nullptr);
    ~compilateur();
    bool isAvailable() { return !running; }
    void exit() { if(thread && thread->isRunning()) thread->exit(); }
    Format getAutoFormat(QString code) const;

    void compiler(QString code, int format);
    void decompiler(QString code, int format, QString indentation);

signals:
    void compilationOver(QString);
    void decompilationOver(QString);
    void error(QString);
    void progress(double);

private:
    bool running = false;
    QThread *thread = nullptr;
    void changeProgress(int progress);
};

#endif // COMPILATEUR_H
