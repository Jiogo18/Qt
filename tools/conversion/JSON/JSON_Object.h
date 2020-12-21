#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include <QString>
#include <QMap>
#include <QVariant>
#include "debug.h"

class JSON_Object : public QObject
{
public:
    enum Type {
        Object = 0,
        Array = 1,
        Argument = 2,
        String = 3,
        Char = 4,
        Number = 5
    };
    const QStringList TypeString{
        "Object", "Array", "Argument", "String", "Char", "Number"
    };
    JSON_Object();
    JSON_Object(const JSON_Object &obj);
    const JSON_Object &operator =(const JSON_Object &obj);
    JSON_Object *copy() const;
    void setValue(QString key, QString value);
    QString getValue(QString key);
    bool containsValue(QString key);
    bool setType(int type);
    int getType();
    QString getTypeString();
    void setChild(QString name, JSON_Object *obj);
    JSON_Object *getChild(QString name);
    QString getEntress() const;

protected:
    QMap<QString, QString> *pValue;
    int pType;
    QMap<QString, JSON_Object*> *pChild;

};

#endif // JSON_OBJECT_H
