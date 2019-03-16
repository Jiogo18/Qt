#ifndef FENGRAPHIQUE_H
#define FENGRAPHIQUE_H

#include <QtWidgets>
#include <QTimer>
#include <QtMath>


class fenGraphique : public QWidget
{
    Q_OBJECT
    public:
        fenGraphique();

    public slots:
        void keyPressEvent(QKeyEvent *event);


    private slots:
        void avancerVoiture(bool avance=true);


    private:
        QGraphicsScene *scene;
        QGraphicsView *view;
        QGraphicsPixmapItem *voiture;

};

#endif // FENGRAPHIQUE_H
