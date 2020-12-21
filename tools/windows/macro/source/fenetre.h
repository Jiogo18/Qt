#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include "ui_fenetre.h"
#include <QFile>
#include <windows.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSound>
#include <QKeyEvent>
#include <QClipboard>
#include <QMap>

#include <iostream>


class fenetre : public QWidget, private Ui::fenetre
{
    Q_OBJECT

public:
    fenetre();
    bool testErreur();
    void Action(int ligne);
    void Clavier(QString touche, QString etat);
    QString getLigne(int ligne);
    int prochainsStr(QString texte, QString str);
    QStringList strToList(QString texte);
    void Pause(int temp);
    int strToByte(QString str);


private slots:
    void executer();
    void arreter();
    void actuSelection();
    void ajouterLigne();
    void retirerLigne();
    void enregistrerSous();
    void OuvrirFichier();
    void enregistrer();
    void nouveau();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void codeUpdate();


private:
    QString fichier;
    QString fichierOK;
    bool fichierValide;
    bool execution;
    bool toucheArret[3];
    int repetRestant;
    QMap<QString, int> *vInt;
    QMap<QString, QString> *vQString;
    QStringList *touches;


};

#endif // FENETRE_H
