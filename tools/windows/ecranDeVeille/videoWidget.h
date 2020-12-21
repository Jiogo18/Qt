#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

//media player/playlist/widget
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

//files
#include <QDir>
#include <QDirIterator>
#include <QMimeDatabase>

//size and foreground
#include <QGuiApplication>//size
#include <QScreen>
#include <QRect>
//#include <windows.h>
//#include <processthreadsapi.h>
//#include <winuser.h>



//bugs avec mp4: résolue avec:
//https://bugreports.qt.io/browse/QTBUG-51692?focusedCommentId=329030&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel
//https://1f0.de/lav-splitter/

class videoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit videoWidget(QWidget *parent=0);
    ~videoWidget();
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setVideo(QString fileName);
    int mediaNumber() const { return player->playlist()->mediaCount(); }

    void show();
    void hide();

    void debug(QString str) { emit onDebug(str); }
signals:
    void onDebug(QString str);
    void mouseMoved();
    void keyPressed(QKeyEvent *event);
    void mousePressed(QMouseEvent *event);

private:
    QList<QString> filesOfDir(QString dir);
    void addMedia(QString file);
    void removeCurrentMedia() { player->playlist()->removeMedia(player->playlist()->currentIndex()); }

    void showAsFullScreen();

    void error(QMediaPlayer::Error error);
    void status(QMediaPlayer::MediaStatus state);

    QMediaPlayer *player=nullptr;
    bool visible = false;//le dernier ordre est show ?
    bool currentIsLoading = false;
    QSize lastSize;
};

#endif // VIDEOWIDGET_H
