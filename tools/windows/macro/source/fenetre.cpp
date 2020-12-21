#include "fenetre.h"

fenetre::fenetre()
{
    setWindowTitle("Macro-Prog");
    setupUi(this);
    actuSelection();
    connect(AddAction, SIGNAL(activated(int)), this, SLOT(actuSelection()));
    connect(AddBoutonSouris, SIGNAL(activated(int)), this, SLOT(actuSelection()));
    connect(AddTouche, SIGNAL(activated(int)), this, SLOT(actuSelection()));
    connect(AddEtat, SIGNAL(activated(int)), this, SLOT(actuSelection()));

    connect(bAjouter, SIGNAL(clicked(bool)), this, SLOT(ajouterLigne()));
    connect(bRetirer, SIGNAL(clicked(bool)), this, SLOT(retirerLigne()));
    fichier = "";
    fichierOK = "";
    fichierValide = false;
    connect(bOuvrir, SIGNAL(clicked(bool)), this, SLOT(OuvrirFichier()));
    connect(bEnregistrerSous, SIGNAL(clicked(bool)), this, SLOT(enregistrerSous()));
    connect(bEnregistrer, SIGNAL(clicked(bool)), this, SLOT(enregistrer()));
    connect(bNouveau, SIGNAL(clicked(bool)), this, SLOT(nouveau()));
    connect(bExecuter, SIGNAL(clicked(bool)), this, SLOT(executer()));//permet d'envoyer un true
    connect(bCancel, SIGNAL(clicked(bool)), this, SLOT(arreter()));

    connect(TexteCode, SIGNAL(textChanged()), this, SLOT(codeUpdate()));
}


void fenetre::actuSelection()
{
    AddLigne->setMaximum(TexteCode->document()->lineCount()+1);
    RetireLigne->setMaximum(TexteCode->document()->lineCount());
    AddTemps->hide();
    AddTempsLbl->hide();
    AddBoutonSouris->hide();
    AddTouche->hide();
    AddEtat->hide();
    txtEcrire->hide();
    AddTourMolette->hide();
    AddTourMoletteLbl->hide();
    AddDeplaceType->hide();
    AddDeplace->hide();
    if(AddAction->currentText() == QString("Attendre"))
    {
        AddTemps->show();
        AddTempsLbl->show();
    }
    else if(AddAction->currentText() == QString("Souris"))
    {
        AddEtat->show();
        AddBoutonSouris->show();
    }
    else if(AddAction->currentText() == QString("Clavier"))
    {
        AddEtat->show();
        AddTouche->show();
    }
    else if(AddAction->currentText() == QString("Ecrire"))
    {
        txtEcrire->show();
    }
    else if(AddAction->currentText() == QString("Molette"))
    {
        AddTourMolette->show();
        AddTourMoletteLbl->show();
    }
    else if(AddAction->currentText() == QString("Déplacer la souris"))
    {
        AddDeplaceType->show();
        AddDeplace->show();
    }
}

void fenetre::ajouterLigne()
{
    QString code = "";
    TexteCode->moveCursor(QTextCursor::Start);
    for(int i=1; i<AddLigne->value(); i++)
        TexteCode->moveCursor(QTextCursor::Down);

    code = "[" + AddAction->currentText() + QString("]");
    if(AddAction->currentText() == QString("Attendre"))
    {
        code += " " + QString::number(AddTemps->value()*1000);
    }
    else if(AddAction->currentText() == QString("Souris"))
    {
        code += " " + AddBoutonSouris->currentText();
        code += " " + AddEtat->currentText();
    }
    else if(AddAction->currentText() == QString("Clavier"))
    {
        code += " " + AddTouche->currentText();
        code += " " + AddEtat->currentText();
    }
    else if(AddAction->currentText() == QString("Ecrire"))
    {
        code += " " + txtEcrire->text();
    }
    else if(AddAction->currentText() == QString("Molette"))
    {
        code += " " + QString::number(AddTourMolette->value()*100);
    }
    else if(AddAction->currentText() == QString("Déplacer la souris"))
    {
        code += " " + AddDeplaceType->currentText();
        code += " " + QString::number(AddDeplace->value());
    }
    else if(AddAction->currentText() == QString("Vide"))
    {}

    code += "\n";
    TexteCode->textCursor().insertText(code);
    actuSelection();
    AddLigne->setValue(AddLigne->value()+1);
    RetireLigne->setValue(AddLigne->value());
}

void fenetre::retirerLigne()
{
    QTextCursor tc = TexteCode->textCursor(); //Fonctionne aussi, normalement, avec un QTextEdit
    tc.movePosition(QTextCursor::Start); //On place le curseur au début
    for(int currentLine = 1; currentLine < RetireLigne->value(); currentLine++)
        tc.movePosition(QTextCursor::Down); //Tant que la ligne courante est inférieur à numLine, descendre le curseur d'une ligne
    tc.select(QTextCursor::LineUnderCursor); //On sélectionne la ligne courante
    tc.removeSelectedText(); //On supprime le texte sélectionné
    tc.deleteChar(); //On supprime le "\n" (qui est encore présent, malgré l'effacement de la ligne
    TexteCode->setTextCursor(tc); //On ré-affecte le QTextCursor

    //RetireLigne->setValue(RetireLigne->value());
    actuSelection();
}


void fenetre::enregistrer()
{
    if(fichierValide)
    {
        QFile file(fichierOK);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << "fichier de sauvegarde macro\n";
        out << TexteCode->toPlainText();
        file.close();

    }
    else
    {
        QMessageBox alerteSave;
        alerteSave.setIcon(QMessageBox::Question);
        alerteSave.setText("Aucun fichier de sauvegarde n'a été défini.");
        alerteSave.setInformativeText("Voulez vous en enregistrer un ?");
        QAbstractButton *SaveButton = alerteSave.addButton("Enregistrer", QMessageBox::ActionRole);
        QAbstractButton *DontSaveButton = alerteSave.addButton("Ne pas enregistrer", QMessageBox::ActionRole);
        alerteSave.exec();
        if(alerteSave.clickedButton() == SaveButton)
        {
              enregistrerSous();
        }
    }
}

void fenetre::nouveau()
{
    fichier = "";
    fichierValide=false;
    fichierOK="";
    TexteCode->clear();
}

void fenetre::enregistrerSous()
{
    fichier = QFileDialog::getSaveFileName(this, "Enregistrer un fichier [macro]", QString(), "Fichier texte (*.txt)");
    if(fichier != QString(""))
    {
        fichierOK = fichier;
        fichierValide = true;
    }
        enregistrer();
}

void fenetre::OuvrirFichier()
{

    fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier [macro]", QString(), "Fichier texte (*.txt)");
    QFile file(fichier);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    if(fichier != QString("") && file.isOpen())
    {
        QTextStream in(&file);

        if(!in.atEnd() && in.readLine() == QString("fichier de sauvegarde macro")) //si la premiere ligne est bonne
        {
            fichierOK = fichier;
            fichierValide = true;
            TexteCode->clear();
            while (!in.atEnd())
            {
                QString line = in.readLine();
                TexteCode->append(line);
            }


        }
        else
        {
            QMessageBox alerteOuverture;
            alerteOuverture.setIcon(QMessageBox::Warning);
            alerteOuverture.setText("Le fichier choisi ne correspond pas à un fichier pour ce programme.");
            alerteOuverture.setInformativeText("Voulez vous en rouvrir un autre ?");
            QAbstractButton *OpenButton = alerteOuverture.addButton("Ouvrir", QMessageBox::ActionRole);
            QAbstractButton *AnnulerButton = alerteOuverture.addButton("Annuler", QMessageBox::ActionRole);
            alerteOuverture.exec();
            if (alerteOuverture.clickedButton() == OpenButton)
            {
                OuvrirFichier();
            }
            else
                return;
        }
        file.close();
        setWindowTitle(QString("Macro-Prog [") + fichier + QString("]"));
    }
    else
        setWindowTitle(QString("Macro-Prog"));
    AddLigne->setMaximum(TexteCode->document()->lineCount());//TexteCode a tjs une ligne de plus
}

void fenetre::executer()
{
    TexteCode->setReadOnly(true);
    bExecuter->setDisabled(true);
    bCancel->setEnabled(true);
    enregistrer();
    if(!testErreur())
    {
        std::cout << "Execution.\n";
        execution=true;
        Pause(tempsExec->value()*1000);
        QSound::play("sons/start.wav");
        repetRestant=repete->value();
        vInt = new QMap<QString, int>;
        vQString = new QMap<QString, QString>;
        touches = new QStringList;
        while((repetRestant>0 || repetRestant==-1) && execution)
        {
            bExecuter->setText("Executer (" + QString::number(repetRestant) + " restants)");

            for(int ligne=1; ligne<=TexteCode->document()->lineCount(); ligne++)
            {
                Action(ligne);
            }
            if(repetRestant>0)
                repetRestant--;

            if((repetRestant==0 || repetRestant==-1) && execution)//si on continu
                QSound::play("sons/restart.wav");
        }
        repete->setSuffix("");
        while(touches->size())
        {
            if(touches->at(0).startsWith("Click-"))
            {
                if(touches->at(0) == QString("Click-Gauche"))
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                if(touches->at(0) == QString("Click-Gauche"))
                    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                if(touches->at(0) == QString("Click-Gauche"))
                    mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            }
            else
            {
                Clavier(touches->at(0), "Relaché");
            }
            qDebug(QString("Touche automatiquement relachee : "+touches->at(0)+"\n").toStdString().c_str());
            touches->removeAt(0);
        }
        txtInfo->setText("Terminé");
        QSound::play("sons/stop.wav");
        std::cout << "Execution terminee.\n";


    }
    bCancel->setDisabled(true);
    bExecuter->setText("Executer");
    bExecuter->setEnabled(true);
    TexteCode->setReadOnly(false);
}

void fenetre::arreter()
{
    execution=false;
}

bool fenetre::testErreur()
{/*
    QList<QString> actions;
    for(int ligne=1; ligne<TexteCode->document()->lineCount(); ligne++)
    {

        while (!in.atEnd())
        {
            QString line = in.readLine();
            TexteCode->append(line);
        }
    }
    if(!actions.isEmpty())
    {
        txtInfo->setText(QString("ERREUR : "));
    }
    return actions.isEmpty();*/
    return false;
}

void fenetre::Action(int ligne)//clavier voir : windows, sift, click, ecrire, molette pas test, vide pas test
{
    QString strLigne = getLigne(ligne);
    txtInfo->setText(QString("[") + QString::number(ligne) + QString("] ") + strLigne);
    QCoreApplication ::processEvents();//update
    while(strLigne.left(1) == QString(" "))//permet d'autoriser l'indentation
    {
        strLigne.remove(0, 1);
    }

    if(strLigne.isEmpty()  ||  strLigne.left(2)==QString("//"))
        return;//si c vide ou commentaire

    QStringList actions = strToList(strLigne);
    if(actions.size()>=3 && actions.at(0)+actions.at(1)+actions.at(2) == QString("Déplacerlasouris"))
    {
        actions.replace(0, "Déplacer la souris");
        actions.removeAt(1);
        actions.removeAt(1);//2
    }

    if(actions.at(0) == QString("[Attendre]"))
    {
        Pause(actions.at(1).toInt());
    }
    else if(actions.at(0) == QString("[Souris]"))
    {
        if(!touches->contains(actions.at(1)) && actions.at(2) == QString("Pressé"))
            touches->append(actions.at(1));
        if(actions.at(2) == QString("Clické") || actions.at(2) == QString("Relaché"))
            touches->removeAll(actions.at(1));
        if(actions.at(1) == QString("Click-Gauche"))
        {
            if(actions.at(2) == QString("Pressé") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            if(actions.at(2) == QString("Relaché") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
        else if(actions.at(1) == QString("Click-Droit"))
        {
            if(actions.at(2) == QString("Pressé") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            if(actions.at(2) == QString("Relaché") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        }
        else if(actions.at(1) == QString("Click-Molette"))
        {
            if(actions.at(2) == QString("Pressé") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            if(actions.at(2) == QString("Relaché") || actions.at(2) == QString("Clické"))
                mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        }

    }
    else if(actions.at(0) == QString("[Clavier]"))
    {
        if(actions.at(2)==QString("(Maj)"))
            actions.removeAt(2);

        QString touche=actions.at(1);
        touche = touche.left(prochainsStr(actions.at(1), "(")-1);//-1 pour l'espace avant (   -> pourles 1(&)...
        if(!touches->contains(touche) && actions.at(2) == QString("Pressé"))
            touches->append(touche);
        if(actions.at(2) == QString("Clické") || actions.at(2) == QString("Relaché"))
            touches->removeAll(touche);
        Clavier(touche, actions.at(2));
    }
    else if(actions.at(0) == QString("[Ecrire]"))
    {
        while(actions.size() > 2)// limite pour les espaces
        {
            actions.replace(1, actions.at(1)+" "+actions.at(2));
            actions.removeAt(2);
        }
        for(int i=0; i<actions.at(1).size(); i++)
        {
            QChar lettre = actions.at(1).at(i);
            if(QString(">?./§¨£%µ°+ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890").contains(lettre))
                Clavier("Shift", "Pressé");
            if(QString("~#{[|`\\@]}").contains(lettre))
                Clavier("Alt-Gr", "Pressé");

            Clavier(lettre, "Clické");

            if(QString(">?./§¨£%µ°+ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890").contains(lettre))
                Clavier("Shift", "Relaché");
            if(QString("~#{[|`\\@]}").contains(lettre))
                Clavier("Alt-Gr", "Relaché");
        }

    }
    else if(actions.at(0) == QString("[Molette]"))
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, DWORD(actions.at(2).toInt()), 0);

    }
    else if(actions.size()>=5 && actions.at(0)+" "+actions.at(1)+" "+actions.at(2) == QString("[Déplacer la souris]"))
    {
        actions.replace(0, actions.at(0)+" "+actions.at(1)+" "+actions.at(2));
        actions.removeAt(2);
        actions.removeAt(1);
        if(actions.at(1) == QString("X="))
        {
            SetCursorPos(actions.at(2).toInt(), cursor().pos().y());
        }
        else if(actions.at(1) == QString("Y="))
        {
            SetCursorPos(cursor().pos().x(), actions.at(2).toInt());
        }
        else if(actions.at(1) == QString("X~="))
        {
            SetCursorPos(cursor().pos().x() + actions.at(2).toInt(), cursor().pos().y());
        }
        else if(actions.at(1) == QString("Y~="))
        {
            SetCursorPos(cursor().pos().x(), cursor().pos().y() + actions.at(2).toInt());
        }
    }
    else if(actions.at(0) == QString("[Var]"))
    {
        //if()
        qDebug("erreur code, partie non presente");
    }
    else if(actions.at(0) == QString("[Vide]"))
    {}
}





void fenetre::Clavier(QString touche, QString etat)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = strToByte(touche);
    ip.ki.wScan = MapVirtualKeyEx(strToByte(touche), 0, (HKL)0xf0010413);;

    if(etat == QString("Pressé") || etat == QString("Clické"))
    {
        ip.ki.dwFlags = 0; //key press
        SendInput(1, &ip, sizeof(INPUT));
    }
    if(etat == QString("Relaché") || etat == QString("Clické"))
    {
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
    }
}


QString fenetre::getLigne(int ligne)
{
    QString texte = TexteCode->toPlainText();
    for(int i=1; i<ligne; i++)
    {
        int prochaineRetour=prochainsStr(texte, "\n");
        texte = texte.right(texte.size()-prochaineRetour+1-2);
    }
    return texte.left(prochainsStr(texte, "\n"));
}


int fenetre::prochainsStr(QString texte, QString str)
{
    for(int i=0; i+str.length()-1<texte.size(); i++)
    {
        if(texte.mid(i, str.size()) == str)
        {
            return i;
        }
    }
    return -1;
}

QStringList fenetre::strToList(QString texte)
{
    QStringList liste;
    while(!texte.isEmpty())
    {
        liste.append(texte.left(prochainsStr(texte, " ")));
        texte = texte.right(texte.size()-liste.last().size());
        if(texte.left(1) == QString(" "))
            texte = texte.remove(0, 1);
        if(!texte.contains(" "))
        {
            liste.append(texte);
            texte.clear();
        }
    }
    return liste;
}

void fenetre::Pause(int temp)
{
    for(int i=0; i<temp-temp%50; i+=50)//20 updates/sec
    {
        if(execution==false)
            return;
        Sleep(50);
        QCoreApplication::processEvents();
    }
    Sleep(temp%50);
}

int fenetre::strToByte(QString str)
{
    if(str == QString("Anuller"))
    {
        return VK_BACK;
    }
    else if(str == QString("Tab"))
    {
        return VK_TAB;
    }
    else if(str == QString("Entrée"))
    {
        return VK_RETURN;
    }
    else if(str == QString("Shift"))
    {
        return VK_RSHIFT;
    }
    else if(str == QString("Ctrl"))
    {
        return VK_LCONTROL;
    }
    else if(str == QString("Alt"))
    {
        return VK_LMENU;
    }
    else if(str == QString("Alt-Gr"))
    {
        return VK_RMENU;
    }
    else if(str == QString("Echap"))
    {
        return VK_ESCAPE;
    }
    else if(str == QString("Espace") || str == QString(" "))
    {
        return VK_SPACE;
    }
    else if(str == QString("Début"))
    {
        return VK_HOME;
    }
    else if(str == QString("Fin"))
    {
        return Qt::Key_End;
    }
    else if(str == QString("Flèche_Gauche"))
    {
        return VK_LEFT;
    }
    else if(str == QString("FlSSSSSSèche_Haut"))
    {
        return VK_UP;
    }
    else if(str == QString("Flèche_Droite"))
    {
        return VK_RIGHT;
    }
    else if(str == QString("Flèche_Bas"))
    {
        return VK_DOWN;
    }
    else if(str == QString("Imprim_écran"))
    {
        return VK_PRINT;
    }
    else if(str == QString("Suppr"))
    {
        return VK_DELETE;
    }
    else if(str == QString("Windows"))
    {
        return VK_LWIN;
    }
    else
    {
        QStringList liste = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "^", "$", "ù", ",", ";", ":", "!", ")", "=", "<", " ", "&", "é", "\"", "'", "(", "-", "è", "_", "ç", "à", "~", "#", "{", "[", "|", "`", "*", "\\", "@", "+", "}", ">", "?", ".", "/", "§", "%", "µ", "¨", "£", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "]"};
        if(liste.contains(str))
        {
            QChar str2 = str[0].toLatin1();
            char str3 = str2.toLatin1();
            return VkKeyScan(str3);//115 == s (ascii)
        }
        else if(str == QString(""))
        {
            return 0;
        }
        else
            return -1;
    }
}


void fenetre::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        toucheArret[0] = true;
    if(event->key() == Qt::Key_Alt)
        toucheArret[1] = true;
    if(event->key() == Qt::Key_ParenRight)//parenthèse droite "("
        toucheArret[2] = true;
    if(toucheArret[0] && toucheArret[1] && toucheArret[2])
        execution=false;
}
void fenetre::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        toucheArret[0] = false;
    if(event->key() == Qt::Key_Alt)
        toucheArret[1] = false;
    if(event->key() == Qt::Key_ParenRight)
        toucheArret[2] = false;
}

void fenetre::codeUpdate()
{
    if(AddLigne->value() > TexteCode->document()->lineCount())
    {
        AddLigne->setValue(TexteCode->document()->lineCount());//le text code a tjs une ligne vide en bas
        AddLigne->setMaximum(TexteCode->document()->lineCount());
        RetireLigne->setMaximum(AddLigne->maximum()-1);
    }
}
