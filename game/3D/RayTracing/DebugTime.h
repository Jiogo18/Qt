#ifndef DEBUG_H
#define DEBUG_H

#include <QMap>
#include <QDebug>
#include <QDateTime>

class DebugTimePart
{
public:
    DebugTimePart() {}
    void addValue(qint64 time);
    qint64 getMin() const { return min; }
    qint64 getMax() const { return max; }
    qint64 getNb() const { return nb; }
    qint64 getTotal() const { return total; }
    qint64 getMoyenne() const { if(nb==0) return 0; return total / nb; }
private:
    qint64 min = 0;
    qint64 max = 0;
    qint64 total = 0;
    qint64 nb = 0;
};

class DebugTime
{
public:
    DebugTime() {}
    void addValue(QString name, qint64 time);
    QMap<QString, DebugTimePart> getValues() const { return values; }
    static qint64 getCurrent() { return QDateTime::currentMSecsSinceEpoch(); }
    void clear() { values.clear(); }
private:
    QMap<QString, DebugTimePart> values;
};
QDebug operator <<(QDebug debug, const DebugTime &d);

#endif // DEBUG_H
