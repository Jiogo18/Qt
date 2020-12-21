#ifndef BUTTON_H
#define BUTTON_H

#include <QString>
#include <QObject>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include "QmlItem.h"

class Button : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    Button(const Button &b);
    Button(QQuickItem *parent=nullptr);
    Button(QQmlApplicationEngine *engine, qreal x, qreal y, qreal width, qreal height, QString text);
    Button &operator=(const Button &b);
    Button &operator=(const Button *b);
    ~Button();
    void paint(QPainter *painter);

    QString text() const;
    void setText(const QString &text);
    QRectF geometry() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void hoverEnterEvent(QHoverEvent *event);
    void hoverLeaveEvent(QHoverEvent *event);


signals:
    void clicked();

private:
    QString m_text;
    bool m_pressed=false;
    bool m_hovered=false;
};

#endif // BUTTON_H
