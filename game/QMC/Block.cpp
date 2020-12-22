#include "Block.h"


Block::Block(QObject *parent, QString rressourcepack, QRectF rrec, QString rid)
{
    setParent(parent);
    id = rid;
    rec = rrec;
    ressourcepack = rressourcepack;
    name=id;
    connect(parent, SIGNAL(actuRessourcepack(QString)), this, SLOT(setRessourcepack(QString)));
    casse=0;
    //tempCasse doit etre set selon une enum ou un fichier ressource...
}

QRectF Block::boundingRect() const
{
    return rec;
}


void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::blue);
    QString limage;
    brush.setColor(Qt::darkGray);
    limage = id+".png";

    QPixmap image(":/ressourcepacks/" + ressourcepack + "/QMC/textures/blocks/" + limage);
    if(!image)
    {
        qDebug(("No pixmap "+limage+" in "+ressourcepack).toStdString().c_str());
        image = QPixmap(":/ressourcepacks/defaut/QMC/textures/blocks/" + limage);
    }

    painter->fillRect(rec, brush);
    painter->drawPixmap(rec, image, QRectF(0, 0, image.width(), image.height()));

    painter->setPen(QPen(Qt::lightGray));
    QFont font = painter->font();
    font.setPointSize(20);
    painter->setFont(font);
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void Block::setRessourcepack(QString nressourcepack)
{
    ressourcepack = nressourcepack;
    update();
}
