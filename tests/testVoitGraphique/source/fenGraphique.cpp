#include "fenGraphique.h"

fenGraphique::fenGraphique()
{
    {
    scene = new QGraphicsScene;
    QPixmap img("image.png");
    QSize size( img.size().width() / 1 , img.size().height() / 1 );
    img = img.scaled(size);
    //scene.addPixmap(img);

    QPen pen(QColor("Green"));
    pen.setWidth(10);
    scene->addRect(QRectF(0, 0, 100, 100), pen);
    view = new QGraphicsView(scene);
    scene->setBackgroundBrush(QBrush(QPixmap("route.png")));


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
    }

    QPixmap voit("voiture.png");
    voit = voit.scaled(voit.width()/10, voit.height()/10);
    voiture = scene->addPixmap(voit);
    voiture->setTransformOriginPoint(voit.width()/2, voit.height()/2);

}

void fenGraphique::keyPressEvent(QKeyEvent *event)
{
        switch (event->key())
        {
        case Qt::Key_S:
            avancerVoiture(false);
            break;
        case Qt::Key_Z:
            avancerVoiture();
            break;
        case Qt::Key_Q:
            voiture->setRotation(voiture->rotation()-10);
            break;
        case Qt::Key_D:
            voiture->setRotation(voiture->rotation()+10);
            break;
        }
        if(voiture->rotation() <= -180)
            voiture->setRotation(voiture->rotation()+360);
        if(voiture->rotation() > 180)
            voiture->setRotation(voiture->rotation()-360);
}

void fenGraphique::avancerVoiture(bool avance)
{
    double const deplacement=2;
    double rotation=voiture->rotation(), dx, dy;

    dx = qCos(qDegreesToRadians(rotation)) * deplacement;
    dy = qSin(qDegreesToRadians(rotation)) * deplacement;
    if(avance)
    {
        voiture->moveBy(dx, dy);
    }
    else
        voiture->moveBy(-dx, -dy);

}
