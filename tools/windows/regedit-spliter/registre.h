#ifndef REGISTRE_H
#define REGISTRE_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QRegularExpression>

#include <QProgressBar>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>

class registre
{
public:
    registre();

    void setWhitelist(QStringList whitelist) { this->whitelist = whitelist; }
    void setBlacklist(QStringList blacklist) { this->blacklist = blacklist; }
    void setProgressBar(QProgressBar *pb) { pbChargement = pb; }
    void split(QString regFile);

    QString getLastWhitelistPath() const { return lastWhitelistPath; }
    QString getLastBlacklistPath() const { return lastBlacklistPath; }
    int getClefsOriginal() const { return clefsOriginal; }
    int getClefsWhitelist() const { return clefsWhitelist; }
    int getClefsSemiWhitelist() const { return clefsSemiWhitelist; }
    int getClefsBlacklist() const { return clefsBlacklist; }
    QStringList getWhitelistValueNotable() { return whitelistValueNotable; }


    static QFile *open(QString fileName, bool openOnly=true, QMessageBox::Icon iconIf=QMessageBox::Warning);
    static bool isRegFile(QFile *file, QString *error=nullptr);
    static int getFileNumber(QString &fileName, int index);

    static int getKeyBegin(const QString &in, int pos);
    static int getKeyEnd(const QString &in, int pos);
    static QString getKeyAt(const QString &in, int &pos);
    static int removeKeyAt(QString *in, int pos=0);//retourne l'index de début de block
    bool isWhitelistKey(QString key) const;
    bool isBlacklistKey(QString key) const;
    void addNotableValue(const QString &key);


private:
    QStringList whitelist = {};
    QStringList blacklist = {};
    QString lastWhitelistPath = "";
    QString lastBlacklistPath = "";
    int clefsOriginal = 0;
    int clefsWhitelist = 0;
    int clefsSemiWhitelist = 0;
    int clefsBlacklist = 0;
    QStringList whitelistValueNotable = {};
    const QRegularExpression regexNotable = QRegularExpression(QString()+
                    "(\\\W[0-9A-F]{32}\\\W)|"+//[ or \ or "HEXA_KEY_SIZE32] or \ or "
                        //obligé d'avoir le warning car c++ connait pas
                    "({[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}})",//{AN-HEXA-KEY-FOR-INSTALLATION}
                QRegularExpression::CaseInsensitiveOption);
    QProgressBar *pbChargement = nullptr;
};

#endif // REGISTRE_H
