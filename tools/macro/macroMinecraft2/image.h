#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include "debug.h"

#include <QtGui>//pour le FindWindow


class image
{
public:
    image();
    static QImage *shootScreen(QString fenetre="");
    static QImage *safeScreen(QString fenetre);
    /*static QImage Item(int x, int y, int W, int H, int fenetre=0);
    static QImage InfoBulle(int fenetre=0);
    static QImage recadrerInfoBulle(QImage img);
    static QImage colorerInfoBulle(QImage img);
    static bool reconnaissance(QImage imgBase, QImage imgCherche, int xMin, int xMax, int yMin, int yMax, QString erreur, QPoint *pos=new QPoint(0, 0));
    */
    static void save(const QImage *img, QString name, QString folder);
    static QImage *load(QString folder, QString name);
    static QImage *convertToGoodFormat(const QImage *img);
    static int pixelsSim(const QImage *img1, const QImage *img2, int marge=0);
    static double ressemblance(const QImage *img1, const QImage *img2);
    static double ressemblanceContainsMid(const QImage *img1, const QImage *img2);//img2 est plus petite et est au milieu
    static QImage *cutContainsMid(const QImage *img1, const QImage *img2);
    //static bool containsMid(const QImage *img1, const QImage *img2);
    static QPoint getMidPoint(const QImage *img1, const QImage *img2);
    static QSize getMinSize(QSize size1, QSize size2);
    static int getNbPixsMin(const QImage *img1, const QImage *img2);
    static QImage *relativeScaled(const QImage *img, double size);
    static QString findBestImageContains(const QImage *img, const QMap<QString, QImage *> *listeImgs, double min);
    static QString findBestImageContains(const QImage *img, const QMap<QString, const QImage *> *listeImgs, double min);
    static QString findBestImage(const QImage *img, const QMap<QString, const QImage *> *listeImgs, double min);
    static HWND findApp(QString name);
    static PWINDOWINFO findInfoApp(QString name);
    static QPoint getLeftTopFen(QString name);
    static QImage *negaImgs(const QImage *img1, const QImage *img2);//retourne les pixels qui ne sont pas similaires
    static bool isTransparent(const QImage *img);
    static int pixTransparent(const QImage *img);
};

#endif // IMAGE_H
