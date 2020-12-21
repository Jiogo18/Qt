#ifndef BLACKLIST_H
#define BLACKLIST_H

#include <QHostAddress>
#include <QMap>


class Blacklist
{
public:
    Blacklist();
    enum Reason {
        NotBlacklisted = 0,
        CommandBlacklist = 1,
        AutoBlacklist = 2,
        ByOperator = 4|CommandBlacklist, ByServer = 8|CommandBlacklist,
        TooManyConnection = 16|AutoBlacklist

    };
    bool addBlacklist(const QHostAddress &host, int raison);
    bool addBlacklist(const QString &host, int raison);
    bool unBlacklist(const QHostAddress &host);
    bool unBlacklist(const QString &host);
    int isBlacklisted(const QHostAddress &host) const;
    void debugList() const;

    static QString blacklistReason(int reason);

private:
    QMap<QHostAddress, int> blacklist;
    QMap<QString, int> blacklistIp;
};

bool operator<(const QHostAddress &host1, const QHostAddress &host2);

#endif // BLACKLIST_H
