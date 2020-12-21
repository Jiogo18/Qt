#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    TActuSec.setInterval(1000);
    connect(&TActuSec, &QTimer::timeout, this, &fenetre::actuSec);
    TActuSec.start();
    TActuCooMouse.setInterval(50);
    connect(&TActuCooMouse, &QTimer::timeout, this, &fenetre::actuCooMouse);
    TActuCooMouse.start();

    connect(ui->bSaveItem, &QPushButton::clicked, this, &fenetre::saveCase);
    connect(ui->bSaveInfoBulle, &QPushButton::clicked, this, &fenetre::saveInfoBulle);
    connect(ui->bSaveGui, &QPushButton::clicked, this, &fenetre::saveGui);
}

fenetre::~fenetre()
{
    delete ui;
}

void fenetre::actuSec()
{
    //ui->posSouris->setText("x=" + QString::number(cursor().pos().x()) + ", y=" + QString::number(cursor().pos().y()));
    setWindowOpacity((qreal)ui->sliderOpacity->value()/10);

    RECT rectFen;
    GetWindowRect((HWND)FindWindow(NULL, (const wchar_t*) ui->lProg->text().utf16()), &rectFen);
    ui->case_spinW->setValue(rectFen.right-rectFen.left);
    ui->case_spinH->setValue(rectFen.bottom-rectFen.top);
    ui->case_spinXCase0->setValue(ui->case_spinW->value()/2 - 185);
    ui->case_spinYCase0->setValue(ui->case_spinH->value()/2 - 48);
}

void fenetre::actuCooMouse()
{
    ui->lCooMouse->setText("x=" + QString::number(cursor().pos().x()) + " ; y=" + QString::number(cursor().pos().y()));
}

void fenetre::saveCase()
{
    ui->timerCount->setMaximum(ui->timerSave->value()-1);
    for(int i=0; i<ui->timerSave->value(); i+=10)
    {
        Sleep(10);
        ui->timerCount->setValue(i);
    }
    ui->timerCount->setValue(0);

    actuSec();
    int posPlusCase = (ui->case_spinTailleCase->value() + ui->case_spinEspaceCase->value());
    int xCase=ui->case_spinXCase0->value() + posPlusCase*(ui->case_spinCaseS->value()%9);
    int yCase=ui->case_spinYCase0->value() + posPlusCase*(ui->case_spinCaseS->value()/9);
    if(ui->case_spinCaseS->value() >= 27)
        yCase += ui->case_spinEspaceCaseP->value() - ui->case_spinEspaceCase->value();

    QImage img = Screenshot::Item(xCase-1, yCase-1,
                            ui->case_spinTailleCase->value()+2, (ui->case_spinTailleCase->value()+2)/2,
                            (WId)FindWindow(NULL, (const wchar_t*) ui->lProg->text().utf16()));
    reconnaissance1(img);
}

void fenetre::reconnaissance1(QImage imgScreen)
{
    QImage imgBase("C:/Users/Jerome/Pictures/Qt/data/stone.png");
    if(imgScreen == imgBase)
    {
        qDebug("c'est de la stone");
    }
    else
        qDebug("pas de ressemblance");
}

void fenetre::saveInfoBulle()
{
    ui->timerCount->setMaximum(ui->timerSave->value()-1);
    for(int i=0; i<ui->timerSave->value(); i+=10)
    {
        Sleep(10);
        ui->timerCount->setValue(i);
    }
    ui->timerCount->setValue(0);
    Screenshot::InfoBulle((WId)FindWindow(NULL, (const wchar_t*) ui->lProg->text().utf16()));
}

void fenetre::saveGui()
{
   MC_Gui objectGui("C:/Users/Jerome/Pictures/Qt/data/defaut_1.14/assets/minecraft/textures/gui/container");
}
