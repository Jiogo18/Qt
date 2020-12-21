#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include "JSON.h"
#include <QFileDialog>
#include "debug.h"

class fenetre : public QWidget
{
    Q_OBJECT
public:
    fenetre();
    ~fenetre();
    void sLoadOpen();
    void sLoad();
    void sUploadOpen();
    void sUpload();
    void sCmd();
    //lvl: 0=critical (arret), 1=erreur, 2=warning, 3=info, autre : inconnue

private:
    //de quoi selectionner un fichier (line, + 2 buttons) => load & parcourir
    QLineEdit *lLoad;
    QPushButton *bLoadOpen;
    QPushButton *bLoad;
    //de quoi creer un nouveau fichier (line, + 2 buttons) => upload & parcourir
    QLineEdit *lUpload;
    QPushButton *bUploadOpen;
    QPushButton *bUpload;
    //de quoi lire l'objet (text edit coloré?) (non modifiable)
    //logs
    //ligne de commande + bouton pour modifier  les propriétées
    QTextEdit *pteObject;
    QTextEdit *pteLog;
    QLineEdit *lCmd;
    QPushButton *bCmd;

    enum LogLevel {
        Critical = 0,
        Error = 1,
        Warning = 2,
        Info = 3,
        Debug = 4,
        Commentaire = 5
    };
    JSON *json;
    debug *pdeb;
};

#endif // FENETRE_H
