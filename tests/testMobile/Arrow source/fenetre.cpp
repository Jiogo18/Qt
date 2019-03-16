#include "fenetre.h"

fenetre::fenetre()
{
    //set fenetre
    {
        setWindowTitle("Mouse-ZQSD");

        sliderPixMoveLbl = new QLabel("Sensibilitée : "+QString::number(0));
        sliderPixMove = new QSlider(Qt::Horizontal);
        sliderPixMove->setMinimum(1);
        sliderPixMove->setMaximum(100);
        sliderPixMove->setValue(10);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(sliderPixMoveLbl);
        layout->addWidget(sliderPixMove);
        setLayout(layout);


        move = new QTimer;
        connect(sliderPixMove, SIGNAL(valueChanged(int)), this, SLOT(sliderChange(int)));
        sliderChange(10);
        connect(move, SIGNAL(timeout()), this, SLOT(deplace()));
        move->start();
    }

    xP = false;
    xM = false;
    yP = false;
    yM = false;

}

void fenetre::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Q:
            xM = true;
            break;
        case Qt::Key_D:
            xP = true;
            break;
        case Qt::Key_Z:
            yM = true;
            break;
        case Qt::Key_S:
            yP = true;
            break;
    }
}

void fenetre::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Q:
            xM = false;
            break;
        case Qt::Key_D:
            xP = false;
            break;
        case Qt::Key_Z:
            yM = false;
            break;
        case Qt::Key_S:
            yP = false;
            break;
    }
}



void fenetre::sliderChange(int value)
{
    move->setInterval(100/value);
    sliderPixMoveLbl->setText("Sensibilitée : "+QString::number(value));
}

void fenetre::deplace()
{
    int x = cursor().pos().x();
    int y = cursor().pos().y();

    if(xP!=xM)
    {
        if(xP==true)
        {
            if(x==QApplication::desktop()->screenGeometry().width()-1)
                xP=false;
            x++;
        }
        else if(xM==true)
        {
            if(x==0)
                xM=false;
            x--;
        }
    }

    if(yP!=yM)
    {
        if(yP==true)
        {
            if(y==QApplication::desktop()->screenGeometry().height()-1)
                yP=false;
            y++;
        }
        else if(yM==true)
        {
            if(y==0)
                yM=false;
            y--;
        }
    }

    cursor().setPos(x, y);
}
