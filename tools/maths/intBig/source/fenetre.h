#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QMap>

namespace Ui {
class fenetre;
}

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

private:
    Ui::fenetre *ui;
    bool calcOn;
    QFile *fSave;
    QMap<QString, QList<quint64>> mapStats;//temps total, nb d'appel, temps min, temps max
};

#endif // FENETRE_H