#include "fenetre.h"
#include "ui_fenetre.h"

/*correction imprimante:
6:170 =>9:186 (L=4 H=17)
6:810 => 9:826 (L=4 H=17)
27:3370 => 31:3386 (L=5 H=17)
+(640 ; 64)
patern x: +640
patern y: +21.3
ou 4 5 5 (L=47) * 40 (+64)
total de blocks : 6*40
a partir de 1798 il monte à y=169 H=18
a partir de 1968 y=169 H=17*/

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    connect(ui->bFileIn, &QPushButton::clicked, this, &fenetre::bParcourirPress);
    connect(ui->bCorriger, &QPushButton::clicked, this, &fenetre::bCorrigerPress);
    connect(ui->bShow, &QPushButton::clicked, this, &fenetre::showImg);
    lblImg = new QLabel("image show");
    show();
}

fenetre::~fenetre() { lblImg->close(); delete lblImg; delete ui; }
void fenetre::closeEvent(QCloseEvent *event)
{
    lblImg->close();
    event->accept();
}


void fenetre::bParcourirPress()
{
    QString fileName=QFileDialog::getOpenFileName(this, "Ouvrir une image",
                          ui->lFileIn->text(), "Images en 300ppp (*.png)");
    if(fileName != "")
        ui->lFileIn->setText(fileName);
    loadImg();//dira si c'est chargé
}
void fenetre::dragEnterEvent(QDragEnterEvent *event)
{//hover with a picture (or something else
    if (event->mimeData()->hasFormat("FileName"))
          event->acceptProposedAction();
}
void fenetre::dropEvent(QDropEvent *event)
{//drop a picture
    if(event->mimeData()->hasFormat("FileName")) {
        ui->lFileIn->setText(event->mimeData()->text());
        event->acceptProposedAction();
    }
}
void fenetre::loadImg()
{
    QString file=ui->lFileIn->text();
    QUrl url(file);

    if(url.isValid() && imgIn.load(url.toString())) {
        //(show image)
        QImage imgLbl = imgIn;
        if(imgLbl.width()>500)
            imgLbl=imgLbl.scaledToWidth(500);
        if(imgLbl.height()>500)
            imgLbl=imgLbl.scaledToHeight(500);
        lblImg->setPixmap(QPixmap::fromImage(imgLbl));
        lblImg->setFixedSize(imgLbl.size());

        imgIn = imgIn.convertToFormat(QImage::Format_RGB32);
    }
    else {
        imgIn = QImage();//clear
        if(!url.isValid())
            lblImg->setText(file+" n'est pas un fichier valide.");
        else
            lblImg->setText(file+" est introuvable.");
    }
    QCoreApplication::processEvents();
}
void fenetre::showImg()
{
    ui->bShow->setDisabled(true);
    setCursor(Qt::WaitCursor);
    loadImg();
    if(ui->lFileIn->text() != "")//si pas d'image ça sert à rien
        lblImg->show();
    setCursor(Qt::ArrowCursor);
    ui->bShow->setEnabled(true);
}


void fenetre::bCorrigerPress()
{
    ui->bCorriger->setDisabled(true);
    setCursor(Qt::WaitCursor);
    loadImg();
    if(!imgIn.isNull())
        corrigeImg();

    if(!imgOut.isNull()) {
        QString file=ui->lFileIn->text();
        if(!ui->checkReplaceImage->isChecked()) {
            QString extension = file.mid(file.lastIndexOf("."));//".png"
            file = file.left(file.lastIndexOf("."));
            file = getUniqueFile(file, extension);
        }

        if(imgOut.save(QUrl(file).toString())) {//garde le dossier et le format
            ui->lFileOut->setText(file);
        }
        else
            ui->lFileOut->setText(file + " n'a pas pu être enregistré");
        ui->pbPixels->setValue(ui->pbPixels->maximum());
    }
    else {
        ui->lFileOut->setText("L'image n'a pas été traitée.");
        ui->pbPixels->setValue(0);
    }
    setCursor(Qt::ArrowCursor);
    ui->bCorriger->setEnabled(true);
}
QString fenetre::getUniqueFile(QString file, QString extension)
{
    quint64 i=1;
    QUrl url;
    do {
        i++;
        url.setUrl(file+" ("+QString::number(i)+")"+extension);
    } while(QFile::exists(url.toLocalFile()));

    return url.url();
}

void fenetre::corrigeImg()
{
    /*debug:
    listePixelRed.clear();
    listePixelGreen.clear();
    listePixelBlue.clear();*/
    imgOut = detectCorruption(imgIn);
}

QImage fenetre::detectCorruption(QImage img)
{
    ui->pbPixels->setMaximum(6*120*2);
    for(int iy=0; iy<6; iy++) {//le block par ligne
        for(int ix=0; ix<120; ix++) {
            ui->pbPixels->setValue(iy*120+ix);
            qreal ym = 170 + 640*iy, yM = 186 + 640*iy;
            if(84 <= ix) ym --;//après 1769 il est à 169 de base
            if(92 <= ix) yM--;//après 1968 il passe à 185 de base
            qreal xm = 6.0 + (64.0/3.0)*(qreal)ix, xM = 10.0 + (64.0/3.0)*(qreal)ix;
            if(ix % 3 == 0) xM--;

            for(qreal y=ym; y<=yM && y<img.height(); y++) {
                for(qreal x=xm; x<=xM && x<img.width(); x++) {
                    int x2=qFloor(x), y2=qFloor(y);
                    QColor moyenne = moyenneColor(img.pixel(x2-1,y2), img.pixel(x2,y2-1));
                    img.setPixel(x2, y2, corrigePixel(img.pixel(x2, y2), (int)(5*(ix % 3)+(x-xm)), moyenne).rgb());
                    //if(ym < y && y+1 <= yM && xm < x && x+1 <= xM)//rempli l'intérieur des erreurs (pour check la formule)
                    //    img.setPixel((int)floor(x), (int)floor(y), 0);

                }
            }

        }
    }
    return img;
}

QColor fenetre::corrigePixel(QColor color, int ix, QColor colorMoyenne)
{
    //utilise les couleur des cases à coté sauf pour:
    //1 : B  R  B R  0
    //2 : R  RV 0 RV VB
    //3 : RV VB 0 VB B
    if(ix==6||ix==8||ix==10) {//rouge et vert
        color.setBlue(colorMoyenne.blue());
    } else if(ix==9||ix==11||ix==13) {
        color.setRed(colorMoyenne.red());
    } else if(ix==1||ix==3||ix==5||ix==6||ix==8||ix==10) {//rouge
        color.setGreen(colorMoyenne.green());
        color.setBlue(colorMoyenne.blue());
    } else if(ix==6||ix==8||ix==9||ix==10||ix==11||ix==13) {//vert
        color.setRed(colorMoyenne.red());
        color.setBlue(colorMoyenne.blue());
    } else if(ix==0||ix==2||ix==9||ix==11||ix==13||ix==14) {//bleu
        color.setRed(colorMoyenne.red());
        color.setGreen(colorMoyenne.green());
    } else {
        color = lastColor;
    }
    lastColor = color;
    return color;
}

