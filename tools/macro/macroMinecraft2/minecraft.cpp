#include "minecraft.h"

minecraft::minecraft(QString strMCApp, QString strMCFolder, debug* debP) : QObject()
{
    MCApp = strMCApp;
    deb=debP;
    //emit newIcon("world"); marche mais pas dans le constructeur
    imgs.insert("current_screenshot", new QImage());
    MCGui = new mc_gui(imgs.value("current_screenshot"), deb);

    setMCFolder(strMCFolder);
    tTick.setInterval(10000);
    connect(&tTick, &QTimer::timeout, this, &minecraft::tickOperation);
}
minecraft::~minecraft()
{
    QMapIterator<QString, QImage*> i(imgs);
    while(i.hasNext()) {
        i.next();
        if(i.value() != nullptr)
            delete i.value();
    }
}



void minecraft::start()
{
    deb->addLog("minecraft", "Macro Minecraft lancé", 3);
    tTick.start();
}
void minecraft::stop()
{
    tTick.stop();
    deb->addLog("minecraft", "Macro Minecraft arreté", 3);
    reset();
}
void minecraft::tickOperation()
{
    QImage *img = shootCurrent();
    if(img == nullptr)
        return;

    QString nameImg="screenshot_"+QDateTime::currentDateTime().toString("yyMMdd-HHmmss");
    //imgs.insert(nameImg, img);//pour enregistrement


    //coeur du system
    //if(MCGui->getCurrentGui() != "")
        //deb->addLog("minecraft", MCGui->getCurrentGui()+" a été détecté dans \""+nameImg+"\"", 3);
    if(MCGui->getCurrentGui() == "inventory" || MCGui->getCurrentGui() == "creative_inventory")
    {
        emit newIcon("player");
    }
    else if(MCGui->getCurrentGui() == "crafting_table")
    {
        emit newIcon("crafting_table");
    }
    else if(MCGui->getCurrentGui() == "chest" || MCGui->getCurrentGui() == "chest_big")
    {
        emit newIcon("chest");
    }
    else
        emit newIcon("world");

}

QImage *minecraft::shootCurrent()
{
    QImage *img = image::shootScreen(MCApp);
    if(img == nullptr) {//on peut pas l'enregistrer ça a pas marché
        *(imgs.operator[]("current_screenshot")) = QImage();//par contre on enregistre "rien" ici
    }
    else
        *(imgs.operator[]("current_screenshot")) = *img;//change le contenue du pointeur
    MCGui->actuCurrentGui();
    return img;
}
void minecraft::startOne()
{
    Sleep(1000);//laisse le temps de passer à la fenetre
    tickOperation();
    reset();
}
void minecraft::reset()
{
    //on parcours imgs pour enregistrer "screenshot_" et "test_" et pour les retirer de la liste
    QMap<QString, QImage*> imgsTmp = imgs;

    QMapIterator<QString, QImage*> i(imgs);
    int nbScreenshot=0, nbTest=0;
    while(i.hasNext()) {
        i.next();
        if(i.key().startsWith("screenshot_")) {
            image::save(i.value(), i.key(), MCFolder+"\\screenshot");
            nbScreenshot++;
            imgsTmp.remove(i.key());//on retire les images screenshot
        }
        else if (i.key().startsWith("test")) {
            image::save(i.value(), i.key(), MCFolder+"\\test");
            nbTest++;
            imgsTmp.remove(i.key());//on retire les images test
        }
    }
    if(nbScreenshot != 0)
        deb->addLog("minecraft", QString::number(nbScreenshot)+" screenshot enregistrés", 3);
    if(nbTest != 0)
        deb->addLog("minecraft", QString::number(nbTest)+" test enregistrés", 3);

    imgs = imgsTmp;//on applique les changements (retire les images enregistrées)

}

void minecraft::craftDoor()
{
    Sleep(1000);
    shootCurrent();
    mc_craft craft(MCGui, MCApp);
    craft.craftDoor(10);
}






void minecraft::setMCApp(QString name)
{
    MCApp = name;
    deb->addLog("minecraft", "Nouveau nom d'application cible : \""+MCApp+"\"", 3);
}

void minecraft::setActuInterval(int tps)
{
    if(tps < 200)//max de speed
        tps = 200;
    tTick.setInterval(tps);
}
void minecraft::setMCFolder(QString folder)
{
    if(MCFolder == folder)
        return;
    MCFolder = folder;
    deb->addLog("minecraft", "Dossier de données mis à \""+folder+"\"", 3);
    MCGui->setMCFolder(MCFolder);
}


