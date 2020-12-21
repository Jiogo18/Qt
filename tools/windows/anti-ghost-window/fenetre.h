#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>


class fenetre : public QWidget
{
    Q_OBJECT
public:
    fenetre(bool simple, QWidget *parent=nullptr);
    ~fenetre();
    void addLogWindow(QString log);

private:
    QLabel *lblGhost = nullptr;
    QLabel *lblCurrentWindow = nullptr;
    QTextEdit *txtWindow = nullptr;
    QLineEdit *leCurrentWindow = nullptr;
};

#endif // FENETRE_H
