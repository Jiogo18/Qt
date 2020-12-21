#include "Button.h"

Button::Button(const Button &b) : QQuickPaintedItem(b.parentItem())
{
    operator =(b);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
}
Button::Button(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setSize(QSizeF(100, 50));//size de base
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
}

Button::Button(QQmlApplicationEngine *engine, qreal x, qreal y, qreal width, qreal height, QString text)
{
    Button *me = QmlItem::createItem<Button>(engine, "ButtonM");
    me->setSize(QSizeF(width, height));
    me->setX(x); me->setY(y);
    me->setText(text);
    operator=(me);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
}

Button &Button::operator=(const Button &b)
{
    setParent(b.parent());
    setParentItem(b.parentItem());
    setSize(b.size());
    setPosition(b.position());
    setRotation(b.rotation());
    setText(b.text());
    return *this;
}
Button &Button::operator=(const Button *b)
{
    setParent(b->parent());
    setParentItem(b->parentItem());
    setSize(b->size());
    setPosition(b->position());
    setRotation(b->rotation());
    setText(b->text());
    return *this;
}
Button::~Button() {}

void Button::paint(QPainter *painter)
{
    QRectF geoPainted=geometry();
    qreal width=3;//taille du crayon
    geoPainted.setX(geoPainted.x()+width);
    geoPainted.setY(geoPainted.y()+width);
    geoPainted.setWidth(geoPainted.width() - 2*width);
    geoPainted.setHeight(geoPainted.height() - 2*width);
    painter->setPen(QPen(Qt::gray, width));
    if(m_pressed) {
        painter->setBrush(QBrush(QColor("#282828")));//très sombre
    } else if(m_hovered) {
        painter->setBrush(QBrush(QColor("#808080")));//clair
    } else
        painter->setBrush(QBrush(QColor("#505050")));//sombre
    painter->drawEllipse(geoPainted);
    painter->setPen(QPen(Qt::white));
    painter->drawText(geoPainted, m_text, QTextOption(Qt::AlignCenter));
}


QString Button::text() const {return m_text;}
void Button::setText(const QString &text) {m_text=text;}

QRectF Button::geometry() const
{return QRectF(QPointF(0, 0), QQuickPaintedItem::size());}



void Button::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_pressed=true;
    update();
}
void Button::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_pressed=false;
    update();
    emit clicked();
}
void Button::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    m_hovered=true;
    update();
}
void Button::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    m_hovered=false;
    update();
}
