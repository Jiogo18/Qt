#ifndef PASSWORD_H
#define PASSWORD_H

#include <QCryptographicHash>
#include <QString>
#include <QDebug>
#include <QDataStream>
#include "UUID.h"

class Password
{
public:
    Password(QString password = "");
    Password(const QByteArray &password, const UUID &id = UUID());
    Password(const Password &password);
    void operator=(const Password &password);
    bool operator==(const Password &password) const { return this->password == password.password; }
    bool operator!=(const Password &password) const { return !operator==(password); }

    bool isStored() const { return !password.isEmpty(); }
    static bool isAcceptablePassword(QString password, QString *reason=nullptr);
    bool isAcceptable() const;

    void clear() { password.clear(); }
    static Password generateSentence() { return QString::number(QRandomGenerator::securelySeeded().generate64()); }

    Password hash() const { return isStored() ? Password(QCryptographicHash::hash(password, QCryptographicHash::Sha256), id) : *this; }
    Password hashWith(const Password &secureWord) const;
    Password chiffre(const Password &key) const;
    Password dechiffre(const Password &key) const { return chiffre(key); }
    Password reverse() const;

    QT_DEPRECATED
    const QByteArray &getData() const { return password; }

private:
    QByteArray password;
    UUID id;
    friend QDataStream &operator<<(QDataStream &out, const Password &t);
    friend QDataStream &operator>>(QDataStream &in, Password &t);
};

QDataStream &operator<<(QDataStream &out, const Password &t);
QDataStream &operator>>(QDataStream &in, Password &t);

#endif // PASSWORD_H
