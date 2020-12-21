#ifndef MINECRAFT_H
#define MINECRAFT_H

#include "debug.h"
#include <QObject>
#include <QIcon>
#include <QTimer>
#include "image.h"
#include "mc_gui.h"
#include "mc_craft.h"

class minecraft : public QObject
{
    Q_OBJECT
public:
    minecraft(QString strMCApp, QString strMCFolder, debug* debP=nullptr);
    ~minecraft();
    void start();
    void stop();
    void tickOperation();
    QImage *shootCurrent();
    void startOne();
    void reset();
    void craftDoor();

signals:
    void newIcon(QString ico);

public slots:
    void setMCApp(QString name);
    void setActuInterval(int tps);
    void setMCFolder(QString folder);

private:
    debug *deb;
    QString MCApp="Minecraft";
    QMap<QString, QImage*> imgs;
    QTimer tTick;
    QString MCFolder;
    mc_gui *MCGui;
};

#endif // MINECRAFT_H
