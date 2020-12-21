#include "html.h"

QString html::compile(QString code)
{
    QList<QString> tree;
    for(QChar c : code) {
        if(tree.last() == c) {
            tree.removeLast();
        }
    }
}

QString html::decompile(QString code, QString indentation)
{

}
