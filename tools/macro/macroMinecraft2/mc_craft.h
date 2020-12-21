#ifndef MC_CRAFT_H
#define MC_CRAFT_H


#include "image.h"
#include "mc_gui.h"

class mc_craft
{
public:
    mc_craft(mc_gui *MCGuiP, QString fenetre);
    bool canCraft();
    void craftDoor(int nb);
    void clickCase(int caseCible, int click=0);
    void echangeItem(int case1, int case2, bool drop=false);
    void setOneItem(int case1, int case2);
    void goToCase(int caseCible);


private:
    mc_gui *MCGui;
    QString MCApp;
};

#endif // MC_CRAFT_H
