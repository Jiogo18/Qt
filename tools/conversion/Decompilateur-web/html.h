#ifndef HTML_H
#define HTML_H

#include <QString>

namespace html {
    QString compile(QString code);
    QString decompile(QString code, QString indentation);
}

#endif // HTML_H
