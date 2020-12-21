#ifndef ANTI_GHOST_WINDOW_H
#define ANTI_GHOST_WINDOW_H


#include "fenetre.h"
#include <QTimer>
#include <QString>
#include <QTime>
#include <QDebug>
#include <windows.h>
#include <QRect>
#include <QDir>
#include <QFile>
#include <QObject>

class anti_ghost_window : public QObject
{
    Q_OBJECT
public:
    anti_ghost_window(bool hideApp);
    ~anti_ghost_window();
    static HWND getCurrentWindow();
    void actuCurrent();
    static QString getInfoWindow(HWND window);

    static QString getTimestamp();
    void addLog(QString log);
    void cleanLog() const;
    QString newFileName() const;
    bool verifFileOut(bool boucle=false);


private:
    void needLogDir() const;
    fenetre *fen = nullptr;
    QTimer timerActu;
    HWND lastWindow;
    const QString fileLog= "anti-ghost-window_*.log";
    QDir dirLog;
    int nbDataSaved=0;//nb de données enregistrées depuis la derniere écriture
    qint64 lastSecWrite=0;
    QFile currentFile;
    QTextStream out;
};

#endif // ANTI_GHOST_WINDOW_H
