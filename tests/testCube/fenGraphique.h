#ifndef FENGRAPHIQUE_H
#define FENGRAPHIQUE_H

#include <QtWidgets>
//#include <QTimer>
#include <QtMath>


class fenGraphique : public QWidget
{
    Q_OBJECT
    public:
        fenGraphique();

    public slots:
        void keyPressEvent(QKeyEvent *event);


    private slots:
        void actuCube();
        void calcPoints();


    private:
        QGraphicsScene *scene;
        QGraphicsView *view;
        QPen *trait;
        QVector<int, int> *points;
        int rotationH;
        int rotationV;

        QGraphicsPixmapItem *voiture;

};

#endif // FENGRAPHIQUE_H
