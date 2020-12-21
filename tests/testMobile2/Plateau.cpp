#include "Plateau.h"

Plateau::Plateau(const Plateau &p) : QQuickPaintedItem(p.parentItem())
{
    operator=(p);
}

Plateau::Plateau(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_listButton = new QList<Button*>;
}

Plateau::Plateau(QQmlApplicationEngine *engine)
{
    operator =(QmlItem::createItem<Plateau>(engine, "Plateau"));
    m_engine=engine;
    setSize(QSizeF(200, 200));
    m_listButton = new QList<Button*>;
    m_listButton->append(new Button(m_engine, 200, 0, 100, 50, "foo"));

    m_Voit=new Voiture(m_engine, 100, 100, 200, 100);
    m_buttonLeft = new Button(m_engine, 50, 300, 50, 50, "<");
    m_buttonRight = new Button(m_engine, 150, 300, 50, 50, ">");
    m_buttonUp = new Button(m_engine, 100, 250, 50, 50, "/\\");
    m_buttonDown = new Button(m_engine, 100, 300, 50, 50, "\\/");
    connect(m_buttonLeft, &Button::clicked, m_Voit, &Voiture::turnLeft);
    connect(m_buttonRight, &Button::clicked, m_Voit, &Voiture::turnRight);
    connect(m_buttonUp, &Button::clicked, m_Voit, &Voiture::forward);
    connect(m_buttonDown, &Button::clicked, m_Voit, &Voiture::back);
}

Plateau &Plateau::operator=(const Plateau &p)
{
    setSize(p.size());
    setPosition(p.position());
    m_engine = p.m_engine;
    m_listButton = p.m_listButton;
    m_Voit = p.m_Voit;
    m_buttonLeft=p.m_buttonLeft; m_buttonRight=p.m_buttonRight;
    m_buttonUp=p.m_buttonUp; m_buttonDown=p.m_buttonDown;
    return *this;
}

Plateau::~Plateau()
{
    while (!m_listButton->isEmpty())
        delete m_listButton->takeFirst();
    delete m_listButton;
    delete m_Voit;
    delete m_buttonLeft; delete m_buttonRight; delete m_buttonUp; delete m_buttonDown;
}

void Plateau::paint(QPainter *painter)
{
    /*for(int i=0; i<m_listVoit->size(); i++)
        m_listVoit->at(i)->paint(painter);*/
    painter->drawRect(0, 0, QQuickPaintedItem::width()-1, QQuickPaintedItem::height()-1);
}
