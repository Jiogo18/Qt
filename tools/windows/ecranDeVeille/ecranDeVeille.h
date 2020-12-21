#ifndef ECRANDEVEILLE_H
#define ECRANDEVEILLE_H

#include <QWidget>
#include <QFile>
#include <QDateTime>
#include <QCursor>
#include "videoWidget.h"
#include "imageTool.h"
#include <QTimer>
#include <windows.h>
#include <QKeyEvent>


class EDVConfig //Ecran De Veille Config
{
public:
    EDVConfig() {}
    EDVConfig(const EDVConfig &config) { operator =(config); }
    const EDVConfig *operator =(const EDVConfig &config);
    bool operator ==(const EDVConfig &config);

    void setRessourcePath(QString path) { this->ressourcePath = path; }
    QString getRessourcePath() { return ressourcePath; }
    void setTimeout(int time) { if(time < 4) time = 4; timeOut = time; }
    int getTimeout() const { return timeOut; }

    void setTimeRefresh(int time) { if(time < 10) time = 10; timeRefresh = time; }
    int getTimeRefresh() const { return timeRefresh; }

    void setScreenshot(bool on) { screenshot = on; }
    bool isScreenshot() const { return screenshot; }
    void setScreenshotMargin(float margin) { if(margin < 0) margin = 0; if(margin > 100.0) margin = 100; screenshotMargin = margin; }
    float getScreenshotMargin() const { return screenshotMargin; }
    void setScreenshotTime(int time) { if(time < 1) time = 1; screenshotTime = time; }
    int getScreenshotTime() const { return screenshotTime; }

    void setMouse(bool on) { mouse = on; }
    bool isMouse() const { return mouse; }
    void setMouseMargin(int margin) { if(margin < 0) margin = 0; mouseMargin = margin; }
    int getMouseMargin() const { return mouseMargin; }
    void setMouseTime(int time) { if(time < 1) time = 1; mouseTime = time; }
    int getMouseTime() const { return mouseTime; }

    void setMouseV(bool on) { mouseV = on; }
    bool isMouseV() const { return mouseV; }
    void setMouseVMargin(int margin) { if(margin < 0) margin = 0; mouseVMargin = margin; }
    int getMouseVMargin() const { return mouseVMargin; }
    void setMouseVRepeat(bool repeat) { mouseVRepeat = repeat; }
    int isMouseVRepeat() const { return mouseVRepeat; }


    void setKeyboard(bool on) { keyboard = on; }
    bool isKeyboard() const { return keyboard; }
    void setKeyboardRepeat(bool repeat) { keyboardRepeat = repeat; }
    int isKeyboardRepeat() const { return keyboardRepeat; }

    bool save(QString filePath) const;
    bool load(QString filePath);

private:
    QString ressourcePath="";
    int timeOut=180;//time of inactivity (s)
    int timeRefresh=1000;//time for global refresh (msec)

    bool screenshot=true;
    float screenshotMargin=2;//(%)
    int screenshotTime=10;//(s)

    bool mouse=true;
    int mouseMargin=10;//(px)
    int mouseTime=10000;//(msec)

    bool mouseV=true;
    int mouseVMargin=100;//(px)
    bool mouseVRepeat=false;

    bool keyboard=true;
    bool keyboardRepeat=false;
};

class ecranDeVeille : public QObject
{
    Q_OBJECT
public:
    ecranDeVeille(QObject *parent = nullptr);
    ~ecranDeVeille();


    void setConfig(EDVConfig config);
    EDVConfig getConfig() { return config; }

    void showIf();
    void show();
    void hide();

    int getTimeRemain() const { return timerTimeout.remainingTime(); }
    int getNbVideo() const { return viewVideo->mediaNumber(); }

    bool timeoutScreenshot();
    void timeoutMouse();
    void mouseMovedOnWindow();
    void keyPressedOnWindow(const QKeyEvent *event);
    void mousePressedOnWindow(const QMouseEvent *event);

    void debug(QString str) { emit onDebug(str); }
signals:
    void onDebug(QString str);
    void shown();
    void hidden();

private:
    EDVConfig config;
    videoWidget *viewVideo=nullptr;
    QTimer timerTimeout;

    QTimer timerScreenshot;
    QImage lastScreenShot;

    QTimer timerMouse;
    QPoint lastMousePos;
};

#endif // ECRANDEVEILLE_H
