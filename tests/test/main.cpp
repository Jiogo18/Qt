#include <QApplication>
#include "fenetre.h"
#include "objet.h"

class HelloWorldTask : public QRunnable
{
    void run() override
    {
        qDebug() << "Hello world (class) from thread" << QThread::currentThread();
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetre fen;
    if(false)
    {
        qDebug() << "Hello world (main) from thread" << QThread::currentThread();
        HelloWorldTask *hello = new HelloWorldTask();
        // QThreadPool takes ownership and deletes 'hello' automatically
        QThreadPool::globalInstance()->start(hello);
        objet *o1 = new objet();
        QThreadPool::globalInstance()->start(o1);
        objet o2;
        QThreadPool::globalInstance()->start(&o2);
    }

    return app.exec();
}
