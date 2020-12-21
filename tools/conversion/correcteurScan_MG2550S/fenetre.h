#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QFileDialog>
//#include <QDebug>
#include <QImage>//it include QColor and QRgb
#include <QtMath>//floor
#include <QCloseEvent>
#include <QLabel>
#include <QMimeData>//drop event

#include <QDebug>


namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = nullptr);
    ~fenetre();
    void closeEvent(QCloseEvent *event);

    void bParcourirPress();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void loadImg();
    void showImg();

    void bCorrigerPress();
    static QString getUniqueFile(QString fileName, QString extension);

    void corrigeImg();
    QImage detectCorruption(QImage img);
    //QColor corrigePixel(QColor color, QColor dsource, QColor lsource, int ix, int iy, QColor colorMoyenne);
    QColor corrigePixel(QColor color, int ix, QColor colorMoyenne);

    static QColor moyenneColor(QColor a, QColor b) { return QColor((a.red()+b.red())/2, (a.green()+b.green())/2, (a.blue()+b.blue())/2); }



private:
    Ui::fenetre *ui;
    QImage imgIn;
    QImage imgOut;
    /*QMap<int, QMap<int, int>> listePixelRed;
    QMap<int, QMap<int, int>> listePixelGreen;
    QMap<int, QMap<int, int>> listePixelBlue;*/
    QColor lastColor;
    QLabel *lblImg;
};

#endif // FENETRE_H
