#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenetre)
{
    ui->setupUi(this);
    fenPict = new viewPicture;
    connect(ui->inputLine, &QLineEdit::editingFinished, this, &fenetre::actuInput);
    connect(ui->inputOpen, &QPushButton::clicked, this, &fenetre::fenInput);
    connect(ui->inputShow, &QPushButton::clicked, this, &fenetre::fenShow);
    connect(ui->outputOpen, &QPushButton::clicked, this, &fenetre::fenOuput);
    connect(ui->bCouper, &QPushButton::clicked, this, &fenetre::startCut);
}

fenetre::~fenetre()
{
    delete ui;
}

void fenetre::actuInput()
{
    fenPict->setPicture(ui->inputLine->text());
    QPixmap pict(ui->inputLine->text());
    if(pict.isNull())
    {
        ui->inputEtat->setText("Image invalide");
        ui->inputHeight->setText(0);
        ui->inputWidth->setText(0);
        ui->inputShow->setDisabled(true);
        ui->bCouper->setDisabled(true);
    }
    else
    {
        ui->inputEtat->setText("Image chargée avec succés");
        ui->inputHeight->setText(QString::number(pict.height()));
        ui->inputWidth->setText(QString::number(pict.width()));
        ui->inputShow->setEnabled(true);
        ui->bCouper->setEnabled(true);
    }
}

void fenetre::fenInput()
{
    QString file = QFileDialog::getOpenFileName(this, "Ouvrir une image", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    ui->inputLine->setText(file);
    actuInput();
}

void fenetre::fenShow()
{
    fenPict->show();
    fenPict->setFocus();
}

void fenetre::fenOuput()
{
    QString file = QFileDialog::getExistingDirectory(this, "Ouvrir un dossier", QString(), QFileDialog::ShowDirsOnly);
    ui->outputLine->setText(file);
}

void fenetre::startCut()
{
    ui->bCouper->setDisabled(true);

    QPixmap pix(ui->inputLine->text());
    QPixmap pix2;
    QString fileOut = ui->outputLine->text() + ui->inputLine->text().mid(ui->inputLine->text().lastIndexOf("/"));

    fileOut = fileOut.mid(0, fileOut.lastIndexOf("."));

    float height = ui->inputHeight->text().toInt() / ui->ouputNbHeight->value();
    float width = ui->inputWidth->text().toInt() / ui->ouputNbWidth->value();
    for(int vertical=0; vertical<ui->ouputNbHeight->value(); vertical++)
    {
        for(int horizontal=0; horizontal<ui->ouputNbWidth->value(); horizontal++)
        {
            pix2 = pix.copy((int)(horizontal*width), (int)(vertical*height), (int)width, (int)height);
            if(pix2.save(fileOut + "_" + QString::number(horizontal) + ";" + QString::number(vertical) + ".png"))
                qDebug("save ok");

        }
    }

    ui->bCouper->setEnabled(true);
}
