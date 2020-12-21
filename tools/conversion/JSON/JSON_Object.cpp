#include "JSON_Object.h"

JSON_Object::JSON_Object() : QObject()
{
    pValue = new QMap<QString, QString>;
    pType = JSON_Object::Object;
    pChild = new QMap<QString, JSON_Object*>;
}
JSON_Object::JSON_Object(const JSON_Object &obj) : QObject() {operator =(obj);}
const JSON_Object &JSON_Object::operator =(const JSON_Object &obj)
{
    JSON_Object *obj2 = obj.copy();
    pValue = obj2->pValue;//recréé
    pType = obj2->pType;
    pChild = obj2->pChild;//recréé
    return *this;
}
JSON_Object *JSON_Object::copy() const
{
    JSON_Object *obj = new JSON_Object;
    *obj->pValue=*this->pValue;
    obj->pType=this->pType;
    QMapIterator<QString, JSON_Object*> i(*pChild);
    while(i.hasNext()) {
        i.next();
        obj->setChild(i.key(), i.value()->copy());//copy les enfants aussi
    }

    return obj;
}


void JSON_Object::setValue(QString key, QString value) {pValue->insert(key, value);}
QString JSON_Object::getValue(QString key) {return pValue->value(key);}
bool JSON_Object::containsValue(QString key) {return pValue->contains(key);}

bool JSON_Object::setType(int type)
{
    if(0 <= type && type <= 1) {pType = type;}
    else pType = JSON_Object::Object;
    return 0 <= type && type <= 1;
}
int JSON_Object::getType() {return pType;}
QString JSON_Object::getTypeString() {return TypeString.at(pType);}

void JSON_Object::setChild(QString name, JSON_Object *obj) {pChild->insert(name, obj);}
JSON_Object *JSON_Object::getChild(QString name) {return pChild->value(name);}

QString JSON_Object::getEntress() const
{
    return QList<QString>(pValue->keys() + pChild->keys()).join(", ");
}





