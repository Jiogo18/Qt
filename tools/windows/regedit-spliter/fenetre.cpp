#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenetre)
{
    ui->setupUi(this);
    connect(ui->bSplitOpen, &QPushButton::clicked, this, &fenetre::onBSplitOpen);
    connect(ui->bSplit, &QPushButton::clicked, this, &fenetre::onBSplit);
    connect(ui->bSplitAndNext, &QPushButton::clicked, this, &fenetre::onBSplitAndNext);
    reg.setProgressBar(ui->pbChargement);
}

fenetre::~fenetre()
{
    delete ui;
}




void fenetre::onBSplitOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Reg File"), "", tr("Registre Files (*.reg *.reg.edit);;All Files (*)"));
    if(fileName != "")
        ui->splitPath->setText(fileName);
}
void fenetre::onBSplit()
{
    QStringList whitelist = ui->splitWhitelist->toPlainText().split("\n");
    QStringList blacklist = ui->splitBlacklist->toPlainText().split("\n");
    reg.setWhitelist(whitelist);
    reg.setBlacklist(blacklist);
    reg.split(ui->splitPath->text());
    ui->clefsTotal->setText(QString::number(reg.getClefsOriginal()));
    ui->clefsWhitelist->setText(QString::number(reg.getClefsWhitelist()));
    ui->clefsSemiWhitelist->setText(QString::number(reg.getClefsSemiWhitelist()));
    ui->clefsBlacklist->setText(QString::number(reg.getClefsBlacklist()));
    ui->whitelistValueNotable->setPlainText(reg.getWhitelistValueNotable().join("\n"));
}
void fenetre::onBSplitAndNext()
{
    onBSplit();
    if(reg.getLastBlacklistPath() != "") {
        ui->splitPath->setText(reg.getLastBlacklistPath());
    }
}
