#include "videoWidget.h"

videoWidget::videoWidget(QWidget *parent) : QVideoWidget(parent)
{
    player = new QMediaPlayer(this);
    player->setVideoOutput(this);
    player->setVolume(0);
    playlist=new QMediaPlaylist(player);
    player->setPlaylist(playlist);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(status(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(error(QMediaPlayer::Error)));
    connect(playlist, &QMediaPlaylist::mediaInserted, this, &videoWidget::actuNumberVideo);
    connect(playlist, &QMediaPlaylist::mediaRemoved, this, &videoWidget::actuNumberVideo);
}

void videoWidget::keyPressEvent(QKeyEvent *event)
{emit keyPress(); emit noAFK();}//n'importe quelle touche
void videoWidget::mousePressEvent(QMouseEvent *event)
{emit mousePress(); emit noAFK();}//n'importe quelle click

void videoWidget::setVideo(QString fileName)
{
    fileName = fileName.replace("\\","/");
    playlist->clear();//retire les anciennes vidéos
    if(fileName.isEmpty())
        return;
    QDir dir(fileName);
    if(dir.exists())//un dossier
    {
        QList<QString> files=filesOfDir(fileName);
        for(int i=0; i<files.size(); i++)
        {
            addMedia(files.at(i));
            if(playlist->mediaCount()>=50)
            {
                removeUselessVideo();
                if(playlist->mediaCount()>=50)
                    return;//si il en reste encore bc
            }
        }
    }
    else
    {
        QFile file(fileName);
        if(file.exists())
            addMedia(fileName);
    }
    removeUselessVideo();
    //item->setScale(6);//zoom
}

void videoWidget::play()
{
    if(playlist->isEmpty() || !QVideoWidget::isVisible())
    {hide(); return;}
    if(player->state()==QMediaPlayer::StoppedState ||
       player->state()==QMediaPlayer::PausedState)
    {
        playlist->next();//changer de vidéo
        player->play();
    }
}
void videoWidget::stop()
{
    if(player->state()==QMediaPlayer::PlayingState ||
       player->state()==QMediaPlayer::PausedState)
        player->stop();
}
void videoWidget::pause()
{
    if(player->state()==QMediaPlayer::PlayingState)
        player->pause();
}
void videoWidget::show()
{
    if(playlist->isEmpty())
    {hide(); return;}
    if(!QVideoWidget::isVisible())
    {
        QVideoWidget::showFullScreen();
        QVideoWidget::setFocus();
    }
    play();
}
void videoWidget::hide()
{
    stop();
    if(QVideoWidget::isVisible())
        QVideoWidget::hide();
}
bool videoWidget::isVisible()
{return QVideoWidget::isVisible();}

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
    if(fileMedia.size() > 500000000)//plus de 500Mo
    {
        qDebug()<<"trop lourd"<<file<<support;
    }
    switch (support)
    {
    case QMultimedia::NotSupported:
        qDebug()<<"pas supporté"<<file<<support;
        return;
    default:
        playlist->addMedia(QUrl::fromLocalFile(file));
        //qDebug()<<"supporté"<<file<<support;
        break;
    }
}

void videoWidget::actuNumberVideo()
{
    //qDebug()<<"actuNumber"<<nbVideo->text()<<">"<<playlist->mediaCount();
    emit videoNumberChanged(playlist->mediaCount());
}

void videoWidget::removeUselessVideo()
{
    if(playlist->mediaCount() > 0)
    {//suppr les videos incompatibles
        int i=playlist->mediaCount();
        while(i>0)
        {
            i--;
            playlist->setCurrentIndex(i);
            player->play();
            //attendre que ça charge
            while(player->mediaStatus()==QMediaPlayer::LoadingMedia)
            {
                QThread::msleep(10);
                QCoreApplication::processEvents();
            }//dés que c'est chargé, la fonction error va suppr les faux
        }
    }
    stop();
}

void videoWidget::error(QMediaPlayer::Error error)
{
    //https://doc.qt.io/qt-5/qmediaplayer.html#Error-enum
    //qDebug()<<"erreur"<<error<<"sur"<<playlist->currentIndex()<<playlist->currentMedia().canonicalUrl();
    switch (error) {
    case QMediaPlayer::NoError:
        qDebug()<<"erreur sans erreur?";
        break;
    case QMediaPlayer::ResourceError:
    case QMediaPlayer::FormatError:
    case QMediaPlayer::NetworkError:
    case QMediaPlayer::ServiceMissingError:
    case QMediaPlayer::AccessDeniedError:
        //qDebug()<<"video retirée";
        playlist->removeMedia(playlist->currentIndex());//retire le media
        break;
    default:
        qDebug()<<"autre erreur:"<<error;
        break;
    }
    //qDebug()<<"passé sur la video"<<playlist->currentIndex()<<playlist->currentMedia().canonicalUrl();
}

void videoWidget::status(QMediaPlayer::MediaStatus state)
{
    switch (state) {
    case QMediaPlayer::EndOfMedia:
        play();//recommence
        break;
    case QMediaPlayer::InvalidMedia:
        playlist->removeMedia(playlist->currentIndex());//retire le media
        break;
    default://pause quand on hide() et stop?
        break;
    }
}
