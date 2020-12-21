#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>

class Block : public QObject, public QGraphicsItem//si survolé, changer de QPixmap
{
    Q_OBJECT
    public:
        Block(QObject *parent, QString rressourcepack, QRectF rrec, QString rid);

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void mousePressEvent(QGraphicsSceneMouseEvent *event);


    public slots:
        void setRessourcepack(QString nressourcepack);


    signals:
        void clicked(QString action);

    private:
        QString ressourcepack;
        QRectF rec;
        QString id;
        QString name;
        int casse;
        int tempCasse;
};

#endif // BLOCK_H
