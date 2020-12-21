#include "debug.h"

debug::debug(QTextEdit *teOut) : QObject() {pteOut = teOut; pLvlDeb=4;}

debug::debug(QString msg) : QObject()
{
    qDebug("%s", QString(
               "[" + QTime::currentTime().toString("hh:mm:ss") + "] " + msg
               ).toStdString().c_str());
}

void debug::setOut(QTextEdit *teOut) {pteOut = teOut;}

void debug::addLog(QString provenance, QString detail, int lvl)
{
    QColor color = "#000000";
    QColor bgColor = "#FFFFFF";
    bool bold = false;
    switch (lvl) {
        case 0:
            color = "#DE4F4F";//rouge
            bgColor = "#000000";//noir
            bold = true;
            provenance += "/ERROR";
            break;
        case 1:
            color = "#DE4F4F";//222, 79, 79 (rouge)
            provenance += "/ERROR";
            break;
        case 2:
            color = "#D6AB1C";//236, 188, 29 (jaune) ECBC1D
            provenance += "/WARN";
            break;
        case 3:
            color = "#000000";//
            provenance += "/INFO";
            break;
        case 4:
            color = "#0000AA";//bleu 0000AA
            bold=true;
            provenance += "/DEBUG";
            break;
        default://commentaire
            color = "#C0C0C0";//gris claire
            provenance += "/INFO";
            break;
    }

    QString line = QTime::currentTime().toString("[HH:mm:ss]")+" ["+provenance+"] : "+detail;
    QString lineHtml = line;
    if(bold)
        lineHtml = "<b>"+lineHtml+"</b>";
    lineHtml = "<p style=\"margin: 0px 0px 0px 0px\"><mark style=\"background-color:"+bgColor.name()+";\"><font color="+color.name()+">"+lineHtml+"</font></mark></p>";
    if(pteOut != nullptr)
        //pteOut->insertHtml(lineHtml);//c'est pas ça (insere la ou est le curseur)
        pteOut->append(lineHtml);
    emit newLog(line);
    emit newLogHtml(lineHtml);

    if(pLvlDeb < lvl)
        return;
    switch (lvl) {
        case 0:
            qCritical("%s", line.toStdString().c_str());
            //qFatal("%s", line.toStdString().c_str());//log + quitter le prog
            break;
        case 1:
            qCritical("%s", line.toStdString().c_str());
            break;
        case 2:
            qWarning("%s", line.toStdString().c_str());
            break;
        case 3:
            qInfo("%s", line.toStdString().c_str());
            break;
        case 4:
            qDebug("%s", line.toStdString().c_str());
            break;
        default:
            qInfo("%s", line.toStdString().c_str());
            break;
    }
}

void debug::setLvlDebug(int lvl) {pLvlDeb=lvl;}
int debug::getLvlDebug() {return pLvlDeb;}
