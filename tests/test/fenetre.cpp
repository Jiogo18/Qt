#include "fenetre.h"


fenetre::fenetre()
{
    setupUi(this);
    setWindowTitle("Fenetre de Test Qt");
    connect(b1, SIGNAL(clicked(bool)), this, SLOT(slot1()));
    connect(b2, SIGNAL(clicked(bool)), this, SLOT(slot2()));
    connect(b3, SIGNAL(clicked(bool)), this, SLOT(slot3()));
    connect(b4, SIGNAL(clicked(bool)), this, SLOT(slot4()));
    connect(b5, SIGNAL(clicked(bool)), this, SLOT(slot5()));
    //TimeExec = new QTime(QTime::currentTime());
    //TimeExec->start();

    /*QAudioInput *myQAudioInput = new QAudioInput;
    QAudioOutput *myQAudioOutput = new QAudioOutput;
    QIODevice *myDevice = myQAudioInput->start();
    myQAudioOutput->start(myDevice);
    myQAudioInput->stop();*/

    connect(slider1, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider2, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider3, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider4, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider5, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider6, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider7, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider8, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider9, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider10, &QSlider::valueChanged, this, &fenetre::slotImg);
    connect(slider11, &QSlider::valueChanged, this, &fenetre::slotImg);
    scene = new QGraphicsScene;
    graphicsView1->setScene(scene);
    imgItem = scene->addPixmap(img);
    centre = QPixmap(1,1);
    centre.fill(QColor(50,0,255));
    centreItem = scene->addPixmap(centre);
    imgItem->setZValue(0);
    centreItem->setZValue(1);
    //QRectF rect=graphicsView1->sceneRect();
    //graphicsView1->setSceneRect(rect.x(), rect.y(), rect.width()/8, rect.height()/8);
    graphicsView1->scale(4, 4);
    if(!img.load("C:/Qt/MesProg/jeux/map3D/ressourcepacks/defaut/textures/blocks/oak_log_top.png"))//"C:/Users/Jerome/Desktop/Capture + scan.PNG"))
    {
        qDebug("image non chargée");
    }
    else
        slotImg();

    show();
}

void fenetre::debug(QString str)
{
    if(str.isEmpty())
    {
        std::cout << std::endl;
    }
    else
        std::cout << "[" << QTime::currentTime().toString().toStdString() << "] : " << str.toStdString().c_str() << std::endl;
}

void fenetre::closeEvent(QCloseEvent *event)
{
    //TimeExec->restart();
    //deleteLater();
    //qApp->quit();
    event->accept();
}

void fenetre::slot1()
{
    /*QSystemTrayIcon *sticon;
    sticon = new QSystemTrayIcon(this);
    sticon->setIcon(QIcon("icone.png"));
    sticon->setVisible(1);
    sticon->showMessage("Alerte Windows", "Windows a detecté un virus", QSystemTrayIcon::Warning, 10000000);
    sticon->show();*/
    if(SendNotifyMessageA(HWND_BROADCAST, 0, 1, 1))
    {
        debug("notif 1 : ok");
    }
    else
        debug("notif 1 : non");

    if(SendNotifyMessageW(HWND_BROADCAST, 0, 1000, 1000))
    {
        debug("notif 2 : ok");
    }
    else
        debug("notif 2 : non");

    SendMessage(HWND_BROADCAST, RegisterWindowMessageA(valeur->text().toStdString().c_str()), 0, 0);

    debug("");
}

void fenetre::slot2()
{
    debug("\n\n\n\nslot1()");


    int i2,i3=0,nbI=0,max=0, min=-1;
    QTime timerS2_1;
    for(int i=0; i<10000; i++)
    {
        i2=0;
        timerS2_1.restart();
        timerS2_1.addMSecs(TimeExec->msec());
        while(timerS2_1.addMSecs(2) > TimeExec->currentTime())
        {
            i2++;
            //i2+=0;
            //QCoreApplication::processEvents();
        }
        if(i2>max)
            max = i2;
        if(i2<min || min==-1)
            min = i2;
        i3 += i2;
        nbI++;
    }
    valeur->setText(QString::number(i3) + " / " + QString::number(nbI) + " = " + QString::number(i3/nbI));
    valeur2->setText("max:"+QString::number(max) + "    min:"+QString::number(min));


}

void fenetre::slot3()
{
    int i=0;
    QTime timerS2_1(TimeExec->currentTime());
    while(timerS2_1.addSecs(60*5+35) > TimeExec->currentTime())
    {
        i++;
    }
    valeur->setText(QString::number(i) + " / " + QString::number(qPow(2,31)));
}

void fenetre::slot4()
{
    QTimer::singleShot(10000, this, SLOT(slot4_2()));
    myQAudioInput->start();
    b4->setText("Bouton 4 (lancé)");
}

void fenetre::slot4_2()
{
    myQAudioInput->stop();
    b4->setText("Bouton 4 (stop)");
}


void fenetre::slot5()
{
    quintptr a=valeur->text().toULongLong();
    quintptr b=valeur2->text().toULongLong();
    qDebug() << a << b << (a+b) << (a*b) << pow(a,b);
}

void fenetre::slotImg()
{
    /*QMatrix matrix(slider1->value()/100.0, slider2->value()/100.0,
                   slider3->value()/100.0, slider4->value()/100.0,
                   slider5->value()/100.0, slider6->value()/100.0);*/
    QTransform trans(slider1->value()/100.0, slider2->value()/100.0, slider3->value()/100000.0,
                         slider4->value()/100.0, slider5->value()/100.0, slider6->value()/100000.0,
                         slider7->value()/100.0, slider8->value()/100.0, slider9->value()/100.0);
    double pt0x=0, pt0y=0, pt0z=0,
            pt1x=slider1->value()/100.0,
            pt1y=slider2->value()/100.0,
            pt1z=slider3->value()/100.0,
            pt3x=slider4->value()/100.0,
            pt3y=slider5->value()/100.0,
            pt3z=slider6->value()/100.0,
            pt2x=slider7->value()/100.0,
            pt2y=slider8->value()/100.0,
            pt2z=slider9->value()/100.0;
    //transform=QTransform((pt3x-pt0x), (pt3y-pt0y), (pt3z-pt0z), (pt1x-pt0x), (pt1y-pt0y), (pt1z-pt0z), 0, 0, 1);
    //transform=QTransform(1, 0, (pt3y-pt2y)/16,0, 1, (pt2x-pt1x)/16, 0, 0, 1);
    trans=QTransform(1, 0, (pt3y-pt2y)/16/2,0, 1, (pt2x-pt1x)/16/2, 0, 0, 1);
    qDebug()<<"matrice1"<<trans;
    /*transform=QTransform((pt1x-pt0x)/transform.map(QPoint(img.width()-1, 0)).x()*16, (pt1y-pt0y), transform.m13(),
                         (pt3x-pt0x), (pt3y-pt0y)/transform.map(QPoint(0, img.height()-1)).y()*16, transform.m23(),
                         0, 0, 1);*/
    //sait pas trop comment faire avec 4 pts
    QPointF p1(pt0x*16, pt0y*16);
    QPointF p2(pt1x*16, pt1y*16);
    QPointF p3(pt2x*16, pt2y*16);
    QPointF p4(pt3x*16, pt3y*16);

    trans=QTransform();
    /*transform.translate(p1.x(), p1.y());
    transform.scale(p2.x() - p1.x(), p3.y() - p1.y());
    transform.shear((p3.x() - p1.x()) / transform.m11(),
                    (p2.y() - p1.y()) / transform.m22());
    // relative position of the 4th point in the transformed coordinate system:
    qreal px = transform.inverted().map(p4).x();
    qreal py = transform.inverted().map(p4).y();
    // this defines the perspective distortion:
    qreal y = 1 + (py - 1) / px;
    qreal x = 1 + (px - 1) / py;
    QTransform persp(1/y, 0, 1/y-1,
                     0, 1/x, 1/x-1,
                     0, 0, 1);
    transform = persp * transform;*/
    QPolygonF polyImg(QRectF(img.rect()));
    polyImg.removeLast();
    if(QTransform::quadToQuad(polyImg, QPolygonF({p1,p2,p3,p4}),trans))
        qDebug()<<"transfo possible";
    //fonctionne !
    qDebug()<<"matrice2"<<trans;
    qDebug()<<"pt0"<<pt0x<<pt0y<<pt0z
            <<"pt1"<<pt1x<<pt1y<<pt1z
            <<"pt2"<<pt2x<<pt2y<<pt2z
            <<"pt3"<<pt3x<<pt3y<<pt3z;
    QImage img2=img.transformed(trans).toImage();

    QPointF pt0trans=trans.map(QPointF(0, 0));
    QPointF pt1trans=trans.map(QPointF(img.width(), 0));
    QPointF pt2trans=trans.map(QPointF(img.width(), img.height()));
    QPointF pt3trans=trans.map(QPointF(0, img.height()));
    double xMin=pt0trans.x(), xMax=pt0trans.x(), yMin=pt0trans.y(), yMax=pt0trans.y();
    QPointF temp;
    for(int i=1; i<=3; i++)
    {
        if(i==1) temp=pt1trans;
        if(i==2) temp=pt2trans;
        if(i==3) temp=pt3trans;
        if(temp.x() < xMin) xMin=temp.x();
        if(xMax < temp.x()) xMax=temp.x();
        if(temp.y() < yMin) yMin=temp.y();
        if(yMax < temp.y()) yMax=temp.y();
    }
    QPointF centre=trans.map(QPointF((img.width())/2,(img.height())/2));
    img2.setPixelColor(centre.x()-xMin, centre.y()-yMin, Qt::red);
    img2.setPixelColor((img2.width()-1)/2, (img2.height()-1)/2, Qt::green);
    //lbl1->setPixmap(QPixmap::fromImage(img2));

    double moyenneX=moyenne({pt0x,pt1x,pt2x,pt3x});
    double moyenneY=moyenne({pt0y,pt1y,pt2y,pt3y});
    imgItem->setPixmap(QPixmap::fromImage(img2));
    imgItem->setX(slider10->value()+moyenneX - (centre.x()-xMin + moyenneX));
    imgItem->setY(slider11->value()+moyenneY - (centre.y()-yMin) + moyenneY);
    //imgItem->setScale(imgItem->scale()*4);//=img2.scaled(img2.width()*4,img2.height()*4);//agrandit un peu
    centreItem->setX(slider10->value());
    centreItem->setY(slider11->value());
    //je veux que rouge soit sur bleu
    //=> je veux que le centre de la face soit sur le bleu
}

double fenetre::moyenne(QList<double> l)
{
    double retour=0;
    for(int i=0; i<l.size(); i++)
        retour += l.at(i);
    return retour/l.size();
}


/*
void fenetre::actWindow()
{
    qDebug("\n\n\n\n\n");
    HWND cur = GetForegroundWindow();
    if(true)
    {
        LPTSTR name[100];
        GetClassName(cur, *name, 100);
        std::cout << cur << " " << name << endl;
    }
}
*/
