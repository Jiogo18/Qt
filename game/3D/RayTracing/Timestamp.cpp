#include "Timestamp.h"

/*
Timestamp::Timestamp() {}
Timestamp::Timestamp(qint64 time, qint32 id)
{
    this->time = time;
    this->id = id;
}
Timestamp::Timestamp(const Timestamp &timestamp)
{
    time = timestamp.time;
    id = timestamp.id;
}

bool Timestamp::operator ==(const Timestamp &timestamp) const
{ return time == timestamp.time && id == timestamp.time; }

bool Timestamp::operator <(const Timestamp &timestamp) const
{
    if(time == timestamp.time)
        return id < timestamp.id;
    return time < timestamp.time;
}

QDebug operator <<(QDebug debug, const Timestamp &timestamp)
{
    debug << "Timestamp(" + QString::number(timestamp.getTime())
             +"#" + QString::number(timestamp.getID())
             +")";
    return debug;
}




qint64 TIMESTAMP::last_time = 0;
qint32 TIMESTAMP::last_id = 0;
Timestamp TIMESTAMP::getUUID()
{
    qint64 time = getTime();
    if(time == last_time) {
        last_id++;
    }
    else {
        last_time = time;
        last_id = 0;
    }
    return Timestamp(time, last_id);
}
*/
