#ifndef UUID_H
#define UUID_H

#include <QDateTime>
#include <QRandomGenerator>
#include <QDataStream>

class UUID
{
public:
    UUID();
    UUID(const UUID &uuid);
    UUID *operator=(const UUID &uuid);
    bool operator==(const UUID &uuid);

private:
    qint64 createdAt = 0;
    qint64 randomId= 0;

    friend QDataStream &operator<<(QDataStream &out, const UUID &t);
    friend QDataStream &operator>>(QDataStream &in, UUID &t);
};

QDataStream &operator<<(QDataStream &out, const UUID &t);
QDataStream &operator>>(QDataStream &in, UUID &t);

#endif // UUID_H
