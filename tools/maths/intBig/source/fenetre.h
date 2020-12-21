#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QMap>
#include "Console.h"

namespace Ui { class fenetre; }

class fenetre : public QWidget
{
    Q_OBJECT
public:
    explicit fenetre(QWidget *parent = 0);
    fenetre(const fenetre &fen);
    ~fenetre();
    void closeEvent(QEvent *event);
    void calcAll();
    void calcOnly();
    void calc(int nb);
    void debug(QString str, bool important=false);
    void addStat(QString type, quint64 start, quint64 stop);
    void finCalc();
    void ouvrirFichier(int i);//i est la méthode de calcul
    void fermerFichier();
    void parcourir();
    void ecrireStats();
    void calcInfo1();

    void calcConsole();

private:
    Ui::fenetre *ui;
    bool calcOn;
    QFile *fSave;
    QFile *fStats;
    QMap<QString, QList<quint64>> mapStats;//temps total, nb d'appel, temps min, temps max
    Console console;
};

#endif // FENETRE_H
