#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QtGlobal>
#include <QDebug>
#include <QDateTime>


/*class Timestamp {
public:
    Timestamp();
    Timestamp(qint64 time, qint32 id);
    Timestamp(const Timestamp &timestamp);
    qint64 getTime() const { return time; }
    qint32 getID() const { return id; }

    bool operator ==(const Timestamp &timestamp) const;
    bool operator <(const Timestamp &timestamp) const;

protected:
    qint64 time=0;
    qint32 id=0;
};
QDebug operator <<(QDebug debug, const Timestamp &timestamp);
*/

namespace TIMESTAMP {
    //Timestamp getUUID();
    //extern qint32 last_id;
    //extern qint64 last_time;
    //static qint64 getTime() { return QDateTime::currentMSecsSinceEpoch(); }
}



/*class Timestamp
{
    enum type {
        Block,
        Entity
    };

public:
    static Timestamp fromBlock(Point3D point);
    static Timestamp fromEnity(QString entityType);

private:
    Type type;
};*/


#endif // TIMESTAMP_H
