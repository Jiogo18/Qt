#include "viewPicture.h"
#include "ui_viewPicture.h"

viewPicture::viewPicture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewPicture)
{
    ui->setupUi(this);
}

viewPicture::~viewPicture()
{
    delete ui;
}

void viewPicture::setPicture(QString file)
{
    ui->Image->setPixmap(QPixmap(file));
}
