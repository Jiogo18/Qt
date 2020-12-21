#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    toastI = new toastIcon(this);

    //on charge les configs
    connect(&EDV, &ecranDeVeille::onDebug, this, &fenetre::debug);
    EDVConfig config = EDV.getConfig();
    ui->ressourcePath->setText(config.getRessourcePath());
    ui->timeout->setValue(config.getTimeout());
    ui->timeActu->setValue(config.getTimeRefresh());

    ui->screenshot->setChecked(config.isScreenshot());
    ui->screenshotMargin->setValue(config.getScreenshotMargin());
    ui->screenshotTime->setValue(config.getScreenshotTime());

    ui->mouse->setChecked(config.isMouse());
    ui->mouseMargin->setValue(config.getMouseMargin());
    ui->mouseTime->setValue(config.getMouseTime());

    ui->mouseV->setChecked(config.isMouseV());
    ui->mouseVMargin->setValue(config.getMouseVMargin());
    ui->mouseVRepeat->setChecked(config.isMouseVRepeat());

    ui->key->setChecked(config.isKeyboard());
    ui->keyRepeat->setChecked(config.isKeyboardRepeat());

    connect(ui->bActu, &QPushButton::clicked, this, &fenetre::updateConfig);
    timerActu.start(1000);//se recale plus tard
    connect(&timerActu, &QTimer::timeout, this, &fenetre::updateConfig);
    //timerActu actualise aussi les paramètres (ainsi que le bouton bActu)

    connect(ui->bLaunch, &QPushButton::clicked, &EDV, &ecranDeVeille::show);
}

fenetre::~fenetre() { delete ui; delete toastI; }

void fenetre::closeEvent(QCloseEvent *event)
{
    EDV.hide();//normalement n'arrive pas mais why not...
    updateConfig();
    event->accept();
}

void fenetre::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange && isMinimized()) {
        hide();
    }
}

void fenetre::show()
{
    timerActu.start(EDV.getConfig().getTimeRefresh());
    QWidget::show();
}

void fenetre::hide()
{
    timerActu.stop();
    updateConfig();
    QWidget::hide();
}



void fenetre::updateConfig()
{
    EDVConfig config = EDV.getConfig();//si on oublie un truc il reste dans la boucle
    config.setRessourcePath(ui->ressourcePath->text());
    config.setTimeout(ui->timeout->value());
    config.setTimeRefresh(ui->timeActu->value());

    config.setScreenshot(ui->screenshot->isChecked());
    config.setScreenshotMargin(ui->screenshotMargin->value());
    config.setScreenshotTime(ui->screenshotTime->value());

    config.setMouse(ui->mouse->isChecked());
    config.setMouseMargin(ui->mouseMargin->value());
    config.setMouseTime(ui->mouseTime->value());

    config.setMouseV(ui->mouseV->isChecked());
    config.setMouseVMargin(ui->mouseVMargin->value());
    config.setMouseVRepeat(ui->mouseVRepeat->isChecked());

    config.setKeyboard(ui->key->isChecked());
    config.setKeyboardRepeat(ui->keyRepeat->isChecked());

    if(!(lastConfig == config)) {
        EDV.setConfig(config);
        lastConfig = config;
    }
    if(timerActu.interval() != EDV.getConfig().getTimeRefresh())
        timerActu.start(EDV.getConfig().getTimeRefresh());
    ui->lcdNBTime->display(qCeil(EDV.getTimeRemain() / 1000.0));
    updateNbVideo();
}


void fenetre::updateNbVideo()
{
    int nbVideo = EDV.getNbVideo();
    QString style;
    if(nbVideo == 0)
    {
        style="QLabel { color : red; }";
        ui->bLaunch->setDisabled(true);
    }
    else
    {
        style="QLabel { color : black; }";
        ui->bLaunch->setEnabled(true);
    }
    ui->lblVideoNb->setText(QString::number(nbVideo));
    ui->lblVideoNb->setStyleSheet(style);
}

void fenetre::debug(QString str) { qDebug() << str; }
