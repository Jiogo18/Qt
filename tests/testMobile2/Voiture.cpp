#include "Voiture.h"

Voiture::Voiture(const Voiture &v) : QQuickPaintedItem(v.parentItem())
{
    operator=(v);
    setAcceptedMouseButtons(Qt::AllButtons);
}
Voiture::Voiture(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setImage(QImage(":/image/image/voiture.png"), QSizeF(175.8,98));//879*490
    setAcceptedMouseButtons(Qt::AllButtons);
}

Voiture::Voiture(QQmlApplicationEngine *engine, qreal x, qreal y, qreal width, qreal height)
{
    Voiture *me = QmlItem::createItem<Voiture>(engine, "Voit");
    operator=(me);
    delete me;
    setSize(QSizeF(width, height));
    setX(x); setY(y);
    setAcceptedMouseButtons(Qt::AllButtons);
}

Voiture *Voiture::create(QQmlApplicationEngine *engine)
{return QmlItem::createItem<Voiture>(engine, "Voit");}

Voiture &Voiture::operator=(const Voiture &v)
{
    setParent(v.parent());
    setParentItem(v.parentItem());
    setPosition(v.position());
    setRotation(v.rotation());
    setImage(v.image(), v.size());
    return *this;
}

Voiture &Voiture::operator=(const Voiture *v)
{
    setParent(v->parent());
    setParentItem(v->parentItem());
    setPosition(v->position());
    setRotation(v->rotation());
    setImage(v->image(), v->size());
    return *this;
}

Voiture::~Voiture() {}
void Voiture::paint(QPainter *painter)
{
    if(m_img.isNull()) return;
    painter->drawImage(0,0,m_img);
}

//positions du milieu !
qreal Voiture::x() const {return QQuickPaintedItem::x() + QQuickPaintedItem::width()/2;}
void Voiture::setX(qreal x) {QQuickPaintedItem::setX(x - QQuickPaintedItem::width()/2);}
qreal Voiture::y() const {return QQuickPaintedItem::y() + QQuickPaintedItem::height()/2;}
void Voiture::setY(qreal y) {QQuickPaintedItem::setY(y - QQuickPaintedItem::height()/2);}

const QImage &Voiture::image() const {return m_baseImg;}
void Voiture::setImage(QImage img, QSizeF size)
{
    m_baseImg=img;
    if(size.isEmpty())
        size=img.size();
    QQuickPaintedItem::setSize(size);
    actuImg();
}

void Voiture::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(oldGeometry);//fonction void qui l'utilise pas (pour pas avoir de waring)
    QPointF goodGeometry=newGeometry.center();
    QPointF min(0,0), max(200,200);
    if(goodGeometry.x() < min.x())
        goodGeometry.setX(min.x());
    if(goodGeometry.x() > max.x())
        goodGeometry.setX(max.x());
    if(goodGeometry.y() < min.y())
        goodGeometry.setY(min.y());
    if(goodGeometry.y() > max.y())
        goodGeometry.setY(max.y());
    if(newGeometry.center() != goodGeometry)
    {//le pt du milieu est pas dans le cadre
        goodGeometry.setX(goodGeometry.x() - width()/2);
        goodGeometry.setY(goodGeometry.y() - height()/2);
        setPosition(goodGeometry);
    }
    if(newGeometry.size() != m_img.size())
        actuImg();
    emit voitChanged();
}

void Voiture::turnCar(qreal rot)
{
    QQuickPaintedItem::setRotation(QQuickPaintedItem::rotation() + rot);
    emit turned();
}
void Voiture::moveCar(qreal distance)
{
    qreal x=QQuickPaintedItem::x(), y=QQuickPaintedItem::y();
    x += qCos(qDegreesToRadians(QQuickPaintedItem::rotation())) * distance;
    y += qSin(qDegreesToRadians(QQuickPaintedItem::rotation())) * distance;
    QQuickPaintedItem::setPosition(QPointF(x,y));
    emit moved();
}


void Voiture::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}
void Voiture::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //moveCar(10);
    //turnCar(10);
    forward();
    turnLeft();
}


void Voiture::actuImg()
{
    QImage lastImg=m_img;
    m_img = m_baseImg.scaled(QQuickPaintedItem::size().toSize());
    if(lastImg != m_img)
        update();
}


QDebug operator<<(QDebug debug, const Voiture &v)
{
    QDebugStateSaver saver(debug);
    debug << "Voiture(" << v.position() << v.size() << v.image() <<")";
    return debug;
}
