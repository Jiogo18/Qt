#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenetre)
{
    ui->setupUi(this);

    deb = new debug(ui->teLog);


    mc = new minecraft(ui->lMCApp->text(), ui->lMCFolder->text(), deb);
    mc->setActuInterval(ui->sTimeActu->value());
    connect(mc, &minecraft::newIcon, this, &fenetre::setIcon);
    connect(ui->lMCApp, &QLineEdit::textEdited, mc, &minecraft::setMCApp);
    connect(ui->sTimeActu, QOverload<int>::of(&QSpinBox::valueChanged), mc, &minecraft::setActuInterval);
    connect(ui->bStart, &QPushButton::clicked, this, &fenetre::bStartClicked);
    connect(ui->bStop, &QPushButton::clicked, this, &fenetre::bStopClicked);
    connect(ui->lMCFolder, &QLineEdit::textEdited, mc, &minecraft::setMCFolder);
    connect(&tStopMC, &QTimer::timeout, this, &fenetre::bStopClicked);
    connect(ui->bStartOne, &QPushButton::clicked, mc, &minecraft::startOne);
    connect(ui->bCraftDoor, &QPushButton::clicked, mc, &minecraft::craftDoor);
    tStopMC.setSingleShot(true);
    ui->bStop->setDisabled(true);

    tActuMouse.start(200);
    connect(&tActuMouse, &QTimer::timeout, this, &fenetre::actuCooMouse);

    show();
}

fenetre::~fenetre()
{
    delete ui;
    delete deb;
    delete mc;
}




void fenetre::setOpacity(int v) {setWindowOpacity(static_cast<double>(v) * 0.01);}
void fenetre::setIcon(QString ico) {setWindowIcon(QIcon(":/icons/"+ico+".ico"));}

void fenetre::bStartClicked()
{
    ui->bStart->setDisabled(true);
    ui->bStop->setEnabled(true);
    mc->start();
    tStopMC.start(ui->sTimeMax->value());
}
void fenetre::bStopClicked()
{
    mc->stop();
    tStopMC.stop();//si on press sur le bouton, pas besoin de relancer un stop
    ui->bStart->setEnabled(true);
    ui->bStop->setDisabled(true);
}

void fenetre::actuCooMouse()
{
    ui->lblMouse->setText("x=" + QString::number(cursor().pos().x()) + " ; y=" + QString::number(cursor().pos().y()));
}

