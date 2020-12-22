#include "fenetre.h"

fenetre::fenetre()
{
    //set fenetre
    {
    scene = new QGraphicsScene;
    view = new QGraphicsView(scene);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);
    setMinimumSize(854, 480);
    setMaximumSize(minimumSize());
    }
    ressourcepack = "defaut";
    boutons = new QList<GraphicsButton*>;
    inGame=false;
    optionOpen=false;
    etat=0;
    version=0.1;
    openMenu();

}

void fenetre::newRessourcepack(QString nressourcepack)
{
    ressourcepack = nressourcepack;
    emit actuRessourcepack(ressourcepack);
}

void fenetre::removeButtons()
{
    for(int i=0; i<boutons->size(); i++)
    {
        scene->removeItem(boutons->at(i));
        boutons->at(i)->deleteLater();
    }
    if(!boutons->empty())
        boutons->clear();
    scene->clear();
    scene->setStickyFocus(true);
    view->setSceneRect(0, 0, 830, 456);
}

void fenetre::setBackground(QString limage)
{
    QPixmap background(":/ressourcepacks/" + ressourcepack + "/QMC/textures/gui/" + limage + ".png");
    if(!background)
    {
        qDebug("%s", ("No pixmap "+limage+" in "+ressourcepack).toStdString().c_str());
        background.load(":/ressourcepacks/defaut/QMC/textures/gui/" + limage + ".png");
    }
    scene->setBackgroundBrush(QBrush(background));
}





void fenetre::buttonClicked(QString action)
{
    if(action == "quit")
    {
        close();
    }
    else if(action == "openMenu")
    {
        openMenu();
    }
    else if(action == "openSolo")
    {
        openSolo();
    }
    else if(action == "openMulti")
    {
        openMulti();
    }
    else if(action == "openOption")
    {
        openOption();
    }
    else
    {
        if(optionOpen)
        {
            qDebug(("Bouton : " + action + " (Option) n'a pas d'action.").toStdString().c_str());
        }
        else
            qDebug(("Bouton : " + action + " (Menu) n'a pas d'action.").toStdString().c_str());
    }

}


void fenetre::openMenu()
{
    setBackground("menu");
    removeButtons();
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-500/2, 150, 500, 40), "Solo", "openSolo"));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-500/2, 200,  500, 40), "Multi", "openMulti", false));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-500/2, 300, 200, 40), "Option", "openOption"));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2+500/2-200, 300, 200, 40), "Quitter", "quit"));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-500/2, 150, 500, 40), "Solo", "openSolo"));
    for(int i=0; i<boutons->size(); i++)
        scene->addItem(boutons->at(i));

    QPainterPath path;
    QFont font = scene->font();
    font.setPointSize(15);
    path.addText(1, 456-5, font,  "QMC " + QString::number(version));
    scene->addPath(path,    QPen(QBrush(Qt::black), 1),     QBrush(Qt::darkGray)      );

    inGame=false;
    etat=1;
}

void fenetre::openSolo()
{
    setBackground("options_background");
    removeButtons();
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-200-50, 350, 200, 40), "Charger", "loadSave", false));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2-200-50, 400, 200, 40), "Suprimer", "loadSave", false));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2+50, 350, 200, 40), "Nouveau monde", "newSave"));
    boutons->append(new GraphicsButton(this, ressourcepack, QRectF(830/2+50, 400, 200, 40), "Retour", "openMenu"));
    for(int i=0; i<boutons->size(); i++)
        scene->addItem(boutons->at(i));

    QDir dir("saves");
    QStringList saves=dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i=0; i<saves.size(); i++)
    {
        QFile settings("saves/"+saves.at(i)+"/settings.txt");
        if (!settings.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            saves.removeAt(i);
            i--;
        }
        QTextStream in(&settings);
        if(in.readLine() != "###QMC saves###")
        {
            saves.removeAt(i);
            i--;
        }
    }
    qDebug((QString::number(saves.size()) + " saves : ").toStdString().c_str());
    for(int i=0; i<saves.size(); i++)
        qDebug((QString::number(i+1) + " : " + saves.at(i)).toStdString().c_str());

    inGame=false;
    etat=2;
}

void fenetre::openMulti()
{
    inGame=false;
    etat=3;
}

void fenetre::openOption()
{
    optionOpen=true;
    etat=1;
}





