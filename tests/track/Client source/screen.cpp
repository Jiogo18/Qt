#include "screen.h"
#include "ui_screen.h"

screen::screen()
{
    setupUi(this);
    setMouseTracking(true);
}


void screen::setScreen(QPixmap image, int x, int y)
{

    vue->setGeometry(0, 0, width(), height());
    float x2 = x, y2 = y, ix = image.width(), iy = image.height(), vx = vue->width(), vy = vue->height();
    float sourisX = x2/ix*vx;
    float sourisY = y2/iy*vy;

    souris->setPixmap(QPixmap("C:/Qt/MesProg/track/clientTrack/cursor.png").scaled(width()/40, height()/40));

    souris->setGeometry(sourisX, sourisY, width()/40, height()/40);
    QSize size(vue->width(), vue->height());
    //std::cout << "ancienne resolution : " << image.width() << "," << image.height() << std::endl;
    image = image.scaled(size);
    //std::cout << "nouvelle resolution : " << image.width() << "," << image.height() << std::endl;
    vue->setPixmap(image);

}

QPoint screen::getMouse()
{
    QPoint position =  cursor().pos();
    position.setX(  (position.x() - (pos().x()) ) *1000 / vue->width()); //donne les coo par rapport a la fenetre
    position.setY(  (position.y() - (pos().y()+30)  ) *1000 / vue->height());//x n'a pas de bordure alors que y en a une de 30 pixels
    if(position.x() < 0)
        position.setX(0);
    if(position.x() > 1000)
        position.setX( 1000 );
    if(position.y() < 0)
        position.setY(0);
    if(position.y() > 1000)
        position.setY( 1000 );
    return position;
}


void screen::mousePressEvent(QMouseEvent *event)
{
    emit envoyerClick(event);
}
void screen::mouseReleaseEvent(QMouseEvent *event)
{
    emit envoyerClick(event);
}



void screen::keyReleaseEvent(QKeyEvent *event)
{
    emit envoyerTouche(event);
}
