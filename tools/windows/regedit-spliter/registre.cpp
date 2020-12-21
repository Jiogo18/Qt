#include "registre.h"

registre::registre() {}

void registre::split(QString regFile)
{
    if(regFile.isEmpty()) {
        QMessageBox(QMessageBox::Critical, "Split", "Please add a path to the reg file").exec();
        return;
    }
    //Reset
    lastWhitelistPath = "";
    lastBlacklistPath = "";
    clefsOriginal = 0;
    clefsWhitelist = 0;
    clefsSemiWhitelist = 0;
    clefsBlacklist = 0;
    whitelistValueNotable.clear();
    pbChargement->setMinimum(0);
    pbChargement->setMaximum(100);
    pbChargement->setValue(0);

    //Open the file
    regFile.replace("\\", "/");
    QFile *fileSource = open(regFile, true, QMessageBox::Critical);
    if(!fileSource) {
        qWarning() << "registre::split/error fileSource not opened:" << regFile;
        return;
    }

    //Load keys
    QString bufIn = "";
    QString bufWhitelist = "";
    {
        QTextStream in(fileSource);
        bufIn = in.readAll();
        //in.codec()->name() == "UTF-16LE"
        removeKeyAt(&bufIn, 0);//remove the header
        fileSource->close();
        for(int i=0; i<bufIn.size(); i++) {
            i = bufIn.indexOf("\r\n\r\n", i);
            if(i==-1) break;
            clefsOriginal++;//compte le nombre de clefs
        }
        pbChargement->setMaximum(bufIn.size());
    }


    //whitelist with regex
    QRegularExpression regex;
    int iRegex = 0;
    {
        QString regexPattern = "";
        for(int i=0; i<whitelist.size(); i++) {
            if(whitelist.at(i) == "")
                continue;
            if(regexPattern.size() > 0)
                regexPattern += "|";
            regexPattern += "(" + whitelist.at(i) + ")";
        }
        regex.setPattern(regexPattern);
        if(!regex.isValid()) {
            qWarning() << "registre::split/error regexs invalid:" << regex.pattern();
            iRegex = -1;
        }
        else if(regex.pattern() == "") {
            qWarning() << "registre::split/warning regex have an empty pattern, please make a whitelist";
            iRegex = -1;
        }
    }


    //Get every keys which match with regex
    while(iRegex != -1) {
        QRegularExpressionMatch match = regex.match(bufIn, iRegex);
        if(!match.hasMatch())
            break;//fini
        iRegex = match.capturedEnd()+1;//pour rechercher plus loins au prochain tour

        int beginIn = getKeyBegin(bufIn, match.capturedStart());
        int endIn = getKeyEnd(bufIn, match.capturedStart());
        QString blockKey = bufIn.mid(beginIn, endIn-beginIn+1);

        if(!isBlacklistKey(blockKey)) {//si c'est pas blacklist

            if(match.capturedStart() - beginIn < blockKey.indexOf("]\r\n")) {
                //position relative de la recherche < end of header
                //si dans le header (la clef)
                //alors on prend le tout
                clefsWhitelist++;
                bufIn.remove(beginIn, endIn-beginIn+1);//retire de bufIn...
                iRegex = beginIn;
                bufWhitelist += blockKey;//...et le met dans bufWhitelist
                addNotableValue(blockKey);
            }
            else {
                //alors on prend que la clef + les elements qui correspondent (avec la whitelist)
                clefsSemiWhitelist++;
                QString keyHeader = blockKey.mid(0, blockKey.indexOf("]\r\n")+3);
                bufWhitelist += keyHeader;//1ere ligne (avec les chars)
                addNotableValue(keyHeader);

                while(true && !blockKey.isEmpty()) {
                    QRegularExpressionMatch matchLine = regex.match(blockKey);//pas besoin d'index car on suppr les valeurs jusqu'à la dernière
                    if(!matchLine.hasMatch())
                        break;
                    int beginLine = blockKey.lastIndexOf("\r\n\"", matchLine.capturedStart());//forcément au début de la colonne
                    if(beginLine == -1) beginLine = 0;
                    else beginLine += 2;//on veut pas de \r\n
                    {
                        int beginLine2 = blockKey.lastIndexOf("\r\n@=", matchLine.capturedStart());
                        if(beginLine2 == -1) beginLine2 = 0;
                        else beginLine2 += 2;
                        if(beginLine2 > beginLine)
                            beginLine = beginLine2;
                    }

                    int endLine = blockKey.indexOf("\r\n\"", matchLine.capturedStart());//forcément au début d'une valeur
                    if(endLine == -1) endLine = blockKey.size()-2;
                    else endLine += 1;//avec le \n en plus
                    {
                        int endLine2 = blockKey.indexOf("\r\n@=", matchLine.capturedStart());
                        if(endLine2 == -1) endLine2 = blockKey.size()-2;
                        endLine2 += 1;
                        if(endLine2 < endLine)
                            endLine = endLine2;
                    }

                    QString keyLine = blockKey.mid(beginLine, endLine-beginLine+1);
                    bufWhitelist += keyLine;
                    bufIn.remove(beginIn+beginLine, endLine-beginLine+1);
                    blockKey.remove(beginLine, endLine-beginLine+1);
                    endIn -= endLine-beginLine+1;
                    addNotableValue(keyLine);
                }
                iRegex = beginIn;//on reviens au début du block (car il contient plus aucun element du regex)
                bufWhitelist += "\r\n";//on l'a pas car il est resté dans bufIn
            }
        }
        else
            clefsBlacklist++;//clefs whitelist mais retiré par la blacklist

        //update info
        if((clefsWhitelist+clefsSemiWhitelist) % 5 == 0) {
            pbChargement->setMaximum(bufIn.size());//update car bufIn se vide
            pbChargement->setValue(iRegex);//on a déjà fait i tours
            QCoreApplication::processEvents();
            qWarning() << "registre::split/info" << pbChargement->value() << pbChargement->maximum()
                       << "(" << (pbChargement->value()*100.0/pbChargement->maximum()) << "%,"
                       << clefsOriginal << "clefs,"
                       << clefsWhitelist << "en whitelist,"
                       << clefsSemiWhitelist << "en semi-whitelist"
                       << ")";
        }
    }


    //And save
    QString fileNameWhitelist = "", fileNameBlacklist = "";
    {
        QFile *fileWhitelist = nullptr, *fileBlacklist = nullptr;
        QString filePath = regFile.mid(0, regFile.lastIndexOf("/"));
        QString fileName = regFile.mid(regFile.lastIndexOf("/") + 1).remove(".reg").remove(".edit").remove(".txt");
        int fileNumber = 0;
        if(fileName.contains("whitelist")) {
            int index = fileName.lastIndexOf("-whitelist");
            fileName = fileName.remove(index, QString("-whitelist").size());
            fileNumber = getFileNumber(fileName, index) + 1;
        }
        else if(fileName.contains("-blacklist")) {
            int index = fileName.lastIndexOf("-blacklist");
            fileName = fileName.remove(index, QString("-blacklist").size());
            fileNumber = getFileNumber(fileName, index) + 1;
        }

        fileNameWhitelist = fileName + "-whitelist-" + QString::number(fileNumber) + ".reg.edit";
        fileNameBlacklist = fileName + "-blacklist-" + QString::number(fileNumber) + ".reg.edit";
        lastWhitelistPath = filePath + "/" + fileNameWhitelist;
        lastBlacklistPath = filePath + "/" + fileNameBlacklist;
        fileWhitelist = open(lastWhitelistPath, false);
        fileBlacklist = open(lastBlacklistPath, false);

        if(fileWhitelist) {
            QTextStream outW(fileWhitelist);
            outW.setCodec("UTF-16LE");
            outW << bufWhitelist;
            fileWhitelist->close();
            delete fileWhitelist;
        }
        else
            fileNameWhitelist = "";
        if(fileBlacklist) {
            QTextStream outB(fileBlacklist);
            outB.setCodec("UTF-16LE");
            outB << bufIn;
            fileBlacklist->close();
            delete fileBlacklist;
        }
        else
            fileNameBlacklist = "";
    }
    pbChargement->setValue(pbChargement->maximum());

    //Finishing
    QMessageBox(QMessageBox::Information, "Rechercher et Sépararer", "Séparation terminée vers\n"
                +(fileNameWhitelist.isEmpty() ? "aucun fichier" : fileNameWhitelist)+"\net\n"
                +(fileNameBlacklist.isEmpty() ? "aucun fichier" : fileNameBlacklist)).exec();
    delete fileSource;
}

QFile *registre::open(QString fileName, bool openOnly, QMessageBox::Icon iconIf)
{
    QFile *file = new QFile(fileName);
    if (!file->open(openOnly ? QFile::ReadOnly : QFile::WriteOnly)) {
        QString fileShortName = fileName.mid(fileName.lastIndexOf("/")+1);
        if(openOnly)
            QMessageBox(iconIf, "File not opened", "\"./" + fileShortName + "\" can't be used to read.").exec();
        else
            QMessageBox(iconIf, "File not opened", "\"./" + fileShortName + "\" can't be used to write.").exec();
        qWarning() << "can't open the file " << fileShortName << file->error() << " (to" << fileName.left(fileName.lastIndexOf("/")+1) << ")";
        file->close();
        delete file;
        return nullptr;
    }
    if(openOnly) {
            QString readError = "";
            if(!isRegFile(file, &readError)) {
                qWarning() << "file not opened because:" << readError;
                QMessageBox(iconIf, "File not read", "\"" + fileName + "\"\n" + readError).exec();
                file->close();
                delete file;
                return nullptr;
            }
        }
    else {
        QTextStream out(file);
        out.setCodec("UTF-16LE");
        out << "Windows Registry Editor Version 5.00\r\n\r\n";//header
    }
    return file;
}
bool registre::isRegFile(QFile *file, QString *error)//is the file opened a .reg file?
{
    if(file->atEnd()) {
        if(error)
            *error = "is an empty file.";
        return false;
    }
    QTextStream in(file);
    QString line = in.read(50);//36 caractères max en version 5.0
    file->seek(0);//reset
    int pos = 0;
    line = getKeyAt(line, pos);
    if(!line.contains("Windows Registry Editor Version")) {
        //it doesn't contain the header
        if(error)
            *error = "is not a reg file.";
        return false;
    }
    else {
        bool ok=false;
        line = line.replace("Windows Registry Editor Version ", "");
        double version = line.toDouble(&ok);
        if(!ok) {
            qWarning() << "registre::isRegFile have an unkown version:" << line;
            if(error)
                *error = "have an unknown version (NaN)";
            return false;
        }
        else if(version != 5.0) {//not the good version
            if(error)
                *error = "have an unknown version ("+QString::number(version)+")";
            return false;
        }
    }
    if(error)
        *error = "";
    return true;
}
int registre::getFileNumber(QString &fileName, int index)
{
    if(index < fileName.size()) {//qq chose après l'index
        if(fileName.lastIndexOf("-")==index)
            fileName.remove(index,1);
        bool ok=false;
        int fileNumber = fileName.mid(index).toInt(&ok);//le reste
        fileName = fileName.left(index);//retire le reste

        if(!ok) return 0;
        return fileNumber;
    }
    return 0;
}

int registre::getKeyBegin(const QString &in, int pos)
{
    int begin = in.lastIndexOf("\r\n\r\n[", pos);
    if(begin == -1) return 0;
    return begin += 4;//sans les 4 chars
}
int registre::getKeyEnd(const QString &in, int pos)
{
    int end = in.indexOf("\r\n\r\n[", pos);
    if(end == -1) return in.size()-1;
    return end += 3;//avec les 3 autres chars
}
QString registre::getKeyAt(const QString &in, int &pos)
{
    int begin = getKeyBegin(in, pos);
    int end = getKeyEnd(in, pos);
    pos -= begin;//position relative
    return in.mid(begin, end-begin+1);
}
int registre::removeKeyAt(QString *in, int pos)
{
    int begin = getKeyBegin(*in, pos);
    in->remove(begin, getKeyEnd(*in, pos)-begin+1);
    return begin;//qui est maintenant le begin d'une autre clef
}

bool registre::isWhitelistKey(QString key) const
{
    for(int i=0; i<whitelist.size(); i++)
        if(whitelist.at(i) != "" && key.contains(whitelist.at(i)))
            return true;
    return false;
}
bool registre::isBlacklistKey(QString key) const
{
    for(int i=0; i<blacklist.size(); i++)
        if(blacklist.at(i) != "" && key.contains(blacklist.at(i)))
            return true;
    return false;
}

void registre::addNotableValue(const QString &key)
{
    int iExp = 0;
    while(true) {
        QRegularExpressionMatch match = regexNotable.match(key, iExp);
        if(!match.hasMatch())
            break;//plus rien

        iExp = match.capturedEnd()+1;
        QString value = match.captured();
        value.remove(0, 1);//frist
        value.remove(value.size()-1, 1);//last
        if(!whitelistValueNotable.contains(value) && !whitelist.contains(value))
            whitelistValueNotable.append(value);
    }
}
