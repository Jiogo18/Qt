#include "objet.h"

objet::objet()
{

}

void objet::run()// override
{
    qDebug() << "Hello world (objet) from thread" << QThread::currentThread();
}
