#include "fenetre.h"

fenetre::fenetre(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(QRect(QPoint(0, 0), geometry().size()));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    button = new QWinTaskbarButton(this);

    show();
    setMinimumSize(50,50);
    setGeometry(QRect(geometry().topLeft(), QSize(150, 100)));


    //setCursor(Qt::BlankCursor);
    moveMouseMidWindow();
    posMouse = QPoint(0, 0);


    map = new map3D(scene);
    connect(map, &map3D::finished, this, &fenetre::workFinished);
    connect(map, &map3D::PBMax, this, &fenetre::setPBMax);
    connect(map, &map3D::PBValue, this, &fenetre::setPBValue);
    actualise();

    timerRefresh.setInterval(10000);
    //QObject::connect(&timerRefresh, &QTimer::timeout, this, &fenetre::actualise);
    //timerRefresh.start();
}

fenetre::~fenetre()
{
    if(map != nullptr) delete map;
    if(scene != nullptr) delete scene;
    if(button != nullptr) delete button;
}

void fenetre::actualise()
{
    workStarted();
    scene->setSceneRect(QRect(QPoint(0, 0), geometry().size()));
    map->refresh();
}


void fenetre::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Z:
        map->getClient()->moveFront();
        break;
    case Qt::Key_Q:
        map->getClient()->moveLeft();
        break;
    case Qt::Key_S:
        map->getClient()->moveBack();
        break;
    case Qt::Key_D:
        map->getClient()->moveRight();
        break;
    case Qt::Key_Shift:
        map->getClient()->moveDown();
        break;
    case Qt::Key_Space:
        map->getClient()->moveUp();
        break;
    case Qt::Key_Left:
        map->getClient()->moveRX(-MouseSensibility/10);
        break;
    case Qt::Key_Right:
        map->getClient()->moveRX(MouseSensibility/10);
        break;
    case Qt::Key_Up:
        map->getClient()->moveRZ(MouseSensibility/10);
        break;
    case Qt::Key_Down:
        map->getClient()->moveRZ(-MouseSensibility/10);
        break;
    case Qt::Key_F5:
        actualise();
        break;
    }
    qDebug() << "pos client" << map->getClient()->getPos();
}
void fenetre::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //map->getClient()->moveRX((MidWindow().x() - event->x())/MouseSensibility);
    //map->getClient()->moveRY((MidWindow().y() - event->y())/MouseSensibility);
    //moveMouseMidWindow();
}

void fenetre::resizeEvent(QResizeEvent *event)
{
    if(map != nullptr)
        map->resizeEvent(event->size());
}
QPoint fenetre::MidWindow() { return QPoint(width()/2, height()/2); }
void fenetre::moveMouseMidWindow() { QCursor::setPos(MidWindow() + QWidget::pos()); }

void fenetre::workStarted()
{
    setCursor(Qt::WaitCursor);
    button->progress()->setVisible(true);
}
void fenetre::workFinished() {
    setCursor(Qt::ArrowCursor);
    button->progress()->setVisible(false);
}
void fenetre::setPBMax(int max) { button->progress()->setMaximum(max); }
void fenetre::setPBValue(int value) { button->progress()->setValue(value); }


