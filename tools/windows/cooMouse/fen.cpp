#include "fen.h"

fen::fen()
{
    setWindowTitle("Test de TimerCoordonnées");

//config de la fenetre
    {
    QVBoxLayout *layout = new QVBoxLayout;

    QFormLayout *lConfig = new QFormLayout;
    QLabel *lx = new QLabel("X : ");
    x = new QSpinBox;
    x->setMaximum(GetSystemMetrics( SM_CXSCREEN ));//resolution X
    x->setValue(x->maximum());
    lConfig->addRow(lx, x);

    QLabel *ly = new QLabel("Y : ");
    y = new QSpinBox;
    y->setMaximum(GetSystemMetrics( SM_CYSCREEN ));
    y->setValue(y->maximum());
    lConfig->addRow(ly, y);

    QLabel *lt = new QLabel("Temps (secondes) : ");
    temps = new QSpinBox;
    temps->setMaximum(100);
    temps->setValue(10);
    lConfig->addRow(lt, temps);
    connect(temps, SIGNAL(valueChanged(int)), this, SLOT(actuTemps()));
    layout->addLayout(lConfig);


    posRelative = new QCheckBox("Positions relative selon le deplacement de la sourie.");
    posRelative->setChecked(false);
    connect(posRelative, SIGNAL(stateChanged(int)), this, SLOT(actuPosRelative()));
    layout->addWidget(posRelative);

    souris = new QLabel("La souris se trouve en (x,y)");
    layout->addWidget(souris);


    bLancer = new QPushButton("Lancer");
    connect(bLancer, SIGNAL(clicked(bool)), this, SLOT(DeplaceMouse()));
    layout->addWidget(bLancer);

    setLayout(layout);
    }

    actuTemps();
    setMouseTracking(true);

    xRelatif=0;
    yRelatif=0;

    TimerCoo = new QTimer;
    TimerCoo->setInterval(100);
    connect(TimerCoo, SIGNAL(timeout()), this, SLOT(actuMouse()));
    TimerCoo->start();
}



void fen::actuTemps()
{
    bLancer->setText("Lancer dans " + QString::number( temps->value() ) + " secondes");
}

void fen::DeplaceMouse()
{
    Sleep(temps->value()*1000);
    SetCursorPos(x->value(),y->value());
}


void fen::actuMouse()
{
    int x, y;
    x = cursor().pos().x();
    y = cursor().pos().y();
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    xRelatif += x-screenGeometry.width()/2;
    yRelatif += y-screenGeometry.height()/2;

    if(posRelative->isChecked())
    {
        souris->setText(QString("Position actuelle de la souris : (") + QString::number(x) + QString(", ") + QString::number(y) + QString(")\nPosition relatives : (" )+ QString::number(xRelatif) + QString(", ") + QString::number(yRelatif) + QString(")"));
        SetCursorPos(screenGeometry.width()/2, screenGeometry.height()/2);
    }
    else
    {
        souris->setText(QString("Position actuelle de la souris : (") + QString::number(x) + QString(", ") + QString::number(y) + QString(")"));
    }
}

void fen::actuPosRelative()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    if(posRelative->isChecked())
        SetCursorPos(screenGeometry.width()/2, screenGeometry.height()/2);
    xRelatif=0;
    yRelatif=0;
}
