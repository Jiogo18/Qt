#include "Packet.h"

QByteArray Packet::makePkt(Packet::PacketType type, std::function<void(QDataStream*)> func)
{
    QByteArray *packet = new QByteArray;
    QDataStream *out = new QDataStream(packet, QIODevice::WriteOnly);
    *out << (Size)0;//taille du paquet

    *out << version;
    *out << type;
    func(out);

    out->device()->seek(0);
    *out << (Size)(packet->size() - sizeof(Size));//taille du packet sans le quint64
    return *packet;
}

QString Packet::Connection::toString(const Packet::Connection::RejectedReason &reason, bool complete)
{
    switch (reason) {
    case Timeout:
        return QString(QObject::tr("Auth Timeout"))
                +(complete ? ", "+QObject::tr("You had %n seconds to connect", "", timeToAuth/1000.0) : "");
    case WrongServPassword:
        return QObject::tr("Wrong Server Password");
    case WrongLogin:
        return QString(QObject::tr("Wrong Login"))
                +(complete ? ", "+QObject::tr("This login is not available, try something else") : "");
    case AlreadyConnected:
        return QObject::tr("Already Connected");
    case WrongPassword:
        return QString(QObject::tr("Wrong Password"))
                +(complete ? ", "+QObject::tr("An account is already registered with another password") : "");
    case SkippedSteps:
        return QString(QObject::tr("Skipped Steps"))
                +(complete ? ", "+QObject::tr("Your version may be not supported") : "");
    case OtherReason:
        return QObject::tr("Other Reason");
    }
    return QObject::tr("Unkow");
}
