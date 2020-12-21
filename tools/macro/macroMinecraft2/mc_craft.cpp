#include "mc_craft.h"

mc_craft::mc_craft(mc_gui *MCGuiP, QString fenetre)
{
    MCGui = MCGuiP;
    MCApp = fenetre;
}
bool mc_craft::canCraft()
{
    if(image::findInfoApp(MCApp)->dwWindowStatus != 1) {
        debug("impossible de crafter : minecraft n'est pas au premier plan");
        return false;
    }
    if(MCGui == nullptr) {
        debug("impossible de crafter : erreur avec mc_gui");
        return false;
    }
    if(MCGui->getCurrentGui() != "crafting_table") {//on devrai meme reprendre un screenshot
        debug("impossible de crafter : vous n'etes pas dans une table de craft");
        return false;
    }
    return true;
}


void mc_craft::craftDoor(int nb)
{
    if(!canCraft())//si on peut pas craft
        return;
    QMap<int, QString> itemInv = MCGui->getCurrentItem();
    if(itemInv.isEmpty())//pas dans un gui
        return;
    //QMap<int, int> itemCount;//peut pas le faire encore, il faut dév le truc des polices

    Sleep(1000);


    for(int i=0; i<nb; i++) {
        setOneItem(9, 37);
        setOneItem(9, 38);
        setOneItem(9, 40);
        setOneItem(9, 41);
        setOneItem(9, 43);
        setOneItem(9, 44);
        echangeItem(45, 0, true);
    }
}


void mc_craft::clickCase(int caseCible, int click)
{
    goToCase(caseCible);
    switch (click) {
        case 0:
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        case 1:
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            break;
    }
    Sleep(10);//sécurité
}




void mc_craft::echangeItem(int case1, int case2, bool drop)
{
    clickCase(case1, 0);
    clickCase(case2, 0);
    if(drop) {
        clickCase(-1, 0);
    }
    else {
        clickCase(case1, 0);
    }
}

void mc_craft::setOneItem(int case1, int case2)
{
    clickCase(case1, 0);
    clickCase(case2, 1);
    clickCase(case1, 0);
}

void mc_craft::goToCase(int caseCible)
{
    QPoint pt = MCGui->posCase(caseCible, MCApp).center();
    int i=0;
    SetCursorPos(pt.x(), pt.y());
    Sleep(10);//14 passe mais comme on att après que ça soit bon, ça va
    //10 à test, mc est trop long ou c'est windows qui oblige à mettre +14?
    while(QCursor::pos() != pt) {//tant qu'on est pas au bon endroit en décale
        SetCursorPos(pt.x(), pt.y());
        Sleep(1);//risque pas
        i++;
        if(i>=100)//si on a attendue trop, on arrete
            return;
    }
}
