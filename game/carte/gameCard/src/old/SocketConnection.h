#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include "SocketReceiver.h"
#include "src/common/client/Password.h"
#include "src/common/client/ClientInfo.h"

class SocketConnection : public SocketPacket
{
public:
    enum Step {
        Stopped,
        StepSentence,
        StepPassword,
        StepAutorized,
        StepNotAutorized
    };
    enum ConnectionError {
        NoError,
        ErrorTimeout,
        ErrorWrongServerPassword,
        ErrorWrongLogin
    };
    SocketConnection(Step state = Stopped, ConnectionError error = NoError);
    SocketConnection *operator =(const SocketConnection &sktCnct);

    bool isEmpty() const { return state == Stopped && error == NoError && password.isEmpty(); }

    Step getState() const { return state; }
    ConnectionError getError() const { return error; }
    Password getPassword(QString name) const { return password.value(name); }
    QMap<QString, Password> getPasswords() const { return password; }
    ClientInfo getClientInfo() const { return clientInfo; }

    void addPassword(QString name, Password password, Password key) { password.chiffre(key); this->password.insert(name, password); }
    void setPasswords(QMap<QString, Password> passwords) { password = passwords; }
    void setClientInfo(ClientInfo info) { clientInfo = info; }

private:
    Step state = Stopped;
    ConnectionError error = NoError;
    QMap<QString, Password> password;
    ClientInfo clientInfo;
};

QDataStream &operator<<(QDataStream &out, const SocketConnection &sktCnct);
QDataStream &operator>>(QDataStream &in, SocketConnection &sktCnct);
QDebug operator<<(QDebug debug, const SocketConnection &sktCnct);

#endif // SOCKETCONNECTION_H
