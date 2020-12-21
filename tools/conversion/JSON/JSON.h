#ifndef JSON_H
#define JSON_H

#include "JSON_Object.h"
#include <QString>
#include <QFile>
#include "debug.h"

class JSON
{
public:
    JSON(debug *deb);
    void loadFile(QString fileName);
    QString loadCode(QString fileName);
    void saveInFile(QString fileName);
    void execCmd(QString cmd);
    JSON_Object *StrToJson(QString str);
    QString JsonToStr(JSON_Object *obj);
    JSON_Object::Type typeOfValue(QString value);
    QString removeUseless(QString str);

    const QString fileFormat = "*json";
    const QStringList block = {
        "{", "}",
        "[", "]",
        "(", ")",
        "\"", "\"",
        "'", "'"
    };//tout les 2 avec début et fin (permet des blocks avec plusieurs charactères d'in/out)

private:
    QString getBlock(QString in, int iStart);
    int endOfBlock(QString in, int iStart);
    int endOfElement(QString in, int iStart);
    int nextChar(QString in, int i);

    JSON_Object *objJson;
    debug *pdeb;
};

#endif // JSON_H
