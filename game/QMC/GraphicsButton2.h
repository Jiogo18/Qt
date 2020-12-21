#ifndef GRAPHICSBUTTON2_H
#define GRAPHICSBUTTON2_H

#include <QGraphicsItem>
#include <QAction>
#include <QPainter>
#include <QString>
#include <QFontDatabase>

class GraphicsButton2 : public QGraphicsItem, public QAction //si survolé, changer de QPixmap
{
    public://constructeur par QGraphicsPixmapItem
        GraphicsButton2();

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        bool Pressed;
        bool survole;

    public slots:
        void connectHovered();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


        /*void setUP(int rx, int ry, int rheight, int rwidth, QString ressourcepack, QString rtext="");
        void ActuButton(bool survole);

    public slots:
        void connectSurvole();


    private:
        int height;
        int width;
        QString text;
        QString lien;*/



};

#endif // GRAPHICSBUTTON2_H
