#include "Plateau.h"

Plateau::Plateau(QWidget *parent) : QWidget(parent)
{
    loadPicture();

    scene = new QGraphicsScene;
    view = new QGraphicsView(scene);
    view->setMinimumWidth(300);
    view->setMinimumHeight(300);
    view->resize(800, 700);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QBrush brush(Qt::darkGreen);
    scene->setBackgroundBrush(brush);

    timerActu = new QTimer;
    timerActu->setInterval(100);
    timerActu->setSingleShot(true);

    setLayout(new QVBoxLayout);
    layout()->addWidget(view);

    connect(timerActu, &QTimer::timeout, this, &Plateau::actuPlateau);
    connect(scene, &QGraphicsScene::sceneRectChanged, this, &Plateau::actuPlateau);
}

Plateau::~Plateau()
{
    if(scene) { delete scene; scene = nullptr; }
    if(view) { delete view; view = nullptr; }
    if(timerActu) { delete timerActu; timerActu = nullptr; }
}

void Plateau::actuPlateau()
{
    if(timerActu->isActive())
        return;//on att que le timer se termine
    timerActu->start();

    //actualise background
    QImage background = image.value("background");
    if(!background.isNull())
    {
        background = background.scaled(view->width(), view->height());
        if(graphicsPixmap.contains("background"))
        {
            graphicsPixmap.value("background")->setPixmap(QPixmap::fromImage(background));
        }
        else
            graphicsPixmap.insert("background", scene->addPixmap(QPixmap::fromImage(background)));
    }
}

void Plateau::loadPicture()
{
    image.clear();
    QList<QString> pict = {"background"};
    for(int i=0; i<pict.size(); i++)
    {
        QImage img;
        if(img.load(":/image/"+pict.at(i)+".png"))
        {
            image.insert(pict.at(i), img);
        }
        else
            qDebug()<<"L'image "<<pict.at(i)<<" n'est pas chargée";
    }
}
