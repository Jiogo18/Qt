#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include "videoWidget.h"
#include <QString>
#include "toastIcon.h"

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = 0);
    ~fenetre();
    void closeEvent(QCloseEvent *event);
    void newFileName();
    void afficher();
    void bAfficher();
    void calcChangementsMouse();
    void calcChangementsPict();
    void newChangement();
    double difference(QImage img1, QImage img2);
    QImage screenshot();
    void actuFileConfig();
    void uploadFileConfig();
    void lblVideoNbChange(quint64 nbVideo);
    void ecrireFichier(QFile *file, QString str);

private:
    void actuTempsMouse();
    void actuTempsScreenshot();

    Ui::fenetre *ui;
    QTimer *tActuPict;
    QTimer *tActu;
    quint64 dernierActu;

    //ressources de changement
    QPoint lastCursor;
    QImage lastCapture;

    videoWidget *viewVideo;

    toastIcon *toastI;//toastIcon


};

#endif // FENETRE_H
