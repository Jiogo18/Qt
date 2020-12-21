#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

class videoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    explicit videoWidget(QWidget *parent=0);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setVideo(QString fileName);
    void play();
    void stop();
    void pause();
    void show();
    void hide();
    bool isVisible();
    QList<QString> filesOfDir(QString dir);
    void addMedia(QString file);
    void actuNumberVideo();
    void removeUselessVideo();

public slots:
    void error(QMediaPlayer::Error error);
    void status(QMediaPlayer::MediaStatus state);

signals:
    void keyPress();
    void mousePress();
    void noAFK();
    void videoNumberChanged(quint64);

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};

#endif // VIDEOWIDGET_H
