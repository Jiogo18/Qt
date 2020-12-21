#ifndef OBJET_H
#define OBJET_H

#include <QThread>
#include <QRunnable>
#include <QDebug>

class objet : public QRunnable
{
public:
    objet();
    void run();
};

#endif // OBJET_H
