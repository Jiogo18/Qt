#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#include <QTextEdit>
#include <QString>
#include <QTime>
#include <QColor>

class debug : public QObject
{
    Q_OBJECT
public:
    debug(QTextEdit *teOut=nullptr);
    void setOut(QTextEdit *teOut);
    void addLog(QString provenance, QString detail, int lvl=4);
    void setLvlDebug(int lvl=4);//lvl max pris en compte pour qDebug (-1 pour aucun)
    int getLvlDebug();

signals:
    void newLog(QString);
    void newLogHtml(QString);

private:
    QTextEdit *pteOut;
    int pLvlDeb;
};

#endif // DEBUG_H
