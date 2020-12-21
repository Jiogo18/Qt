#include "videoWidget.h"

videoWidget::videoWidget(QWidget *parent) : QVideoWidget(parent)
{
    player = new QMediaPlayer(this);
    player->setVideoOutput(this);
    player->setVolume(0);
    player->setPlaylist(new QMediaPlaylist(player));
    setCursor(Qt::BlankCursor);//hide the cursor when shown
    setMouseTracking(true);//mouseMoveEvent

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &videoWidget::status);
    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &videoWidget::error);
}

videoWidget::~videoWidget() { delete player; }

void videoWidget::mouseMoveEvent(QMouseEvent *event)
{ emit mouseMoved(); QVideoWidget::mouseMoveEvent(event);}
void videoWidget::keyPressEvent(QKeyEvent *event)
{ emit keyPressed(event); QVideoWidget::keyPressEvent(event);}
void videoWidget::keyReleaseEvent(QKeyEvent *event)
{ emit keyPressed(event); QVideoWidget::keyReleaseEvent(event);}
void videoWidget::mousePressEvent(QMouseEvent *event)
{ emit mousePressed(event); QVideoWidget::mousePressEvent(event); }
void videoWidget::mouseReleaseEvent(QMouseEvent *event)
{ emit mousePressed(event); QVideoWidget::mouseReleaseEvent(event);}


void videoWidget::setVideo(QString fileName)
{
    fileName = fileName.replace("\\","/");
    player->stop();
    player->playlist()->clear();//retire les anciennes vidéos
    if(fileName.isEmpty())
        return;
    QDir dir(fileName);
    if(dir.exists())//un dossier
    {
        QList<QString> files=filesOfDir(fileName);
        for(int i=0; i<files.size(); i++)
        {
            addMedia(files.at(i));
            if(player->playlist()->mediaCount()>=50)//on peut avoir pris C: ....
                break;
        }
    }
    else//un fichier ?
    {
        QFile file(fileName);
        if(file.exists())
            addMedia(fileName);
    }

    currentIsLoading = true;
    if(visible)
        show();
}

void videoWidget::show()
{
    player->play();
    if(player->state() != QMediaPlayer::PlayingState) {
        hide();//sécurité
        return;
    }
    visible = true;
    if(!currentIsLoading)//si en chargement on fait rien
        showAsFullScreen();
}
void videoWidget::hide()
{
    visible = false;
    QVideoWidget::hide();
    player->pause();
}




QList<QString> videoWidget::filesOfDir(QString dir)
{
    QList<QString> retour;
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        retour.append(it.next());
        //qDebug()<<"filesOfDir"<<retour.last();
        if(retour.last().endsWith("/.."))
            for(int i=0; i<3 && retour.size()>0; i++)
                retour.removeLast();//retire "dir/", "dir/." et "dir/.."
    }
    /*for(int i=0; i<retour.size(); i++)
        qDebug()<<"filesOfDir2"<<retour.at(i);*/
    return retour;
}
void videoWidget::addMedia(QString file)
{
    QMimeDatabase mimeData;
    //le MIME type marche:
    QString mimeType=mimeData.mimeTypesForFileName(file).first().name();

    QMultimedia::SupportEstimate support
            =QMediaPlayer::hasSupport(mimeType);

    QFile fileMedia(file);
    //qDebug()<<"info fichier:"<<file<<"MIMEType:"<<mimeType<<"taille:"<<fileMedia.size();
    if(fileMedia.size() > 1000000000)//plus de 1Go
    {
        qDebug()<<"videoWidget vidéo trop lourde:" << file << support;
        return;
    }
    switch (support)
    {
    case QMultimedia::NotSupported:
        qDebug()<<"videoWidget vidéo pas supporté:" << file << support;
        return;
    default:
        player->playlist()->addMedia(QUrl::fromLocalFile(file));
        break;
    }
}


void videoWidget::showAsFullScreen()
{
    QSize screenSize = QGuiApplication::screens().first()->geometry().size();
    QVideoWidget::setFullScreen(true);
    QVideoWidget::show();
    if(screenSize != lastSize) {
        player->setVideoOutput(this);
        lastSize = screenSize;
    }
    if(size() != screenSize)
        setGeometry(QRect(QPoint(0,0), screenSize));

    //focus uniquement devant les autres fenetres de l'appli uniquement :
    setFocus(Qt::PopupFocusReason);
    //pareil que setFocus() : (il faut des autorisations)
    //if(GetForegroundWindow() != (HWND)QWidget::winId())
    //    SetForegroundWindow((HWND)QWidget::winId());
}

void videoWidget::error(QMediaPlayer::Error error)
{
    //https://doc.qt.io/qt-5/qmediaplayer.html#Error-enum
    switch (error) {
    case QMediaPlayer::ResourceError:
    case QMediaPlayer::FormatError:
    case QMediaPlayer::ServiceMissingError:
        qWarning() << "videoWidget cette vidéo n'est pas compatible:" << error;
        removeCurrentMedia();
        break;
    case QMediaPlayer::NetworkError:
    case QMediaPlayer::AccessDeniedError:
        qWarning() << "videoWidget cette vidéo n'est pas accecible:" << error;
        removeCurrentMedia();
        break;
    default:
        qWarning() << "videoWidget Erreur:" << error;
        break;
    }
    emit onDebug(QString("videoWidget::error " + QString::number(error)));
}
void videoWidget::status(QMediaPlayer::MediaStatus state)
{
    switch (state) {
    case QMediaPlayer::EndOfMedia:
        player->play();//recommence
        break;
    case QMediaPlayer::InvalidMedia:
        removeCurrentMedia();
        break;
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferedMedia:
        currentIsLoading = false;
        if(visible)
            showAsFullScreen();
        else
            player->pause();
        //qDebug() << "videoWidget::status" << state;
        break;
    case QMediaPlayer::LoadingMedia:
        currentIsLoading = true;
        //qDebug() << "videoWidget::status" << state;
        break;
    default:
        //qDebug() << "videoWidget::status" << state;
        break;
    }
    emit onDebug(QString("videoWidget::status " + QString::number(state)));
}
