#include "fenGraphique.h"

fenGraphique::fenGraphique()
{//set
    {
    scene = new QGraphicsScene;

    trait = new QPen(QColor("Green"));
    trait->setWidth(10);

    scene->addRect(QRectF(0, 0, 1, 1));

    view = new QGraphicsView(scene);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
    }

    points = new QVector<int, int>;
    rotationH = 0;
    rotationV = 0;
    actuCube();

}

void fenGraphique::keyPressEvent(QKeyEvent *event)
{
}

void fenGraphique::actuCube()
{

}
void fenGraphique::calcPoints()
{
    const QVector<int ,int> pointsBase;

}
