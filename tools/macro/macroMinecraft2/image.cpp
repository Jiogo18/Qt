#include "image.h"

image::image()
{

}


QImage *image::shootScreen(QString fenetre)
{
    WId fen = (WId)findApp(fenetre);
    QScreen *screen = QGuiApplication::primaryScreen();

    if(fen==0)
    {//l'application cible n'a pas été trouvée
        debug("Le handle de \""+fenetre+"\" n'a pas été trouvé");
        return nullptr;
    }

    //faire un screenshot de la fenetre uniquement (ou tout si fen==0)
    QImage *retour = new QImage(screen->grabWindow(fen).toImage());

    if(retour->size() == QSize(1,1))
    {//si la fenetre tourne en arrière plan
        //(si une autre fenetre est dessus ça marche mais il faut que mc soit quand meme affiché "devant le bureau", que la fenetre n'est pas réduit)
        debug("Le handle de \""+fenetre+"\" est "+QString::number(fen)+" mais la fenetre ne fait pas de rendue actuellement");
        return nullptr;
    }
    debug("Le handle de \""+fenetre+"\" est "+QString::number(fen));
    return convertToGoodFormat(retour);
}

QImage *image::safeScreen(QString fenetre)
{
    if(findInfoApp(fenetre)->dwWindowStatus != 1)
    {
        debug("La fenetre \""+fenetre+"\" n'est pas au premier plan");
        return nullptr;
    }
    SetCursorPos(getLeftTopFen(fenetre).x(), getLeftTopFen(fenetre).y());//déplace au pixel en haut à droite pour pas d'interférence
    return shootScreen(fenetre);
}


/*
QImage image::Item(int x, int y, int W, int H, int fenetre)
{
    QImage imgScreen=shootScreen(x, y, W, H, fenetre);
    save(imgScreen, QString::number(QDateTime::currentMSecsSinceEpoch()),
         "C:/Users/Jerome/Pictures/Qt/item");

    return imgScreen;
}

QImage image::InfoBulle(int fenetre)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    //QImage imgScreen= screen->grabWindow(fenetre).toImage();//désactivé pour plus rapide?
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


QImage image::recadrerInfoBulle(QImage img)
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
        if(pixelsX.value(x) >= LongueurXMax*0.7)//4 min (2 de chaques coté)
        {//pas plus de 0.7 car si il y a un item violet ds le chamin, le LongueurXMax sera trop grand
            if(xMin>x)
                xMin=x;
            if(xMax<x)
                xMax=x;
        }
    for(int y=0; y<img.height(); y++)
        if(pixelsY.value(y) >= LongueurYMax*0.7)//4 min (2 de chaques coté)
        {
            if(yMin>y)
                yMin=y;
            if(yMax<y)
                yMax=y;
        }

    return img.copy(xMin, yMin, xMax-xMin+1, yMax-yMin+1);
}

QImage image::colorerInfoBulle(QImage img)
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

bool image::reconnaissance(QImage imgBase, QImage imgCherche, int xMin, int xMax, int yMin, int yMax, QString erreur, QPoint *pos)
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
}*/

void image::save(const QImage *img, QString name, QString folder)
{
    if(img == nullptr || img->isNull())
        return;
    QImage *img2 = convertToGoodFormat(img);
    if(!img2->save(folder+"/"+name+".png"))
        debug(name+" n'a pas été enregistré dans "+folder);
}

QImage *image::load(QString folder, QString name)
{
    if(folder == "")
        return nullptr;
    QImage *retour = new QImage;
    if(!retour->load(folder+"/"+name+".png")) {
        debug(name+" n'a pas pu etre chargée depuis "+folder);
    }
    retour = convertToGoodFormat(retour);
    return retour;
}

QImage *image::convertToGoodFormat(const QImage *img)
{
    if(img == nullptr || img->isNull())
        return nullptr;
    QImage *imgRetour = new QImage(*img);
    if(imgRetour->format() != QImage::Format_ARGB32)
        *imgRetour = imgRetour->convertToFormat(QImage::Format_ARGB32);
    return imgRetour;
}





int image::pixelsSim(const QImage *img1, const QImage *img2, int marge)
{//marge : marge d'erreur (0% pour un score de 0, 50% pour un score <= 384, 100% pour un score <= 768
    if(img1 == nullptr || img2 == nullptr || img1->isNull() || img2->isNull())
        return 0;
    int pixsOk=0;
    QImage imgSimTest = QImage(getMinSize(img1->size(), img2->size()), img1->format());
    for(int iy=0; iy<img1->height() && iy<img2->height(); iy++) {
        for(int ix=0; ix<img1->width() && ix<img2->width(); ix++) {
            QColor pix1 = img1->pixelColor(ix, iy);
            QColor pix2 = img2->pixelColor(ix, iy);
            int scorePix = abs(pix1.red()-pix2.red()) + abs(pix1.green()-pix2.green()) + abs(pix1.blue()-pix2.blue());
            //score d'écart entre chaques couleur
            if(scorePix <= marge*7.68) {//est dans la marge (7.68 car : 100% -> "score < 768")
                pixsOk++;
            }
            else if(pix2.alpha() == 0) {//si le 2 est transparent
                pixsOk++;
            }
            else
                imgSimTest.setPixelColor(ix, iy, QColor(0, 255, 0));

        }
    }
    /*if(img2->size() == QSize(352, 332) && pixsOk*100/getNbPixsMin(img1, img2) >= 97) {
        qDebug()<<"pixelSim"<<pixsOk<<getNbPixsMin(img1, img2);
        imgSimTest.save("C:\\Users\\Jerome\\Pictures\\Qt\\MC2\\test\\pixelsSim-crafting_table-imgSimTest.png");
    }*/
    return pixsOk;
}

double image::ressemblance(const QImage *img1, const QImage *img2)
{//retourne le pourcentage de ressemblance entre 2 imgs
    if(img1 == nullptr || img2 == nullptr || img1->isNull() || img2->isNull())
        return 0;
    return static_cast<double>(pixelsSim(img1, img2, 0)) * 100.0 / static_cast<double>(getNbPixsMin(img1, img2));//en pourcentage
}

double image::ressemblanceContainsMid(const QImage *img1, const QImage *img2)
{//on recadre en plus les images pour qu'elles aient la meme taille
    if(img1 == nullptr || img2 == nullptr || img1->isNull() || img2->isNull())
        return 0;
    QImage *img2_2 = new QImage((*img2).copy(QRect(QPoint(0, 0), getMinSize(img1->size(), img2->size()))));//sécurité
    QImage *img1_2 = cutContainsMid(img1, img2_2);

    //img2_2->save("C:\\Users\\Jerome\\Pictures\\Qt\\MC2\\test\\ressemblanceContainsMid"+QTime::currentTime().toString("HHmmss.zzz")+"-1.png");
    //img1_2->save("C:\\Users\\Jerome\\Pictures\\Qt\\MC2\\test\\ressemblanceContainsMid"+QTime::currentTime().toString("HHmmss.zzz")+"-2.png");

    return ressemblance(img1_2, img2_2);
}

QImage *image::cutContainsMid(const QImage *img1, const QImage *img2)
{
    return new QImage((*img1).copy(QRect(
                                       getMidPoint(img1, img2),
                                       img2->size())));//la meme taille que img2_2
}

/*bool image::containsMid(const QImage *img1, const QImage *img2)
{//on test si le gui est valide
    if(img1 == nullptr || img2 == nullptr || img1->isNull() || img2->isNull())
        return false;
    return ressemblanceContainsMid(img1, img2) >= 90;//90 de marge?
}*/

QPoint image::getMidPoint(const QImage *img1, const QImage *img2)
{//retourne le point d'intersection (le plus en haut à gauche) des 2 imgs en mettant la 2 au milieu de la 1
    QPointF pt;//le milieu moins le milieu (le début de img2_2)
    pt.setX(static_cast<double>(img1->width())/2.0 - static_cast<double>(img2->width())/2.0);
    pt.setY(static_cast<double>(img1->height())/2.0 - static_cast<double>(img2->height())/2.0);
    pt.setX(qCeil(pt.x()/2-0.5)*2);//arrondit au pair le plus proche (mais si impair, donne pair inférieur)
    pt.setY(qCeil(pt.y()/2-0.5)*2);//qCeil(3/2-0.5)*2 = qCeil(1.5-0.5)*2 = qCeil(1)*2 = 2
    return pt.toPoint();
}





QSize image::getMinSize(QSize size1, QSize size2)
{//retourne la taille la plus petite pouvant contenir les 2
    if(size2.width() < size1.width())
        size1.setWidth(size2.width());
    if(size2.height() < size1.height())
        size1.setHeight(size2.height());
    return size1;
}

int image::getNbPixsMin(const QImage *img1, const QImage *img2)
{//retourne le nb de pixels selont la taille minimum pouvant contenir les 2
    if(img1 == nullptr || img2 == nullptr || img1->isNull() || img2->isNull())
        return -1;//avec 0, ça risque de bugger (division)
    QSize sizeMin = getMinSize(img1->size(), img2->size());
    return sizeMin.width()*sizeMin.height();
}

QImage *image::relativeScaled(const QImage *img, double size)
{
    if(img==nullptr || img->isNull())
        return nullptr;
    //obligé de passer par (*img).scaled parce que la fonction n'aime pas les pointeurs
    /*return new QImage((*img).scaled(
                          static_cast<int>(static_cast<double>(img->width()) * size),
                          static_cast<int>(static_cast<double>(img->height()) * size)));*/
    return new QImage(img->scaled(img->size() * size));
}

QString image::findBestImageContains(const QImage *img, const QMap<QString, QImage *> *listeImgs, double min)
{//on change juste l'etat de canst des images
    QMap<QString, const QImage *> *liste2 = new QMap<QString, const QImage *>;
    QMapIterator<QString, QImage *> i(*listeImgs);
    while(i.hasNext()) {
        i.next();
        liste2->insert(i.key(), i.value());
    }
    return findBestImageContains(img, liste2, min);
}
QString image::findBestImageContains(const QImage *img, const QMap<QString, const QImage *> *listeImgs, double min)
{
    if(img == nullptr || img->isNull())
        return "";
    QString bestName="";
    double bestScore=min;
    QMapIterator<QString, const QImage*> i(*listeImgs);
    while(i.hasNext()) {
        i.next();
        QString name = i.key();
        double score = image::ressemblanceContainsMid(img, i.value());

        /*if(listeImgs->contains("inventory") || listeImgs->contains("2_inventory")) {
            debug("\""+name+"\" a un score de "+QString::number(score));
        }*/

        if(bestScore < score) {
            //si c'est le meilleur
            bestName = name;
            bestScore = score;
        }
        //deb->addLog("minecraft/gui/image", name+" a un score de "+QString::number(score));
    }
    /*if(listeImgs->contains("inventory") || listeImgs->contains("tab_inventory")
             || listeImgs->contains("crafting_table") || listeImgs->contains("chest")
             || listeImgs->contains("chest_big"))*/
        //debug("\""+bestName+"\" a un score (final) de "+QString::number(bestScore));
    return bestName;
}

QString image::findBestImage(const QImage *img, const QMap<QString, const QImage *> *listeImgs, double min)
{
    if(img == nullptr || img->isNull())
        return "";
    QString bestName="";
    double bestScore=min;
    QMapIterator<QString, const QImage*> i(*listeImgs);
    while(i.hasNext()) {
        i.next();
        QString name = i.key();
        if(img->size() != i.value()->size())//taille diff
            continue;
        double score = image::ressemblance(img, i.value());

        /*if(listeImgs->contains("inventory") || listeImgs->contains("tab_inventory")
                || listeImgs->contains("crafting_table") || listeImgs->contains("chest")
                || listeImgs->contains("chest_big")) {
            debug("\""+name+"\" a un score de "+QString::number(score));
            //i.value()->save("C:\\Users\\Jerome\\Pictures\\Qt\\MC2\\test\\findBestImage"+QTime::currentTime().toString("HHmmss.zzz")+"-1.png");
        }*/
        //i.value()->save("C:\\Users\\Jerome\\Pictures\\Qt\\MC2\\test\\findBestImage-2-"+QTime::currentTime().toString("HHmmss.zzz")+".png");

        if(bestScore < score) {//si c'est le meilleur
            bestName = name;
            bestScore = score;
        }
    }
    if(listeImgs->contains("inventory") || listeImgs->contains("tab_inventory")
            || listeImgs->contains("crafting_table") || listeImgs->contains("chest")
            || listeImgs->contains("chest_big"))
        debug("\""+bestName+"\" a un score (final) de "+QString::number(bestScore));
    return bestName;
}





HWND image::findApp(QString name)
{return FindWindow(nullptr, (const wchar_t*)name.utf16());}
PWINDOWINFO image::findInfoApp(QString name)
{
    PWINDOWINFO info = new WINDOWINFO();
    info->cbSize = (uint)sizeof(info);
    GetWindowInfo(findApp(name), info);
    return info;
}
QPoint image::getLeftTopFen(QString name)
{
    RECT rect = findInfoApp(name)->rcClient;
    return QPoint(rect.left, rect.top);
}

QImage *image::negaImgs(const QImage *img1, const QImage *img2)
{
    QImage *retour = new QImage(getMinSize(img1->size(), img2->size()), img1->format());
    retour->fill(QColor(0, 0, 0, 0));
    //retour->setAlphaChannel(*retour);
    for(int iy=0; iy<retour->height(); iy++)
        for(int ix=0; ix<retour->width(); ix++)
            if(img1->pixelColor(ix, iy) != img2->pixelColor(ix, iy) && img1->pixelColor(ix, iy).alpha() != 0) {
                retour->setPixelColor(ix, iy, img1->pixelColor(ix, iy));//si les couleurs sont != et que c'est pas transparent
            }
    return retour;
}

bool image::isTransparent(const QImage *img)
{
    if(img == nullptr || img->isNull())
        return true;//elle est pas pleine
    for(int iy=0; iy<img->height(); iy++)
        for(int ix=0; ix<img->width(); ix++)
            if(img->pixelColor(ix, iy) != QColor(0, 0, 0, 0))
                return false;//si un pixel est pas transparent, c'est non
    return true;
}

int image::pixTransparent(const QImage *img)
{
    if(img == nullptr || img->isNull())
        return 0;
    int nb=0;
    for(int iy=0; iy<img->height(); iy++)
        for(int ix=0; ix<img->width(); ix++)
            if(img->pixelColor(ix, iy) == QColor(0, 0, 0, 0))
                nb++;//si un pixel est transparent
    return nb;
}
