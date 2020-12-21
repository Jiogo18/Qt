#include "GraphicsButton2.h"

/*void GraphicsButton2::setUP(int rx, int ry, int rwidth, int rheight, QString ressourcepack, QString rtext)
{
    setX(rx);
    setY(ry);
    width = rwidth;
    height = rheight;
    text = rtext;
    lien = "/ressourcepack/" + ressourcepack + "/assets/QMC/textures/gui/";
    ActuButton(false);
    connect(this, SIGNAL(hovered()), this, SLOT(connectSurvole()));


}

void GraphicsButton2::ActuButton(bool survole)
{
    QString skin;
    if(survole)
    {
        skin = lien + "button2";//si survolé
    }
    else
    {
        skin = lien + "button1";//si pas survolé
    }
    QPixmap pixmap(skin);
    pixmap = pixmap.scaled(width, height);

    QPainter painter( &pixmap );
    painter.setFont( QFont("Arial") );
    painter.drawText( width/2, height/2, width, height, Qt::AlignCenter, text);

    setPixmap(pixmap);
}


void GraphicsButton2::connectSurvole()
{
    ActuButton(ho);
}*/

GraphicsButton2::GraphicsButton2()
{
    Pressed = false;
    setFlag(ItemIsMovable);
    survole = false;
    connect(this, SIGNAL(hovered()), this, SLOT(connectHovered()));
}

QRectF GraphicsButton2::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void GraphicsButton2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);
    if(survole)
    {
        brush.setColor(Qt::blue);
        survole = false;
    }
    else
    {
        if(Pressed)
        {
            brush.setColor(Qt::red);
        }
        else
        {
            brush.setColor(Qt::green);
        }
    }
    painter->fillRect(rec, brush);
    painter->drawEllipse(rec);
    painter->drawRect(rec);

    painter->setPen(QPen(QColor("blue")));
    QFont font = painter->font();
    font.setFamily("Comic Sans MS");
    font.setPointSize(100);
    painter->setFont(font);
    painter->drawText(QRectF(0, 0, 100, 100), "slt", QTextOption(Qt::AlignCenter));

}

void GraphicsButton2::connectHovered()
{
    survole = true;
    update();

}

void GraphicsButton2::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsButton2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
