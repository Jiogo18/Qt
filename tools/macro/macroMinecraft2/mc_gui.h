#ifndef MC_GUI_H
#define MC_GUI_H

#include "image.h"
#include "debug.h"
#include <QDir>

class mc_gui : public QObject
{
public:
    mc_gui(const QImage *currentScreenshotP, debug *debP = nullptr);
    void setMCFolder(QString folderGui);//folder contenant /data/gui, /data/items, /screenshot, /test
    void setCurrentScreenshot(const QImage *screenshot);//inutile si on a précisé une image pointeur dans le constructeur
                                                        //actualise automatiquement le gui

    QRect rectWithSize(QRect rect) const;//retourne un rectangle avec une taille et une pos *MCSize
    QSize getTailleCase() const;//retourne une QSize de la taille des cases avec MCSize
    QMap<int, QString> getCurrentItem();//retourne la liste d'items actuelle
    QMap<int, int> getCurrentItemSize();//retourne la liste de nombre d'items
    QPoint getPosGuiWindow(QString MCApp) const;//récupère la position du gui+fenetre donnée
    QRect posCase(int caseCible, QString MCApp="") const;//retourne le rect de la case (nom de l'app si position selon windows)


    QString actuCurrentGui();//actualise le nom du gui selon le screenshot (actualise aussi MCSize)
    const QImage *getGui(QString name, bool withCase=true, int size=0) const;/*retourne le gui name si il est valide ("x_gui")
                                                           ou size_name si size précisé
                                                           ou MCSize_name*/
    QString getCurrentGui() const;//retourne le gui déterminé par actuCurrentGui()
    QRect getBorderPos() const;//retourne la géométrie du gui avec les 4 bordures


    const QList<QString> GuiList = {//liste des gui dispo
        "inventory",
        "tab_inventory",
        "crafting_table",
        "chest",
        "chest_big",
    };
    const int GuiMaxSize=4;
    const int tailleCase = 18;//compte aussi la bordure
    QMap<QString, QMap<int, QPoint>> GuiCase;//complété dans reloadGuiCase();

private:
    void reloadGui();
    //void reloadGuiCase();//chargement des cases (automatiquement)
    QPoint findBorder(int border, int size) const;
    QPoint findBorder(QImage *imgBorder, int xP, int yP) const;
    void actuBorderSize();
    void setMCSize(int size);
    int getCaseAt(QString gui, int x, int y) const;

    void reloadItems();
    QString findItem(const QImage *caseScreen) const;//on cherche l'item qui correspond à la case
    void actuItem();//actualise la liste des items de l'inventaire

    void reloadFont();
    QString findText(const QImage *img, bool LeftToRight=true);
    QString findChar(const QImage *img, bool Left=true);
    QString MCFolder;
    int MCSize=1;
    debug *deb;
    QMap<QString, const QImage *> imgsGui;//la liste ne sort pas d'ici
    QMap<QString, const QImage*> imgsItem;
    QMap<QString, const QImage*> imgsFont;
    QMap<QString, QString> fontChar;
    const QImage *currentScreenshot;
    QString currentGui;
    QRect currentRectGui;
    QImage *currentScreenGui;
    QMap<int, QString> currentItems;
    QMap<int, int> currentItemsNb;
};

#endif // MC_GUI_H
