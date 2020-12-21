#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QCursor>
#include <QtWidgets>

class Screenshot
{
    public:
        Screenshot();
        static void debug(QString msg);
        static QImage shootScreen(int x, int y, int W, int H, int fenetre=0);
        static QImage Item(int x, int y, int W, int H, int fenetre=0);
        static QImage InfoBulle(int fenetre=0);
        static QImage recadrerInfoBulle(QImage img);
        static QImage colorerInfoBulle(QImage img);
        static bool reconnaissance(QImage imgBase, QImage imgCherche, int xMin, int xMax, int yMin, int yMax, QString erreur, QPoint *pos=new QPoint(0, 0));
        static void save(QImage img, QString name, QString folder);

};

#endif // SCREENSHOT_H
