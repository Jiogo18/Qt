#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "compilateur.h"

#include <QDebug>

namespace Ui {
class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit fenetre(QWidget *parent = nullptr);
    ~fenetre();
    void onImportClicked();
    void onCompileClicked();
    void onDecompileClicked();
    void onCompilationOver(QString code);
    void onError(QString error);
    void onProgressChanged(double progress);
    QString getIndentationStr() const;
    compilateur::Format getFormat() const;
    bool saveInFile(QString code);

signals:
    void compile(QString, int);
    void decompile(QString, int, QString);
    void compilationError(QString);

private:
    Ui::fenetre *ui;
    compilateur comp;
    QString lastFileName;
};

#endif // FENETRE_H
