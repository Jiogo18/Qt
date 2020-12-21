#include "fenetre.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Affichage d'un objet 3D");
    layout = new QHBoxLayout;
    scene = new GraphicScene3D;
    lblScene = new QLabel(this);
    bRefresh = new QPushButton("Rafraichir");
    sX = new QDoubleSpinBox;
    sX->setMaximum(1000);
    sX->setMinimum(-sX->maximum());
    sX->setValue(0);
    sY = new QDoubleSpinBox;
    sY->setMinimum(-sY->maximum());
    sY->setMaximum(1000);
    sY->setValue(0);
    sZ = new QDoubleSpinBox;
    sY->setMaximum(1000);
    sZ->setMinimum(-sZ->maximum());
    sZ->setValue(0);
    sRX = new QSpinBox;
    sRX->setMaximum(360);
    sRX->setMinimum(-sRX->maximum());
    sRX->setValue(0);
    sRY = new QSpinBox;
    sRY->setMaximum(360);
    sRY->setMinimum(-sRY->maximum());
    sRY->setValue(0);
    sZoom = new QDoubleSpinBox;
    sZoom->setMinimum(0);
    sZoom->setMaximum(1000);
    sZoom->setValue(1);
    //sZoom->setValue(200);//tests
    sPerspective = new QDoubleSpinBox;
    sPerspective->setMinimum(0);
    sPerspective->setMaximum(100);
    sPerspective->setValue(0.62);//trouvé avec sheet, minecraft perspective (avant : 4)
    connect(bRefresh, &QPushButton::clicked, this, &fenetre::bRefreshPress);
    layoutOption = new QFormLayout;
    layoutOption->addRow(new QLabel("X"), sX);
    layoutOption->addRow(new QLabel("Y"), sY);
    layoutOption->addRow(new QLabel("Z"), sZ);
    //layoutOption->addRow(new QLabel("rX"), sRX);
    //layoutOption->addRow(new QLabel("rY"), sRY);
    layoutOption->addRow(new QLabel("Zoom"), sZoom);
    layoutOption->addRow(new QLabel("Perspective"), sPerspective);
    //layoutOption->addWidget(bRefresh);
    //layout->addWidget(lblScene);
    layout->addLayout(layoutOption);
    //setLayout(layout);
    //block de 1*1
    scene->addCube(0, 0, 0, "textures/blocks/oak_log_top.png", "textures/blocks/oak_log_top.png", "textures/blocks/oak_log.png");
    scene->addCube(2, 0, 0, "textures/blocks/acacia_log_top.png", "textures/blocks/acacia_log_top.png", "textures/blocks/acacia_log.png");
    scene->addCube(2, 1, 0, "textures/blocks/birch_log_top.png", "textures/blocks/birch_log_top.png", "textures/blocks/birch_log.png");
    scene->addCube(2, 0, 1, "textures/blocks/acacia_log_top.png", "textures/blocks/acacia_log_top.png", "textures/blocks/acacia_log.png");

    //block de 2*2
    //scene->addObj(obj3D({QPoint3D(1,1,1), QPoint3D(-1,1,1), QPoint3D(-1,-1,1), QPoint3D(1,-1,1), QPoint3D(1,1,-1), QPoint3D(-1,1,-1), QPoint3D(-1,-1,-1), QPoint3D(1,-1,-1)}, 0, 0, 0, 0, 0));

    listeKey.clear();
    timerKey = new QTimer;
    connect(timerKey, &QTimer::timeout, this, &fenetre::refreshCoo);
    timerKey->start(50);//fonctionne à 20/s

    show();
    resize(700, 700);
    changement=true;
    bRefreshPress();
    timerRefresh = new QTimer;
    connect(timerRefresh, &QTimer::timeout, this, &fenetre::bRefreshPress);
    timerRefresh->start(50);//pourra etre accelere (5/sec)
}

void fenetre::bRefreshPress()
{

    lblScene->setGeometry(0, 0, width(), height());
    if(false)
    {
        lblScene->setPixmap(
                    scene->refresh(lblScene->width(), lblScene->height(), sX->value(), sY->value(), sZ->value(), sRX->value(), sRY->value(), sZoom->value(), sPerspective->value())
                    );
    }
    else
    {
        if(!changement && cursor().pos() == cooMouseLast)
            return;//si tu as pas de changements et que la sourie à pas bougée
        changement=false;
        cooMouseLast=cursor().pos();
        int rX=-(1920/2-cooMouseLast.x())/5;//taille de l'écran
        int rY=-(1080/2-cooMouseLast.y())/5;//trop sensible
        if(rY < -90) rY = -90;
        if(rY > 90) rY = 90;
        rY-=90;
        sRX->setValue(rX);//on l'actu pour le avancer
        sRY->setValue(rY);
        lblScene->setPixmap(
                    scene->refresh(lblScene->width(), lblScene->height(),
                                   sX->value(), sY->value(), sZ->value(),
                                   rX, rY,
                                   sZoom->value(), sPerspective->value()));
    }
}

void fenetre::resizeEvent(QResizeEvent *event)
{
    lblScene->setGeometry(0, 0, event->size().width(), event->size().height());
    //après il faut rafraichir l'image
    changement=true;
}

void fenetre::keyPressEvent(QKeyEvent *event)
{
    if(!listeKey.contains(event->key()))
        listeKey.append(event->key());
}

void fenetre::keyReleaseEvent(QKeyEvent *event)
{listeKey.removeAll(event->key());}

void fenetre::refreshCoo()
{
    QPoint3D pt(0,0,0);
    for(int i=0; i<listeKey.size(); i++)
    {
        switch (listeKey.at(i))
        {
        case Qt::Key_Z:
            avanceZ(0.1);
            break;
        case Qt::Key_S:
            avanceZ(-0.1);
            break;
        case Qt::Key_D:
            avanceX(-0.1);//inverse entre d et q normalement
            break;
        case Qt::Key_Q:
            avanceX(0.1);
            break;
        case Qt::Key_Space:
            avanceY(0.1);
            break;
        case Qt::Key_Shift:
            avanceY(-0.1);
            break;
        }
    }
}

void fenetre::avanceX(double speed)
{
    /*QPoint3D pt(speed,0,0);
    QPoint3D pt0(0,0,0);*/
    //pt=scene->calcPoint(pt);
    //pt0=scene->calcPoint(pt0);
    /*plusX(pt0.x()-pt.x());
    plusZ(pt0.z()-pt.z());*/
    //comme axe Z mais avec 90° de diff
    double x=obj3D::rotation2(speed,0, sRX->value()-90);
    double y=obj3D::rotation1(speed,0, sRX->value()-90);
    plusX(x);
    plusZ(y);
}

void fenetre::avanceY(double speed)
{
    /*QPoint3D pt(0,speed,0);
    QPoint3D pt0(0,0,0);
    pt=scene->calcPoint(pt);
    pt0=scene->calcPoint(pt0);
    plusY(pt0.y()-pt.y());*/
    plusY(speed);
}

void fenetre::avanceZ(double speed)
{
    /*QPoint3D pt(0,0,speed);
    QPoint3D pt0(0,0,0);
    pt=scene->calcPoint(pt);
    pt0=scene->calcPoint(pt0);
    plusX(pt0.x()-pt.x());
    plusZ(pt0.z()-pt.z());*/
    double x=obj3D::rotation2(speed,0, sRX->value());
    double y=obj3D::rotation1(speed,0, sRX->value());
    plusX(x);
    plusZ(y);
}

void fenetre::plusX(double x) {sX->setValue(sX->value()+x); changement=true;}
void fenetre::plusY(double y) {sY->setValue(sY->value()+y); changement=true;}
void fenetre::plusZ(double z) {sZ->setValue(sZ->value()+z); changement=true;}
