#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenetre)
{
    ui->setupUi(this);
    connect(ui->bC, &QPushButton::clicked, this, &fenetre::chiffrer);
    connect(ui->bD, &QPushButton::clicked, this, &fenetre::dechiffrer);

    show();
}

fenetre::~fenetre()
{delete ui;}




void fenetre::chiffrer()
{
    ui->bC->setDisabled(true);
    debug("Chffirer : initialisation");
    QImage imgIn(0, 0, QImage::Format_ARGB32);
    QImage imgBack(0, 0, QImage::Format_ARGB32);
    if(!imgIn.load(ui->lCIn->text())) {
        debug("Chiffrer : imgIn n'a pas pu etre chargée");
        ui->bC->setEnabled(true);
        return;
    }
    if(!imgBack.load(ui->lCBack->text())) {
        debug("Chiffrer : imgBack n'a pas pu etre chargée");
        ui->bC->setEnabled(true);
        return;
    }


    double propWidth = static_cast<double>(imgIn.width()) / static_cast<double>(imgBack.width());// >=1 : faut agrandir
    double propHeight = static_cast<double>(imgIn.height()) / static_cast<double>(imgBack.height());
    if(propWidth < propHeight)
        propWidth = propHeight;//on prend la proportion la plus grande
    if(1.0 < propWidth) {//si c'est plus que 1, imgIn est plus grand
        imgBack = imgBack.scaled(imgBack.size() * propWidth);
    }//on met la taille de "back"


    QImage imgOut(imgBack.size(), QImage::Format_ARGB32);

    debug("Chffirer : Chiffrement en cours...");
    for(int iy=0; iy<imgOut.height(); iy++)
        for(int ix=0; ix<imgOut.width(); ix++) {
            if(imgIn.rect().contains(QPoint(ix, iy))) {
                imgOut.setPixelColor(ix, iy, maskColor(imgBack.pixelColor(ix, iy), imgIn.pixelColor(ix, iy)));
            }
            else {//si pas de msg caché, on met juste le msg
                imgOut.setPixelColor(ix, iy, maskColor(imgBack.pixelColor(ix, iy), QColor(0, 0, 0, 0)));
            }
        }
    saveImage(imgOut);
    debug("Chffirer : terminé");
    ui->bC->setEnabled(true);
}

void fenetre::dechiffrer()
{
    ui->bD->setDisabled(true);
    debug("Dechffirer : initialisation");
    QImage imgIn(0, 0, QImage::Format_ARGB32);
    if(!imgIn.load(ui->lDIn->text())) {
        debug("Dechiffrer : imgIn n'a pas pu etre chargée");
        ui->bC->setEnabled(true);
        return;
    }

    QImage imgOut(imgIn.size(), QImage::Format_ARGB32);
    debug("Dechffirer : déchiffrement en cours...");
    for(int iy=0; iy<imgOut.height(); iy++)
        for(int ix=0; ix<imgOut.width(); ix++) {
            imgOut.setPixelColor(ix, iy, demaskColor(imgIn.pixelColor(ix, iy)));
        }
    saveImage(imgOut);
    debug("Déchffirer : terminé");
    ui->bD->setEnabled(true);
}




void fenetre::saveImage(QImage out)
{
    if(!out.save(ui->lOut->text())) {
        debug("Impossible d'enregistrer la sortie");
    }
}




QColor fenetre::maskColor(QColor colorBack, QColor colorIn)
{
    colorBack = noMsgColor(colorBack);//on laisse du vide pour le message caché
    double mult = 255.0 / (static_cast<double>(ui->sPrecision->value()) -1.0);//diminuer le gamma

    //on ajoute le msg avec un gamma atténué (mélange de double et int pour les paliers)
    colorBack.setRed(colorBack.red() + qRound(static_cast<double>(colorIn.red()) / mult));
    colorBack.setGreen(colorBack.green() + qRound(static_cast<double>(colorIn.green()) / mult));
    colorBack.setBlue(colorBack.blue() + qRound(static_cast<double>(colorIn.blue()) / mult));
    //colorBack.setAlpha(colorBack.alpha() + qRound(static_cast<double>(colorIn.alpha()) / mult));

    return colorBack;
}
QColor fenetre::demaskColor(QColor colorIn)
{
    colorIn = msgColor(colorIn);//on prend le msg caché
    int mult = static_cast<int>(255.0 / (static_cast<double>(ui->sPrecision->value()) -1.0));//augmenter le gamma
    //ex : msg de 0, 1, 2, 3 => couleure de 0, 85, 170, 255
    colorIn.setRed(colorIn.red() * mult);
    colorIn.setGreen(colorIn.green() * mult);
    colorIn.setBlue(colorIn.blue() * mult);
    //colorIn.setAlpha(colorIn.alpha() * mult);
    return colorIn;
}


QColor fenetre::msgColor(QColor color)//retourne la couleure chiffrée sans la couleure autour
{
    color.setRed(color.red() % ui->sPrecision->value());
    color.setGreen(color.green() % ui->sPrecision->value());
    color.setBlue(color.blue() % ui->sPrecision->value());
    //color.setAlpha(color.alpha() % ui->sPrecision->value());
    return color;
}
QColor fenetre::noMsgColor(QColor color)//retourne uniquement la couleure sans le msg caché
{
    QColor color2 = msgColor(color);
    color.setRed(color.red() - color2.red());
    color.setGreen(color.green() - color2.green());
    color.setBlue(color.blue() - color2.blue());
    //color.setAlpha(color.alpha() - color2.alpha());
    return color;
}

void fenetre::debug(QString str)
{
    qDebug("%s", str.toStdString().c_str());
    ui->teLog->append(str.toStdString().c_str());
}
