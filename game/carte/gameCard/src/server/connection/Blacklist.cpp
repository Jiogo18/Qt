#include "Blacklist.h"

#include "src/common/LogNav.h"

Blacklist::Blacklist()
{

}

bool Blacklist::addBlacklist(const QHostAddress &host, int raison)
{ blacklist.insert(host, raison); return true; }
bool Blacklist::addBlacklist(const QString &host, int raison)
{ blacklistIp.insert(host, raison); return true; }

bool Blacklist::unBlacklist(const QHostAddress &host)
{
    if(blacklist.contains(host)) {
        blacklist.remove(host);
        return true;
    }
    if(blacklistIp.contains(host.toString())) {
        blacklistIp.remove(host.toString());
        return true;
    }
    return false;
}
bool Blacklist::unBlacklist(const QString &host)
{
    if(blacklistIp.contains(host)) {
        blacklistIp.remove(host);
        return true;
    }
    for(const QHostAddress &host2 : blacklist.keys()) {
        if(host == host2.toString()) {
            blacklist.remove(host2);
            return true;
        }
    }
    return false;
}

int Blacklist::isBlacklisted(const QHostAddress &host) const
{
    return blacklist.value(host, NotBlacklisted) | blacklistIp.value(host.toString(), NotBlacklisted);
}

void Blacklist::debugList() const
{
    if(blacklist.isEmpty() && blacklistIp.isEmpty()) {
        LogNav::addText("Blacklist is empty");
        return;
    }
    QString ret = QObject::tr("Blacklist with %n elements", "", blacklist.size() + blacklistIp.size());
    if(blacklist.size() > 0) {
        for(const QHostAddress &host : blacklist.keys()) {
            ret += "\n ["+host.toString()+"] : " + blacklistReason(blacklist.value(host));
        }
    }
    if(blacklistIp.size() > 0) {
        for(const QString &host : blacklistIp.keys()) {
            ret += "\n ["+host+"] : " + blacklistReason(blacklistIp.value(host));
        }
    }
    LogNav::addText(ret);
}

QString Blacklist::blacklistReason(int reason)
{
    QString ret = "";
    if(reason & Reason::AutoBlacklist) {
        ret += "Auto blacklisted, ";
    }
    if(reason & Reason::TooManyConnection) {
        ret += "Too Many connections";
    }
    return ret;
}


bool operator<(const QHostAddress &host1, const QHostAddress &host2)
{
    return host1.toString() < host2.toString();
}
