#include "UUID.h"

UUID::UUID()
{
    createdAt = QDateTime::currentMSecsSinceEpoch();
    randomId = QRandomGenerator::securelySeeded().generate64();
}

UUID::UUID(const UUID &uuid)
{
    createdAt = uuid.createdAt;
    randomId = uuid.randomId;
}

UUID *UUID::operator=(const UUID &uuid)
{
    createdAt = uuid.createdAt;
    randomId = uuid.randomId;
    return this;
}

bool UUID::operator==(const UUID &uuid)
{
    return createdAt == uuid.createdAt && randomId == uuid.randomId;
}



QDataStream &operator<<(QDataStream &out, const UUID &t)
{
    out << t.createdAt << t.randomId;
    return out;
}
QDataStream &operator>>(QDataStream &in, UUID &t)
{
    in >> t.createdAt;
    in >> t.randomId;
    return in;
}
