#ifndef GRAPHICSBUTTON_H
#define GRAPHICSBUTTON_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneHoverEvent>

class GraphicsButton : public QObject, public QGraphicsItem//si survolé, changer de QPixmap
{
    Q_OBJECT
    public:
        GraphicsButton(QObject *parent, QString ressourcepack, QRectF rrec, QString rtext, QString action, bool activated=true);

        void setText(QString rtext);
        void resize(int x, int y);
        int getX();
        int getY();
        void setEnabled(bool etat);


        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);


    public slots:
        void setRessourcepack(QString nressourcepack);



    signals:
        void clicked(QString action);

    private:
        QString text;
        QString ressourcepack;
        QRectF rec;
        bool survole;
        QString act;
        bool actif;
};

#endif // GRAPHICSBUTTON_H
