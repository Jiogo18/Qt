#include "imageTool.h"

QImage imageTool::tookScreenshot()
{
    return QGuiApplication::primaryScreen()->grabWindow(0).toImage();
}

double imageTool::difference(const QImage &img1, const QImage &img2)
{
    if(img1.isNull() || img2.isNull())
        return 1.0;
    int nbPixsDiff=0;
    int width=img1.width();
    if(img2.width()<width)
        width=img2.width();
    int height=img1.height();
    if(img2.height()<height)
        height=img2.height();//on garde la plus petite size
    for(int ix=0; ix<width; ix++)
        for(int iy=0; iy<height; iy++)
            if(img1.pixelColor(ix,iy)!=img2.pixelColor(ix,iy))
                nbPixsDiff++;
    return (double)nbPixsDiff / (img2.width()*img2.height());
}

bool imageTool::nullImage(const QImage &img)
{
    if(img.isNull())
        return true;
    for(int x=0; x<img.width(); x++)
        for(int y=0; y<img.height(); y++)
            if(img.pixel(x, y) != Qt::black)
                return false;
    return true;
}
