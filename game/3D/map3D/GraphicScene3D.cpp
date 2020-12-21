#include "GraphicScene3D.h"

GraphicScene3D::GraphicScene3D()
{
    client=new obj3D();
    imgCamera.load("C:/Qt/MesProg/jeux/map3D/ressourcepacks/defaut/textures/camera.png");
    imgCamera=imgCamera.scaled(imgCamera.width()*2, imgCamera.height()*2);
    objets=new QList<obj3D>;
}

void GraphicScene3D::addObj(obj3D &obj) {objets->append(obj);}

void GraphicScene3D::addCube(quint64 x, quint64 y, quint64 z, QString fileNameTop, QString fileNameBottom, QString fileNameFace)
{
    objets->append(obj3D({QPoint3D(0,0,0), QPoint3D(1,0,0), QPoint3D(1,1,0), QPoint3D(0,1,0), QPoint3D(0,0,1), QPoint3D(1,0,1), QPoint3D(1,1,1), QPoint3D(0,1,1)}, x, y, z, 0, 0));
    objets->last().addTexture({0, 1, 2, 3}, fileNameFace);//1, 2, 3, 0
    objets->last().addTexture({4, 5, 6, 7}, fileNameFace);//5, 6, 7, 4
    objets->last().addTexture({0, 1, 5, 4}, fileNameBottom);//0, 1, 5, 4
    objets->last().addTexture({2, 6, 7, 3}, fileNameTop);
    objets->last().addTexture({1, 5, 6, 2}, fileNameFace);//5, 6, 2, 1
    objets->last().addTexture({4, 0, 3, 7}, fileNameFace);//0, 3, 7, 4
    //organisé pour faire +x; +y; -x; -y
}

obj3D GraphicScene3D::calcFace(const obj3D &obj)
{

    QPoint3D centreObj=calcPoint(QPoint3D(obj.x(), obj.y(), obj.z()));
    obj3D retour = obj3D(centreObj.x(), centreObj.y(), centreObj.z(), 0, 0);//nouvelle rotation: 0
    for(int i=0; i<obj.getPoint().size(); i++)//parcours les points de obj pour les faire tourner dans le graphique
    {
        retour.append(calcPoint(obj.at(i), QPoint3D(obj.x(), obj.y(), obj.z())));
    }
    retour.setTexture(obj.getTexture());
    return retour;
}

QPoint3D GraphicScene3D::calcPoint(QPoint3D point, QPoint3D ptplus)
{
    double x=point.x()+ptplus.x()-client->x();//selon le client, new referentiel
    double y=point.y()+ptplus.y()-client->y();
    double z=point.z()+ptplus.z()-client->z();
    int rX=client->rX();
    int rY=client->rY()-90;
    double x2=obj3D::rotation1(x, z, rX);//x-z
    double z2=obj3D::rotation2(x, z, rX);
    double y2=obj3D::rotation2(z2, y, rY);//z-y
    z2=obj3D::rotation1(z2, y, rY);
    QPoint3D retour(x2, y2, z2);
    return retour;
    //verifier ces trucs, peut etre parce que j'ai ajouté "client" pour une caméra déplaçable?
}

QPixmap GraphicScene3D::refresh(quint64 width, quint64 height, double x, double y, double z, int rX, int rY, double zoom, double perspective)
{
    qDebug()<<"refresh: pos"<<x<<y<<z<<"rotation"<<rX<<rY<<"zoom"<<zoom<<perspective;
    client->setX(x);
    client->setY(y);
    client->setZ(z);
    client->setRX(rX);
    client->setRY(rY);
    QList<obj3D> newObjets=refreshCalc(width, height, zoom, perspective);
    QImage img(width, height, QImage::Format_RGB32);//il faudra penser que le 0 0 de l'image se trouve à width/2 height/2
    img.fill(Qt::black);

    /*for(int i=0; i<newObjets.size(); i++)
    {//dessiner les points (inutile avec les images)
        for(int i2=0; i2<newObjets[i].getPoint().size(); i2++)
        {
            double xPoint=newObjets[i][i2].xPerspective();
            double yPoint=newObjets[i][i2].yPerspective();
            double zPoint=newObjets[i][i2].zPerspective();
            //qDebug()<<"point"<<i3<<"en"<<xPoint<<yPoint<<zPoint;
            if(zPoint<=0)//si c'est devant soit, on peut regarder (+ pour derrière sois et - pour derière le 0 0)
            {
                if(0 <= xPoint && xPoint < width && 0 <= yPoint && yPoint < height)//si dans l'image
                {
                    img.setPixelColor(xPoint, yPoint, Qt::white);
                    //qDebug()<<"points"<<i3<<"déssiné";
                }
                //else
                    //qDebug()<<"points"<<i3<<"hors de porté";
            }
            //else
                //qDebug()<<"points"<<i3<<"derrière la caméra";
        }
    }*/
    //return QPixmap::fromImage(img);

    img=refreshDraw(img, newObjets);
    img=drawAxes(img);
    return QPixmap::fromImage(img);
}

QList<obj3D> GraphicScene3D::refreshCalc(quint64 width, quint64 height, double zoom, double perspective)
{//tourne tous les objs
    QMultiMap<double, obj3D> retourMap;
    for(int i=0; i<objets->size(); i++)
    {
        obj3D obj=calcFace(objets->at(i));
        if(obj.getPoint().isEmpty())
            continue;//erreur, il y a rien dans l'objet

        for(int i2=0; i2<obj.getPoint().size(); i2++)
        {
            //.merge parce que sinon il prendra pas le bon objet (pointsP / pointsRP)
            obj.mergePoints(i2).setPerspective(width, height, perspective, zoom);
            //obj.mergePoints(i2).setX(obj[i2].x());//*zoom*d+width/2);
            //obj.mergePoints(i2).setY(obj[i2].y());//*zoom*d+height/2);
            //obj.mergePoints(i2).setZ(obj[i2].z());//*zoom);
        }
        //trie les objs
        retourMap.insert(obj.z(), obj);
    }
    return retourMap.values();
}

QImage GraphicScene3D::refreshDraw(QImage img, QList<obj3D> objs)
{
    for(int i=0; i<objs.size(); i++)//dessiner
    {//on parcours les obj3D du plus loins au plus proche
        if(objs.at(i).z() > client->z())//si c'est la place de la caméra, on l'insere vite fait
            img=drawCamera(img);

        QList<QList<quint64>> textureTrieur(objs.at(i).getTextureSortZ());//liste des textures : liste des points
        QMap<QList<quint64>, QString> texture=objs.at(i).getTexture();//toutes les textures d'obj
        for(int i2=0; i2<textureTrieur.size(); i2++)
        {//parcours les faces (triées par z) du plus loins au plus proche
            //défini avec:QMatrix(xHautDroite, yHautDroite, xBasGauche, yBasGauche) mais comme dessiné avec x et y sur l'autre img...

            //les coo du centre de la face
            QPointF ptImg(objs.at(i).getMoyenneTexture(textureTrieur.at(i2)).x(),
                         objs.at(i).getMoyenneTexture(textureTrieur.at(i2)).y());

            if(objs.at(i).getMoyenneTexture(textureTrieur.at(i2)).z()>0)
                continue;//quand on aurra la perspect, plus besoin (enleve meme des parties de textures)
            QPointF ptImg2(0,0);//test, sans correction du milieu
            QImage img2=calcImg(objs.at(i), textureTrieur.at(i2), texture.find(textureTrieur.at(i2)).value(), ptImg2);
            img=compileImage(img, 0*ptImg.x()+ptImg2.x(), 0*ptImg.y()+ptImg2.y(), img2);
        }
        if(objs.at(i).z() <= client->z() && i==objs.size()-1)//si c'est pas encore bon mais que c'est le dernier
            img=drawCamera(img);
    }
    return img;
}

QImage GraphicScene3D::compileImage(QImage source, quint64 x, quint64 y, QImage plus)
{//compile le milieu de plus sur source en x,y
    int x2;
    int y2;
    for(int ix=0; ix<plus.width(); ix++)
    {
        x2=ix+x-plus.width()/2;
        if(source.width() <= x2 || x2 < 0)
            continue;
        for(int iy=0; iy<plus.height(); iy++)
        {
            y2=iy+y-plus.height()/2;
            if(source.height() <= y2 || y2 < 0)
                continue;
            QColor pix1=source.pixelColor(x2, y2);
            QColor pix2=plus.pixelColor(ix, iy);
            double r1=pix1.red();//double à cause des divisions
            double g1=pix1.green();
            double b1=pix1.blue();
            double alpha1=pix1.alpha();
            double r2=pix2.red();
            double g2=pix2.green();
            double b2=pix2.blue();
            double alpha2=pix2.alpha();
            //pix2 sur pix1
            int r=(r2*alpha2 + r1*alpha1*(255-alpha2)/255) / (alpha2+alpha1*(255-alpha2)/255);
            int g=(g2*alpha2 + g1*alpha1*(255-alpha2)/255) / (alpha2+alpha1*(255-alpha2)/255);
            int b=(b2*alpha2 + b1*alpha1*(255-alpha2)/255) / (alpha2+alpha1*(255-alpha2)/255);
            int alpha=alpha2 + alpha1*(255-alpha2)/255;
            pix2.setRed(r);
            pix2.setGreen(g);
            pix2.setBlue(b);
            pix2.setAlpha(alpha);
            source.setPixelColor(x2, y2, pix2);
        }
    }
    return source;
}

QImage GraphicScene3D::calcImg(obj3D obj, QList<quint64> pts, QString fileName, QPointF &ptCentre)//l'obj, la texture demandée et ça pos
{
    QImage imgFace=loadImg(fileName);
    double xImg0=obj.at(pts.at(0)).xPerspective();
    double yImg0=obj.at(pts.at(0)).yPerspective();
    double xImg1=obj.at(pts.at(1)).xPerspective();
    double yImg1=obj.at(pts.at(1)).yPerspective();
    double xImg2=obj.at(pts.at(2)).xPerspective();
    double yImg2=obj.at(pts.at(2)).yPerspective();
    double xImg3=obj.at(pts.at(3)).xPerspective();
    double yImg3=obj.at(pts.at(3)).yPerspective();
    double xMin=xImg0, xMax=xImg0, yMin=yImg0, yMax=yImg0;
    for(int i=1; i<=3; i++)
    {
        if(obj.at(pts.at(i)).xPerspective()<xMin) xMin=obj.at(pts.at(i)).xPerspective();
        if(obj.at(pts.at(i)).xPerspective()>xMax) xMax=obj.at(pts.at(i)).xPerspective();
        if(obj.at(pts.at(i)).yPerspective()<yMin) yMin=obj.at(pts.at(i)).yPerspective();
        if(obj.at(pts.at(i)).yPerspective()>yMax) yMax=obj.at(pts.at(i)).yPerspective();
    }
    if(xMin<-10000 || 10000<xMax || yMin<-10000 || 10000<yMax ||//si c'est trop loins
            xMin!=xMin || xMax!=xMax || yMin!=yMin || yMax!=yMax ||//si c'est nan
            xMax<0 || yMax<0)//si les maxs/mins sont pas ds leurs cadres
    {//(les maxs sont stopés par la taille de l'image de base, pas la face)
        qDebug("pos trop loins, retourne null image");
        return imgFace.scaled(0,0);
    }
    //fonctionne mais remplacé par transform pour perspective
    //QMatrix matrix((xImg3-xImg0)/16, (yImg3-yImg0)/16, (xImg1-xImg0)/16, (yImg1-yImg0)/16, 0, 0);//tests
    //défini avec:QMatrix(xHautDroite, yHautDroite, xBasGauche, yBasGauche) mais comme dessiné avec x et y sur l'autre img...
    //imgFace.setPixelColor(0, 0, Qt::red);
    //imgFace.setPixelColor(imgFace.width()-1, imgFace.height()-1, Qt::green);
    //imgFace.setPixelColor(0, imgFace.height()-1, Qt::blue);
    QTransform trans;
    QPolygonF polyImg(QRectF(imgFace.rect()));
    polyImg.removeLast();//le QRectF met 5 pts pour faire le tour
    QPolygonF poly2({QPointF(xImg0,yImg0),
                     QPointF(xImg1,yImg1),
                     QPointF(xImg2,yImg2),
                     QPointF(xImg3,yImg3)});
    if(!QTransform::quadToQuad(polyImg, poly2, trans))
    {
        QString d1="{";
        for(int i=0; i<polyImg.size(); i++)
            d1 += QString::number(polyImg.at(i).x())
                 +QString::number(polyImg.at(i).y())+", ";
        d1 = d1.left(d1.lastIndexOf(", "))+"}";
        QString d2="{";
        for(int i=0; i<poly2.size(); i++)
            d2 += QString::number(poly2.at(i).x())+" "
                 +QString::number(poly2.at(i).y())+", ";
        d2 = d2.left(d2.lastIndexOf(", "))+"}";

        qDebug()<<"transformation impossible"<<d1<<d2;
        return imgFace.scaled(0,0);
    }
    double antiLag=trans.m11()+trans.m12()+trans.m13()
            +trans.m21()+trans.m22()+trans.m23()
            +trans.m31()+trans.m32()+trans.m33();
    if(antiLag < -10000 || 10000 < antiLag)
    {
        qDebug("transfo trop grande, retourne null image");
        return imgFace.scaled(0,0);//nullImg
    }

    imgFace=imgFace.transformed(trans);
    ptCentre=QPointF(xMin+imgFace.width()/2, yMin+imgFace.height()/2);
    return imgFace;

}

QImage GraphicScene3D::drawAxes(QImage img)
{
    QPoint3D pt0=calcPoint(QPoint3D(0,0,0));//centre

    QMultiMap<double, int> listeAxes;//liste les axes selont leurs Z
    QList<QPoint3D> listeAxes2;
    QList<QString> listeAxes3;
    QPoint3D temp(calcPoint(QPoint3D(100,0,0)));
    listeAxes.insert(temp.z(), listeAxes2.size());
    listeAxes2.append(temp);
    listeAxes3.append("axeX");
    temp = QPoint3D(calcPoint(QPoint3D(0,100,0)));
    listeAxes.insert(temp.z(), listeAxes2.size());
    listeAxes2.append(temp);
    listeAxes3.append("axeY");
    temp = QPoint3D(calcPoint(QPoint3D(0,0,100)));
    listeAxes.insert(temp.z(), listeAxes2.size());
    listeAxes2.append(temp);
    listeAxes3.append("axeZ");

    QMultiMap<double, int>::iterator i = listeAxes.begin();
    while (i != listeAxes.end())
    {
        img=drawLigne(img, pt0, listeAxes2.at(i.value()), "textures/axes/"+listeAxes3.at(i.value())+".png");
        i++;
    }
    /*//axe X
    QPoint3D pt1=calcPoint(QPoint3D(100,0,0));
    qDebug()<<"axe X"<<pt1.x()<<pt1.y()<<pt1.z();
    img=drawLigne(img, pt0, pt1, "textures/axes/axeX.png");
    //dessiner face "axeX", plus simple (permet - de bugs de pointillés

    //axe Y
    pt1=calcPoint(QPoint3D(0,100,0));//(0,100,0) mais tests
    qDebug()<<"axe Y"<<pt1.x()<<pt1.y()<<pt1.z();
    img=drawLigne(img, pt0, pt1, "textures/axes/axeY.png");

    //axe Z
    pt1=calcPoint(QPoint3D(0,0,100));
    qDebug()<<"axe Z"<<pt1.x()<<pt1.y()<<pt1.z();
    img=drawLigne(img, pt0, pt1, "textures/axes/axeZ.png");
*/
    return img;
}

QImage GraphicScene3D::drawLigne(QImage img, QPoint3D pt0, QPoint3D pt1, QString file)
{
    //longeures des axes
    QPoint3D pt2(pt0);
    pt2.setY(pt2.y()+10);
    double d1=sqrt( pow(pt1.x()-pt0.x(),2) + pow(pt1.y()-pt0.y(),2) );//adjacent
    double d2=sqrt( pow(pt2.x()-pt0.x(),2) + pow(pt2.y()-pt0.y(),2) );//hypoténuse
    //if(pt1.x() < pt0.x()) d1=-d1;
    //if(pt1.y() < pt0.y()) d1=-d1;
    double rotation=acos(d1/d2);//trigo colège, angle de rotation de l'axe
    double prodScalaire=(pt1.x()-pt0.x())*(pt2.x()-pt0.x()) + (pt1.y()-pt0.y())*(pt2.y()-pt0.y());
    rotation=prodScalaire/(d1*d2);
    rotation=((int)(rotation*10000.0))/10000.0;//arrondit
    rotation = acos(rotation);
    if(pt1.x()-pt0.x() > 0)//en supposant que le 2eme vect est tjs vers le haut
        rotation = 2*3.1415-rotation;

    QImage imgAxe(loadImg(file));
    imgAxe=imgAxe.scaled(imgAxe.width()*2, abs(d1));//longueur
    QMatrix matrixR(cos(rotation), sin(rotation), -sin(rotation), cos(rotation), 0, 0);

    img = compileImage(img, (pt0.x()+pt1.x())/2 + img.width()/2,
                       (pt0.y()+pt1.y())/2 + img.height()/2,
                       imgAxe.transformed(matrixR));

    /*double x, y;//déssiner une ligne de pts (bug sur les lignes en y)
    for(double ix=0; ix<=d; ix++)
    {
        for(int ix2=0; ix2<=1; ix2++)
        {
            for(int iy=0; iy<=1; iy++)
            {
                x=ix+ix2+pt0.x() + img.width()/2;
                y=a*(ix+ix2+pt0.x())+b+iy + img.height()/2;
                if(0<=x && x<img.width() && 0<=y && y<img.height())
                {
                    img.setPixelColor(x, y, c);
                }
            }
        }
    }*/
    return img;
}

QImage GraphicScene3D::loadImg(QString fileName)
{
    QImage img;
    if(!img.load(ressourcepacks+ressourcepack+fileName))//charge le fichier
    {
        img.load(ressourcepacks+ressourcepack+"textures/blocks/block_nul.png");
        qDebug()<<"textureTrieur ne trouve pas"<<ressourcepacks+ressourcepack+fileName;
    }
    return img;
}

QImage GraphicScene3D::drawCamera(QImage img)
{
    return compileImage(img, client->x()+img.width()/2,
                     client->y()+img.height()/2,
                     imgCamera);
}
