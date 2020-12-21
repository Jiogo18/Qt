#include "GraphicsButton.h"


GraphicsButton::GraphicsButton(QObject *parent, QString rressourcepack, QRectF rrec, QString rtext, QString action, bool activated)
{
    setParent(parent);
    survole = false;
    text = rtext;
    rec = rrec;
    ressourcepack = rressourcepack;
    act = action;
    actif=activated;
    setAcceptHoverEvents(true);
    connect(this, SIGNAL(clicked(QString)), parent, SLOT(buttonClicked(QString)));
    connect(parent, SIGNAL(actuRessourcepack(QString)), this, SLOT(setRessourcepack(QString)));
}

QRectF GraphicsButton::boundingRect() const
{
    return rec;
}

void GraphicsButton::setText(QString rtext)
{
    text = rtext;
}

void GraphicsButton::resize(int x, int y)
{
    rec.setX(x);
    rec.setY(y);
    update();
}

int GraphicsButton::getX()
{
    return rec.x();
}

int GraphicsButton::getY()
{
    return rec.y();
}

void GraphicsButton::setEnabled(bool etat)
{
    actif = etat;
}


void GraphicsButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::blue);
    QString limage;
    if(actif)
    {
        if(survole)
        {
            brush.setColor(Qt::gray);
            limage = "button2.png";
        }
        else
        {
            brush.setColor(Qt::darkGray);
            limage = "button1.png";
        }
    }
    else
    {
        brush.setColor(Qt::black);
        limage = "button0.png";
    }
    QPixmap image("ressourcepacks/" + ressourcepack + "/QMC/textures/gui/" + limage);
    if(!image)
    {
        qDebug(("No pixmap "+limage+" in "+ressourcepack).toStdString().c_str());
        image = QPixmap("ressourcepacks/defaut/QMC/textures/gui/" + limage);
    }

    painter->fillRect(rec, brush);
    painter->drawPixmap(rec, image, QRectF(0, 0, image.width(), image.height()));

    painter->setPen(QPen(Qt::lightGray));
    QFont font = painter->font();
    font.setPointSize(20);
    painter->setFont(font);
    painter->drawText(rec, text, QTextOption(Qt::AlignCenter));

}

void GraphicsButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    survole = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    survole = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void GraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(actif)
        emit clicked(act);
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsButton::setRessourcepack(QString nressourcepack)
{
    ressourcepack = nressourcepack;
    update();
}
