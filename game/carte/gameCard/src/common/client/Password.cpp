#include "Password.h"

Password::Password(QString password) : Password((password).toUtf8())
{
    operator=(hash());
}
Password::Password(const QByteArray &password, const UUID &id)
{
    this->password = password;
    this->id = id;
}
Password::Password(const Password &password)
{
    this->password = password.password;
    id = password.id;
}

void Password::operator=(const Password &password)
{
    this->password = password.password;
    id = password.id;
}


bool Password::isAcceptablePassword(QString password, QString *reason)
{
    if(password.size() == 0) {
        if(reason)
            *reason = "empty";
        return false;
    }

    password.remove(QRegExp("[\\s]"));
    if(password.isEmpty()) {
        if(reason)
            *reason = "' ' and '\n' only are not accepted";
        return false;
    }
    //**accepted** chars :
    password.remove(QRegExp("[!-zÀ-Üà-ü¡¿°ツ\\\\/\\^\\-\\s]"));

    if(password.size() > 0) {//il reste des chars qui ne sont pas autorisés
        *reason = "only chars of this regex are accepted [!-zÀ-Üà-ü¡¿°ツ\\\\/\\^\\-\\s]";
        return false;
    }
    return true;
}

bool Password::isAcceptable() const
{
    return isStored();
    //peut completer avec des bases de données (type " " hashé ou si il est trop lourd pour le serv)
}


Password Password::hashWith(const Password &secureWord) const
{
    Password retour(*this);
    QCryptographicHash hasheur(QCryptographicHash::Sha256);
    hasheur.addData(retour.password);
    hasheur.addData(secureWord.password);
    retour.password = hasheur.result();
    return retour;
}
Password Password::chiffre(const Password &key) const
{
    Password retour;
    retour.id = id;
    qint64 size = (password.size() > key.password.size()) ? password.size() : key.password.size();
    retour.password.resize(size);
    for(int i=0; i<size; i++) {
        char currentPass = (i<password.size() ? password.at(i) : 0);
        char currentKey = (i<key.password.size() ? key.password.at(i) : 0);
        retour.password[i] = currentPass ^ currentKey;
    }
    while(retour.password.size() > 0 && retour.password.back() == (char)0) {
        retour.password.remove(retour.password.size()-1, 1);
    }
    return retour;
}

Password Password::reverse() const
{
    Password retour;//on inverse l'ordre des bits
    retour.id = id;
    retour.password.resize(password.size());
    for(int i=0; i<password.size(); i++) {
        char currentPass = password.at(i);
        retour.password[password.size()-1 - i] = currentPass;
    }
    return retour;
}




QDataStream &operator<<(QDataStream &out, const Password &t)
{
    out << t.password << t.id;
    return out;
}
QDataStream &operator>>(QDataStream &in, Password &t)
{
    in >> t.password;
    in >> t.id;
    return in;
}
