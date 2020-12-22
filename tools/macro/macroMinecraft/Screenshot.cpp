#include "Screenshot.h"

Screenshot::Screenshot()
{}

void Screenshot::debug(QString msg)
{
    qDebug("%s", QString("["+QTime::currentTime().toString("hh:mm:ss.zzz")+"] "+msg).toStdString().c_str());
}

QImage Screenshot::shootScreen(int x, int y, int W, int H, int fenetre)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    return screen->grabWindow(fenetre, x, y, W, H).toImage();
}

QImage Screenshot::Item(int x, int y, int W, int H, int fenetre)
{
    QImage imgScreen=shootScreen(x, y, W, H, fenetre);
    save(imgScreen, QString::number(QDateTime::currentMSecsSinceEpoch()),
         "C:/Users/Jerome/Pictures/Qt/item");

    return imgScreen;
}

QImage Screenshot::InfoBulle(int fenetre)
{
    Q_UNUSED(fenetre)
    //QScreen *screen = QGuiApplication::primaryScreen();
    //QImage imgScreen= screen->grabWindow(fenetre).toImage();
    QImage imgScreen = QImage("C:/Users/Jerome/Pictures/Qt/data/screenshot.png");
    save(imgScreen, QString::number(QDateTime::currentMSecsSinceEpoch()), "C:/Users/Jerome/Pictures/Qt/screenshot");

    imgScreen = recadrerInfoBulle(imgScreen);
    imgScreen = colorerInfoBulle(imgScreen);

    QPoint posMinecraft;
    reconnaissance(imgScreen, QImage("C:/Users/Jerome/Pictures/Qt/data/minecraft.png"),
                   6, 6, 0, imgScreen.height()-1, "minecraft:", &posMinecraft);
    QImage apresMinecraft=imgScreen.copy(posMinecraft.x(), posMinecraft.y(), imgScreen.width()-(posMinecraft.x()+1), 16);

    QPoint posVide;
    if(reconnaissance(apresMinecraft, QImage("C:/Users/Jerome/Pictures/Qt/data/vide.png"),
                       0, apresMinecraft.width()-1, 0, 0, "vide", &posVide))
    {
        apresMinecraft=apresMinecraft.copy(0, 0, posVide.x(), 16);
    }

    save(apresMinecraft, QString::number(QDateTime::currentMSecsSinceEpoch()), "C:/Users/Jerome/Pictures/Qt/detail");

    save(imgScreen, QString::number(QDateTime::currentMSecsSinceEpoch()), "C:/Users/Jerome/Pictures/Qt/colore");

    return imgScreen;
}


QImage Screenshot::recadrerInfoBulle(QImage img)
{
    QColor couleur;
    int xMin=img.width(), xMax=0, yMin=img.height(), yMax=0;
    for(int y=0; y<img.height(); y++)
    {
        for(int x=0; x<img.width(); x++)
        {
            couleur = img.pixelColor(x,y);
            if( 23 <= couleur.red() && couleur.red() <= 46 &&
                0 <= couleur.green() && couleur.green() <= 10 &&
                50 <= couleur.blue() && couleur.blue() <= 101)//violet
            {
                img.setPixelColor(x, y, QColor(0, 255, 0));//set en vert
                if(x<xMin) xMin=x;
                if(x>xMax) xMax=x;
                if(y<yMin) yMin=y;
                if(y>yMax) yMax=y;
            }
            if( 15 <= couleur.red() && couleur.red() <= 30 &&
                0 <= couleur.green() && couleur.green() <= 15 &&
                15 <= couleur.blue() && couleur.blue() <= 30)//bleu
            {
                img.setPixelColor(x, y, QColor("black"));
            }
        }
    }
    img=img.copy(xMin, yMin, xMax-xMin+1, yMax-yMin+1);
    QMap<int, int> pixelsX;//inversé (nb de pixels en y selon la pos x)
    QMap<int, int> pixelsY;
    for(int y=0; y<img.height(); y++)
    {
        for(int x=0; x<img.width(); x++)
        {
            couleur = img.pixelColor(x,y);
            if(couleur.red()==0 && couleur.green()==255 && couleur.blue()==0)//vert
            {
                pixelsX.insert(x, pixelsX.value(x)+1);
                pixelsY.insert(y, pixelsY.value(y)+1);
                img.setPixelColor(x, y, QColor("black"));
            }
        }
    }

    int LongueurXMax=0, LongueurYMax=0;//longueur de xMax=maximum avec y égal selon le x
    for(int ix=0; ix<img.width(); ix++)
        if(pixelsX.value(ix)>LongueurXMax)
            LongueurXMax=pixelsX.value(ix);
    for(int iy=0; iy<=img.height(); iy++)
        if(pixelsY.value(iy)>LongueurYMax)
            LongueurYMax=pixelsY.value(iy);

    xMin=img.width()-1;//devient un new point (plus précis)
    yMin=img.height()-1;
    xMax=0;
    yMax=0;
    for(int x=0; x<img.width(); x++)
        if(pixelsX.value(x) >= (float)LongueurXMax*0.7)//4 min (2 de chaques coté)
        {//pas plus de 0.7 car si il y a un item violet ds le chamin, le LongueurXMax sera trop grand
            if(xMin>x)
                xMin=x;
            if(xMax<x)
                xMax=x;
        }
    for(int y=0; y<img.height(); y++)
        if(pixelsY.value(y) >= (float)LongueurYMax*0.7)//4 min (2 de chaques coté)
        {
            if(yMin>y)
                yMin=y;
            if(yMax<y)
                yMax=y;
        }

    return img.copy(xMin, yMin, xMax-xMin+1, yMax-yMin+1);
}

QImage Screenshot::colorerInfoBulle(QImage img)
{
    QList<int> lignes;//pos du pixel jusqu'au premier pixel non noir
    for(int y=0; y<img.height(); y++)
    {
        for(int x=0; x<img.width(); x++)
        {
            if(lignes.size()>y)
            {
                lignes.replace(y, x);
            }
            else
                lignes.insert(y, x);

            if(img.pixelColor(x, y) != QColor("black"))
                break;
        }
    }

    QColor couleur;//remettre en ligne par ligne le txt
    int firstXPixel;//hauteur ==18 tjs (16+ombre)
    for(int y=0; y+17<lignes.size(); y++)//17=taille du texte (18)
    {
        if(lignes[y] == img.width()-1)//si c pas dans une ligne de txt
            continue;

        firstXPixel=lignes[y];
        couleur=img.pixelColor(lignes[y], y);
        for(int i=0; i<18; i++)
        {
            if(firstXPixel > lignes[y+i])
            {
                firstXPixel = lignes[y+i];
                couleur=img.pixelColor(lignes[y+i], y+i);
            }
        }
        for(int x=firstXPixel; x<img.width(); x++)
        {
            for(int y2=0; y2<18; y2++)
            {
                if(img.pixelColor(x, y2+y) == QColor("black"))
                    continue;
                if(img.pixelColor(x, y2+y) != couleur)
                {
                    img.setPixelColor(x, y2+y, QColor("black"));
                }
                else
                    img.setPixelColor(x, y2+y, QColor("white"));
            }
        }
        y+=17;//18 fait a la boucle
    }
    return img;
}

bool Screenshot::reconnaissance(QImage imgBase, QImage imgCherche, int xMin, int xMax, int yMin, int yMax, QString erreur, QPoint *pos)
{
    if(imgBase.width()<xMax) xMax=imgBase.width()-1;
    if(imgBase.height()<yMax) yMax=imgBase.height()-1;
    if(xMin>xMax) xMin=xMax;
    if(yMin>yMax) yMin=yMax;
    for(int iy=yMin; iy<=yMax; iy++)
    {
        for(int ix=xMin; ix<=xMax; ix++)
        {
            if(imgBase.copy(ix, iy, imgCherche.width(), imgCherche.height())==imgCherche)
            {
                pos->setX(ix);
                pos->setY(iy);
                return true;
            }
        }
    }
    debug(erreur + " n'a pas été trouvé");
    return false;
}

void Screenshot::save(QImage img, QString name, QString folder)
{
    if(!img.save(folder+"/"+name+".png"))
        debug(name+" n'a pas été enregistré dans "+folder);
}
