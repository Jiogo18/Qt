#include "fenetre.h"

fenetre::fenetre(QWidget *parent) : QGraphicsView(parent)
{
    map = new map3D();

    gui = new GUI(map, this);
    setScene(gui);
    connect(gui, &GUI::workStarted, this, &fenetre::workStarted);
    connect(gui, &GUI::workFinished, this, &fenetre::workFinished);
//    connect(map, &map3D::PBMax, this, &fenetre::setPBMax);
//    connect(map, &map3D::PBValue, this, &fenetre::setPBValue);

    //setCursor(Qt::BlankCursor);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //winTaskbarButt = new QWinTaskbarButton(this);

    show();
    setMinimumSize(50,50);
    setGeometry(QRect(geometry().topLeft(), QSize(150, 100)));

    //moveMouseMidWindow();
    //posMouse = QPoint(0, 0);


    lastRefreshTime = 0;
    actualise();

    timerRefresh.setSingleShot(true);
    QObject::connect(&timerRefresh, &QTimer::timeout, this, &fenetre::actualise);
}

fenetre::~fenetre()
{
    if(map != nullptr) delete map;
    if(gui != nullptr) delete gui;
    //if(button != nullptr) delete button;
}

void fenetre::actualise()
{
    gui->setSceneSize(geometry().size());
    lastRefreshTime = QDateTime::currentMSecsSinceEpoch();
    gui->refresh();
}


void fenetre::keyPressEvent(QKeyEvent *event)
{
    const Pos3D &previousPos = map->getClient()->getPos();
    switch (event->key()) {
    case Qt::Key_Z:
        map->moveFront();
        break;
    case Qt::Key_Q:
        map->moveLeft();
        break;
    case Qt::Key_S:
        map->moveBack();
        break;
    case Qt::Key_D:
        map->moveRight();
        break;
    case Qt::Key_Shift:
        map->moveDown();
        break;
    case Qt::Key_Space:
        map->moveUp();
        break;
    case Qt::Key_Left:
        map->moveRX(-MouseSensibility/10);
        break;
    case Qt::Key_Right:
        map->moveRX(MouseSensibility/10);
        break;
    case Qt::Key_Up:
        map->moveRZ(MouseSensibility/10);
        break;
    case Qt::Key_Down:
        map->moveRZ(-MouseSensibility/10);
        break;
    case Qt::Key_F5:
        actualise();
        break;
    }
    qDebug() << "pos client" << map->getClient()->getPos();
    if(!(map->getClient()->getPos() == previousPos) && lastRefreshDuration < 300) {
        //si on a bougé et que ça prend pas trop de temps
        if(lastRefreshTime + 400 < QDateTime::currentMSecsSinceEpoch() ) {
            //si ça fait trop longtemps qu'on a pas update
            timerRefresh.stop();
            actualise();
        }
        else {
            //actu dans 100 msec ou plus si on reviens ici
            timerRefresh.start(lastRefreshDuration / 2);
        }
    }
}
//void fenetre::mouseMoveEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event)
//    //map->getClient()->moveRX((MidWindow().x() - event->x())/MouseSensibility);
//    //map->getClient()->moveRY((MidWindow().y() - event->y())/MouseSensibility);
//    //moveMouseMidWindow();
//}

void fenetre::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(gui) gui->setSceneSize(geometry().size());
}
//QPoint fenetre::MidWindow() { return QPoint(width()/2, height()/2); }
//void fenetre::moveMouseMidWindow() { QCursor::setPos(MidWindow() + QWidget::pos()); }

void fenetre::workStarted()
{
    setCursor(Qt::WaitCursor);
    //button->progress()->setVisible(true);
}
void fenetre::workFinished() {
    setCursor(Qt::ArrowCursor);
    //button->progress()->setVisible(false);
    lastRefreshDuration = QDateTime::currentMSecsSinceEpoch() - lastRefreshTime;
}
//void fenetre::setPBMax(int max) { /*button->progress()->setMaximum(max);*/ }
//void fenetre::setPBValue(int value) { /*button->progress()->setValue(value);*/ }


