#include "mc_gui.h"


mc_gui::mc_gui(const QImage *currentScreenshotP, debug *debP)
{
    deb = debP;
    currentScreenshot = currentScreenshotP;
    currentScreenGui = new QImage(1, 1, QImage::Format_RGB16);
}
void mc_gui::setMCFolder(QString folderGui)
{
    if(MCFolder == folderGui)//pas besoin de réactualiser
        return;
    MCFolder = folderGui; reloadGui();
}
void mc_gui::setCurrentScreenshot(const QImage *screenshot)
{
    currentScreenshot = screenshot;
    actuCurrentGui();
}



QRect mc_gui::rectWithSize(QRect rect) const
{//transfo un rect pour le mettre à la taille de l'interface
    QPoint pt(rect.x(), rect.y());
    pt *= MCSize;
    QSize size(rect.size()*MCSize);
    return QRect(pt, size);
}
QSize mc_gui::getTailleCase() const {return QSize(tailleCase * MCSize, tailleCase * MCSize);}



QMap<int, QString> mc_gui::getCurrentItem() {return currentItems;}
QMap<int, int> mc_gui::getCurrentItemSize() {return currentItemsNb;}
QPoint mc_gui::getPosGuiWindow(QString MCApp) const
{
    QPointF pt = image::getMidPoint(currentScreenshot, getGui(currentGui));
    pt.setX(pt.x() + image::findInfoApp(MCApp)->rcClient.left);//on met par rapport à l'écran windows
    pt.setY(pt.y() + image::findInfoApp(MCApp)->rcClient.top);
    return pt.toPoint();
}

QRect mc_gui::posCase(int caseCible, QString MCApp) const
{
    QPointF pt(0, 0);//si on a pas d'app, on prend le 0, 0
    if(MCApp != "")
        pt = getPosGuiWindow(MCApp);//si on a une app, on prend la pos du gui
    if(caseCible == -1) {//on veut le vide
        pt.setX(pt.x()-10);
        pt.setY(pt.y()-10);
        return QRect(pt.toPoint(), QSize(1,1));
    }
    pt.setX(pt.x() + GuiCase.value(currentGui).value(caseCible).x()*MCSize);
    pt.setY(pt.y() + GuiCase.value(currentGui).value(caseCible).y()*MCSize);
    return QRect(pt.toPoint(), getTailleCase());//rectangle de la case, .center() pour avoir le milieu de la case
}






QString mc_gui::actuCurrentGui()
{
    actuBorderSize();
    currentRectGui = getBorderPos();
    *currentScreenGui = currentScreenshot->copy(currentRectGui);
    if(currentScreenshot->size() != currentScreenGui->size()) {
        QMap<QString, const QImage*> *imgsGuiSize = new QMap<QString, const QImage*>;
        for(int i=0; i<GuiList.size(); i++) {//on parcours les gui
            if(getGui(GuiList.at(i), false) != nullptr && getGui(GuiList.at(i), false)->size() == currentScreenGui->size()) {//si c'est la meme taille, on garde
                imgsGuiSize->insert(GuiList.at(i), getGui(GuiList.at(i), false));//on prend ceux sans case
            }
        }
        currentGui = image::findBestImage(currentScreenGui, imgsGuiSize, 90);
    }
    else
        currentGui = "";//pas besoin de calculer
    image::save(currentScreenGui, "currentScreenGui", MCFolder+"\\test\\");
    image::save(image::relativeScaled(currentScreenGui, 1.0/static_cast<double>(MCSize)), "currentScreenGui_1", MCFolder+"\\test\\");

    if(currentGui != "") {
        deb->addLog("minecraft/gui", "Gui détecté : "+currentGui, 3);
        actuItem();
    }
    return currentGui;
    //pas besoin de retour car on veut juste actu le current mais c'est plus propre
}

const QImage *mc_gui::getGui(QString name, bool withCase, int size) const
{
    if(imgsGui.value(name, nullptr) != nullptr)//si c'est un nom direct avec la size avant ("2_chest")
        return imgsGui.value(name, nullptr);
    if(size < 1 || GuiMaxSize < size)//si size n'est pas défini, on met par défault
        return getGui(name, withCase, MCSize);//on refait avec MCSize
    if(withCase) {
        return imgsGui.value(QString::number(size)+"_"+name, nullptr);//sinon on retourne la size donnée
    }
    else {
        return imgsGui.value(QString::number(size)+"_case_"+name, nullptr);//ou sans case si on précise
    }
}

QString mc_gui::getCurrentGui() const {return currentGui;}

QRect mc_gui::getBorderPos() const
{
    QRect retour(0, 0, 0, 0);
    retour.setLeft(findBorder(0, MCSize).x());
    retour.setTop(findBorder(1, MCSize).y());
    retour.setRight(findBorder(2, MCSize).x());
    retour.setBottom(findBorder(3, MCSize).y());
    return retour;
}









void mc_gui::reloadGui()
{
    imgsGui.clear();
    if(MCFolder == "")//on calcul pas tout pour rien
        return;
    //on charge les gui avec la taille 1
    for(int i = 0; i<GuiList.size(); i++) {
        if(GuiList.at(i) == "chest" || GuiList.at(i) == "chest_big") {
            QImage *imgGeneric = image::load(MCFolder+"\\data\\gui", "generic_54");
            if(imgGeneric == nullptr) {
                //imgsGui.insert("1_"+GuiList.at(i), nullptr);
                continue;
            }
            QImage *imgOut = new QImage();
            if(GuiList.at(i) == "chest") {
                *imgOut = QImage(imgGeneric->width(), 167, imgGeneric->format());
                QPainter painter(imgOut);
                painter.drawImage(0, 0, imgGeneric->copy(0, 0, imgGeneric->width(), 71));
                painter.drawImage(0, 71, imgGeneric->copy(0, 126, imgGeneric->width(), 96));
                painter.end();
            }
            else if(GuiList.at(i) == "chest_big") {
                *imgOut = QImage(imgGeneric->width(), 221, imgGeneric->format());
                QPainter painter(imgOut);
                painter.drawImage(0, 0, imgGeneric->copy(0, 0, imgGeneric->width(), 125));
                painter.drawImage(0, 125, imgGeneric->copy(0, 126, imgGeneric->width(), 96));
                painter.end();
            }
            delete imgGeneric;

            imgsGui.insert("1_"+GuiList.at(i), imgOut);
        }
        else {
            const QImage *imgGui = image::load(MCFolder+"\\data\\gui", GuiList.at(i));
            if(imgGui != nullptr && !imgGui->isNull())
                imgsGui.insert("1_"+GuiList.at(i), imgGui);
        }
    }
    deb->addLog("minecraft/gui", QString::number(imgsGui.size())+" textures Gui chargées.", 3);
    //on obtient une map avec des images de type: 1_inventory, 1_crafting_table, 2_inventory, 2_crafting_table...

    //on enregistre la positiond de cases des gui
    QImage *imgCase = image::load(MCFolder+"\\data\\gui", "air");
    QImage *imgCaseBig = image::load(MCFolder+"\\data\\gui", "air_big");
    if(imgCase!=nullptr && imgCaseBig!=nullptr) {
        for(int i=0; i<GuiList.size(); i++) {
            const QImage *imgGui = getGui(GuiList.at(i), true, 1);
            if(imgGui == nullptr || imgGui->isNull())
                continue;

            for(int iy=0; iy<imgGui->height(); iy++)
                for(int ix=0; ix<imgGui->width(); ix++) {
                    if(imgGui->pixelColor(ix, iy) == imgCase->pixelColor(0, 0) ||
                            imgGui->pixelColor(ix, iy) == imgCaseBig->pixelColor(0, 0)) {//si c'est le coins d'un des 2 trucs
                        if(getCaseAt(GuiList.at(i), ix, iy) == -1) {//si c'est pas déja dans une case
                            if(image::ressemblance(new QImage(imgGui->copy(ix, iy, imgCaseBig->width(), imgCaseBig->height())),
                                                   imgCaseBig) >= 95) {
                                GuiCase[GuiList.at(i)].insert(GuiCase.value(GuiList.at(i)).size(), QPoint(ix+4, iy+4));//+4 car c'est la pos exact de la case
                                ix += imgCase->width()-1;//plus rapide (& sécurité)
                            }
                            else if(image::ressemblance(new QImage(imgGui->copy(ix, iy, imgCase->width(), imgCase->height())),
                                                        imgCase) >= 95) {
                                GuiCase[GuiList.at(i)].insert(GuiCase.value(GuiList.at(i)).size(), QPoint(ix, iy));
                                ix += imgCase->width()-1;
                            }
                        }
                    }
                }
        }
    }

    //on ajoute les version avec les cases vides
    for(int i = 0; i<GuiList.size(); i++) {
        const QImage *imgGuic = getGui(GuiList.at(i), true, 1);
        if(imgGuic == nullptr || imgGuic->isNull())
            continue;
        QImage *imgGui = new QImage(*imgGuic);
        QMap<int, QPoint> posPt = GuiCase.value(GuiList.at(i));
        for(int iC=0; iC<posPt.size(); iC++)//on parcours les cases
            for(int iy=0; iy<tailleCase; iy++)
                for(int ix=0; ix<tailleCase; ix++)
                    imgGui->setPixelColor(ix+posPt.value(iC).x(), iy+posPt.value(iC).y(), QColor(0, 0, 0, 0));
        imgsGui.insert("1_case_"+GuiList.at(i), imgGui);//on ajoute l'image
    }


    //on ajoute les gui plus grands
    for(int i = 0; i<GuiList.size(); i++) {//on obtient les gui avec des sizes plus grandes
        QString name = GuiList.at(i);
        if(getGui(name, true, 1) == nullptr)
            continue;
        for(int iS=1; iS <= GuiMaxSize; iS++) {
            imgsGui.insert(QString::number(iS) + "_" + name,//nom avec la taille
                            image::relativeScaled(getGui(name, true, 1), iS));//recadre l'image
        }
    }
    //version case_
    for(int i = 0; i<GuiList.size(); i++) {//on obtient les gui avec des sizes plus grandes (sans case)
        QString name = GuiList.at(i);
        if(getGui(name, true, 1) == nullptr)
            continue;
        for(int iS=1; iS <= GuiMaxSize; iS++) {
            imgsGui.insert(QString::number(iS) + "_case_" + name,//nom avec la taille
                            image::relativeScaled(getGui(name, false, 1), iS));//recadre l'image
        }
    }
    reloadFont();

    deb->addLog("minecraft/gui", "Chargements du gui terminé.", 3);
}

QPoint mc_gui::findBorder(int border, int size) const
{
    //va du centre vers le bord pour trouver la position de la bordure donnée (0, 1, 2 ou 3)
    QImage *borderI=nullptr;
    int xP=0, yP=0;
    switch (border) {
        case 0:
            borderI = image::load(MCFolder+"\\data\\gui", "border_left");
            xP = -1;
            break;
        case 1:
            borderI = image::load(MCFolder+"\\data\\gui", "border_top");
            yP = -1;
            break;
        case 2:
            borderI = image::load(MCFolder+"\\data\\gui", "border_right");
            xP = 1;
            break;
        case 3:
            borderI = image::load(MCFolder+"\\data\\gui", "border_bottom");
            yP = 1;
            break;
        default:
            return currentScreenshot->rect().center();
    }
    borderI = image::relativeScaled(borderI, size);//on met par rapport à la taille donnée
    QPoint retour = findBorder(borderI, xP, yP);
    if(borderI != nullptr) delete borderI;
    return retour;
}
QPoint mc_gui::findBorder(QImage *imgBorder, int xP, int yP) const
{
    //va du centre et part vers un bord (en fonction de xP et yP) et s'arrete quand il a trouvé imgBorder
    if(imgBorder == nullptr) {
        if(currentScreenshot == nullptr)
            return QPoint(0, 0);
        return currentScreenshot->rect().center();
    }
    int x = currentScreenshot->rect().center().x()-imgBorder->width()/2;
    int y = currentScreenshot->rect().center().y()-imgBorder->height()/2;//prend le milieu
    int maxX = currentScreenshot->width()-imgBorder->width();
    int maxY = currentScreenshot->height()-imgBorder->height();
    while(0 <= x && x <= maxX  && 0 <= y && y <= maxY)
    {
        double score = image::ressemblance(new QImage(currentScreenshot->copy(x, y, imgBorder->width(), imgBorder->height())),
                               imgBorder);
        if(score == 100.0)
        {
            if(xP == 1)
                x += imgBorder->width()-1;//avoir le point le plus à l'extrémitée
            if(yP == 1)
                y += imgBorder->height()-1;
            return QPoint(x, y);
        }
        x += xP;//on se déplace selon la bordure (on va de l'intérieur vers l'extérieur)
        y += yP;
    }
    if(x < 0) {x=0;} if(y<0) {y=0;}//on remet dans le cadre
    if(x > maxX) {x=currentScreenshot->width()-1;}
    if(y>maxY) {y=currentScreenshot->height()-1;}//maxX est la limite mais on prend le poit le plus en bas à droite
    return QPoint(x, y);
}
void mc_gui::actuBorderSize()
{//on check quel est la taille de la bordure qui convient
    QImage *borderLight = image::load(MCFolder+"\\data\\gui", "border_left_light");
    if(borderLight == nullptr || borderLight->isNull())
        return;
    QPoint posBorder = findBorder(borderLight, -1, 0);
    for(int i=2; i <= GuiMaxSize+1; i++) {//i=2 car on a déja test le 1 (dans findborder)
        QImage borderLightSize = *image::relativeScaled(borderLight, i);
        //*borderLeftSize = borderLeftSize->copy(0, 0, i*borderLeft->width(), borderLeft->height());//on veut pas que ça descende de trop
        double score=image::ressemblance(new QImage(currentScreenshot->copy(posBorder.x()-i+1,
                                                                         posBorder.y(),
                                                                         borderLightSize.width(),
                                                                         borderLightSize.height())), &borderLightSize);
        if(score < 90.0) {//c'est la cata (90 au cas ou un truc survol?)
            //if(i-1 != MCSize)
            //    deb->addLog("minecraft/gui", "La taille à été mise à "+QString::number(i-1)+" score suivant:"+QString::number(score));
            setMCSize(i-1);//size avant la cata
            return;
        }
    }
    setMCSize(GuiMaxSize);
    deb->addLog("minecraft/gui", "La taille à été mise à "+QString::number(MCSize)+" mais elle est peut etre plus grande", 2);
}
void mc_gui::setMCSize(int size)
{MCSize = (1 <= size && size <= GuiMaxSize) ? size : 0;}//si la taille est pas connue, c'est 0

int mc_gui::getCaseAt(QString gui, int x, int y) const
{
    QMap<int, QPoint> caseGui = GuiCase.value(gui);
    QMapIterator<int, QPoint> i(caseGui);
    while(i.hasNext()) {
        i.next();
        if(posCase(i.key()).contains(QPoint(x, y))) {
            return i.key();
        }
    }
    return -1;
}





void mc_gui::reloadItems()
{//on charge tous les fichiers présents dans le dossier
    imgsItem.clear();
    if(MCFolder == "")
        return;

    QDir dir(MCFolder+"\\data\\items_"+QString::number(MCSize));
    dir.setNameFilters({"*.png"});//on veut que les .png
    QStringList listeItems = dir.entryList();
    dir.setPath(MCFolder+"\\data\\items_"+QString::number(MCSize)+"\\unknown");//pour éviter de les créer 2 fois
    QStringList listeItems2 = dir.entryList();

    for(int i=0; i<listeItems.size()+listeItems2.size(); i++) {
        QString name = "";
        if(i < listeItems.size()) {
            name = listeItems.at(i);
        }
        else
            name = listeItems2.at(i-listeItems.size());
        if(name.endsWith(".png")) {
            name = name.mid(0, name.size()-4);//retire ".png"
            QImage *img = nullptr;
            if(i < listeItems.size()) {
                img = image::load(MCFolder+"\\data\\items_"+QString::number(MCSize), name);
            }
            else
                img = image::load(MCFolder+"\\data\\items_"+QString::number(MCSize)+"\\unknown", name);
            imgsItem.insert(name, img);

            //et la version avec calque
            QImage *imgSelec = new QImage(*img);
            for(int iy=0; iy<img->height(); iy++)
                for(int ix=0; ix<img->width(); ix++) {
                    int red = imgSelec->pixelColor(ix, iy).red();
                    int green = imgSelec->pixelColor(ix, iy).green();
                    int blue = imgSelec->pixelColor(ix, iy).blue();
                    //la transparance : color = color + (colorCalque-color) * (alpha/256)
                    //plus alpha diminue, moins on rajoute, si alpha==128, on rajoute qu'a moitié
                    imgSelec->setPixelColor(ix, iy, QColor(red+(256-red)*128/256, green+(256-green)*128/256, blue+(256-blue)*128/256));
                }
            imgsItem.insert("selected_"+name, imgSelec);
        }
    }
}
QString mc_gui::findItem(const QImage *caseScreen) const
{return image::findBestImageContains(caseScreen, &imgsItem, 98);}

void mc_gui::actuItem()
{
    currentItems.clear();
    currentItemsNb.clear();
    if(currentGui=="")
        return;
    reloadItems();//on reload que ici pour pas de soucis (charger les new items en plus)
    /*if(GuiCase.value(currentGui).isEmpty()) {//si l'actuel gui n'a pas de case attitrées, on arrête
        deb->addLog("minecraft/gui", "actuItem ne connait pas le gui "+currentGui, 1);
        return retour;
    }*/

    //QImage *screenGui = image::cutContainsMid(currentScreenshot, getGui(currentGui));
    QImage *screenGui = image::negaImgs(currentScreenGui, getGui(currentGui));
    screenGui->save(MCFolder+"\\test\\coloredGui-1.png");
    QMap<int, QPoint> cases = GuiCase.value(currentGui);
    QMapIterator<int, QPoint> i(cases);
    while(i.hasNext()) {
        i.next();
        QRect caseRect = QRect(i.value() * MCSize, getTailleCase());
        //on copie avec la taille d'une case et la position (mais le tout remis à la bonne taille)
        QImage *caseScreen = new QImage(screenGui->copy(caseRect));
        QImage *caseScreenNb = new QImage(*caseScreen);

        //on met la partie des nombres en transparent
        for(int iy=10*MCSize; iy<getTailleCase().height(); iy++)
            for(int ix=6*MCSize; ix<getTailleCase().width(); ix++)
                caseScreen->setPixelColor(ix, iy, QColor(0, 0, 0, 0));

        QString nameItem = findItem(caseScreen);
        if(image::isTransparent(caseScreen))
            nameItem = "air";//si c'est full transparent, c'est de l'air
        if(nameItem == "") {//si on connais pas, on enregistre
            image::save(caseScreen, "unknown-"+QString::number(i.key())+"-"+QDateTime::currentDateTime().toString("yymmdd-HHmmss"), MCFolder+"\\data\\items_"+QString::number(MCSize)+"\\unknown");
        }
        currentItems.insert(i.key(), nameItem);



        //on calcul la taille des stacks
        *caseScreenNb = caseScreenNb->copy(QRect(QPoint(6, 10)*MCSize,
                                                 QSize(tailleCase-6, tailleCase-10)*MCSize));
        QString nbItemStr = findText(caseScreenNb, false);
        QString nbItemStr2 = "";
        for(int i=0; i<nbItemStr.size(); i++)//on retire les éventuels chars
            if(QString("0123456789").contains(nbItemStr.at(i)))//si c'est un nb
                nbItemStr2 += nbItemStr.at(i);
        int nbItem = nbItemStr2.toInt();
        if(nbItem == 0)
            nbItem = 1;
        //if(nameItem != "" && nameItem != "air")
        //    qDebug()<<"actuItem-findText"<<i.key()<<nbItem<<nameItem;
        currentItemsNb.insert(i.key(), nbItem);


        //colorer l'interface pour les tests
        for(int iy=0; iy<caseRect.height(); iy++)
            for(int ix=0; ix<caseRect.width(); ix++)
                screenGui->setPixelColor(ix+caseRect.x(), iy+caseRect.y(), QColor(0,255,0));


    }
    screenGui->save(MCFolder+"\\test\\coloredGui-2.png");
}



void mc_gui::reloadFont()
{
    fontChar.clear();
    imgsFont.clear();
    for(int i=0; i<1; i++) {
        QImage *font = nullptr;
        QString charsFont;
        int tailleChar=1;
        QString name="";
        switch (i) {
            case 0:
                name = "ascii";
                charsFont = "                                 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£ ×ƒáíóúñÑªº¿©¬½¼¡«»░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀αßΓπΣσμ ΞθΩδ∞∅∈∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ ";
                //le char n°48 (Ø) se confond avec 0
                tailleChar=8;
                break;
            default:
                continue;
        }
        font = image::load(MCFolder+"\\data\\font", name);
        if(font == nullptr)
            continue;
        int nbX = font->width() / tailleChar;
        int nbY = font->height() / tailleChar;
        for(int iy=0; iy<nbY; iy++)
            for(int ix=0; ix<nbX; ix++) {
                int posChar = ix+iy*nbX;
                if(charsFont.at(posChar) == " " || charsFont.at(posChar)== "")//on rajoute pas les espaces/les vides
                    continue;
                fontChar.insert(name+"_"+QString::number(posChar), charsFont.at(posChar));
                QImage *imgChar = new QImage(font->copy(QRect(QPoint(ix, iy)*tailleChar,
                                                              QSize(1,1)*tailleChar)));
                while(!imgChar->isNull() && image::isTransparent(new QImage(imgChar->copy(imgChar->width()-2, 0, 2, tailleChar)))) {
                    //on retire la bande de fin (il faut en laisser une quand meme)
                    *imgChar = imgChar->copy(0, 0, imgChar->width()-1, imgChar->height());
                }
                if(imgChar->isNull()) {
                    deb->addLog("minecraft/gui", "reloadFont : imgChar est null pour le fichier "+name+" avec le charactère "+QString::number(posChar)+"("+charsFont.at(posChar)+")", 2);
                    continue;//on a eu un mauvais renseignement
                }
                imgsFont.insert(name+"_"+QString::number(posChar), imgChar);

            }
    }
    //on rajoute l'espace (3 de long et 8 de haut avec la taille 1)
    QImage *imgSpace = image::convertToGoodFormat(new QImage(4, 8, QImage::Format_ARGB32));
    imgSpace->fill(QColor(0, 0, 0, 0));
    imgsFont.insert("space", imgSpace);
    fontChar.insert("space", " ");

    deb->addLog("minecraft/gui", QString::number(imgsFont.size())+" charactères chargés.", 3);
}
QString mc_gui::findText(const QImage *img, bool LeftToRight)
{
    QString idChar="";
    QString retour="";
    QImage *img2 = new QImage(*img);
    //image::save(img, "findText-"+QTime::currentTime().toString("HHmmss.zzz"), MCFolder+"\\test");
    while(img2->width() > 0) {//tant qu'il en reste
        idChar = findChar(img2, LeftToRight);
        if(idChar == "")//si il trouve pas on arrete
            break;
        int posX = imgsFont.value(idChar)->width()*MCSize;
        if(LeftToRight) {
            *img2 = img2->copy(posX, 0, img2->width()-posX, img2->height());//on retire le 1er char
            retour += fontChar.value(idChar);
        }
        else {
            *img2 = img2->copy(0, 0, img2->width()-posX, img2->height());//on retire le dernier char
            retour = fontChar.value(idChar) + retour;
        }
    }
    return retour;
}

QString mc_gui::findChar(const QImage *img, bool Left)//retourne l'a case'id du char
{
    QMapIterator<QString, const QImage*> i(imgsFont);
    QString bestId = "";
    int bestPixs = 0;//meilleur nb de pixels donnés
    QImage *img2 = image::convertToGoodFormat(new QImage(1,1,QImage::Format_ARGB32));
    while(i.hasNext()) {
        i.next();
        if(i.key() == "space")
            continue;//on check pas les space, ils gagnent à tous les coups
        if(Left) {
            *img2 = img->copy(QRect(QPoint(0, 0), i.value()->size()*MCSize));
        }
        else
            *img2 = img->copy(QRect(QPoint(img->width()-i.value()->width()*MCSize, 0), i.value()->size()*MCSize));
        QImage *imgFont = image::relativeScaled(i.value(), MCSize);
        int pixSim = image::pixelsSim(img2, imgFont);

        double score = static_cast<double>(pixSim) *100 / static_cast<double>(image::getNbPixsMin(img2, imgFont));
        pixSim -= image::pixTransparent(imgFont);//on retire ici pour garder le score comme il est
        if(score >= 99.9) {
            //qDebug()<<"findChar"<<i.key()<<score<<pixSim<<bestPixs<<fontChar.value(i.key());
            if(bestPixs < pixSim) {
                bestId = i.key();
                bestPixs = pixSim;
            }
        }
        //image::save(imgFont, "findChar-imgFont"+fontChar.value(i.key())+"-"+i.key(), MCFolder+"\\test");
    }
    if(bestId != "")
        return bestId;
    if(img->width() >= 4*MCSize)//si on peut mettre un espace
        return "space";
    return "";
}



