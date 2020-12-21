#include "anti_ghost_window.h"


anti_ghost_window::anti_ghost_window(bool hideApp) : QObject()
{
    fen = new fenetre(hideApp);//si on cache l'app, elle sera simple
    dirLog = QDir("log");
    out.setDevice(&currentFile);
    addLog("Starting...");


    lastWindow = nullptr;
    QObject::connect(&timerActu, &QTimer::timeout, this, &anti_ghost_window::actuCurrent);
    timerActu.start(500);//on est tranquille, c'est pas très gourment
}
anti_ghost_window::~anti_ghost_window()
{
    timerActu.stop();
    if(fen) {
        delete fen;
        fen = nullptr;
    }
    addLog("Closed");
    //currentFile.close();//pas besoin qt close automatiquement
}


HWND anti_ghost_window::getCurrentWindow()
{
    return GetForegroundWindow();
    //GetActiveWindow/GetFocus() : of this application (0 if not a window of this application)
    //GetForegroundWindow : for windows (every app)
    //GetCapture : ? game?
}
void anti_ghost_window::actuCurrent()
{
    HWND currentWindow = getCurrentWindow();
    if(currentWindow == lastWindow)
        return;
    lastWindow = currentWindow;

    addLog(getInfoWindow(currentWindow));
}
QString anti_ghost_window::getInfoWindow(HWND window)
{
    if(window == nullptr)
        return "Null Window";
    QString retour;
    //HDC dc = GetWindowDC(window);
    //HDC dc2 = GetDC(window);
    //qDebug() << "getInfoWindow:"
    //         << " dc:" << dc << " ou " << dc2;(résultats diff mais fonctionnent)

    retour += "PID:";
    DWORD pid;
    DWORD thread=GetWindowThreadProcessId(window, &pid);
    Q_UNUSED(thread)
    retour += QString::number(pid);
    //qDebug() << "PID" << pid << thread;


    retour += ", TITLE:\"";
    LPSTR titleChar = new CHAR[static_cast<uint>(GetWindowTextLengthA(window))];
    if(GetWindowTextA(window, titleChar, GetWindowTextLengthA(window)+1)) {//maxCount est exclut
        for(int i=0; i<GetWindowTextLengthA(window); i++)
            retour += titleChar[i];
    }
    retour += "\"";

    TITLEBARINFO titlebarInfo;
    titlebarInfo.cbSize = sizeof(TITLEBARINFO);
    if(GetTitleBarInfo(window, &titlebarInfo)) {
        QRect headerPos = QRect(QPoint(titlebarInfo.rcTitleBar.left,
                                 titlebarInfo.rcTitleBar.top),
                          QPoint(titlebarInfo.rcTitleBar.right,
                                 titlebarInfo.rcTitleBar.bottom));
        retour += ", HEADER:";
        retour += "(" + QString::number(headerPos.x())
                + "," + QString::number(headerPos.y())
                + " " + QString::number(headerPos.width())
                + "x" + QString::number(headerPos.height())
                + ")";
    }
    WINDOWINFO windowInfo;
    windowInfo.cbSize = sizeof(TITLEBARINFO);
    if(GetWindowInfo(window, &windowInfo)) {
        QRect windowPos = QRect(QPoint(windowInfo.rcWindow.left,
                                       windowInfo.rcWindow.top),
                                QPoint(windowInfo.rcWindow.right,
                                       windowInfo.rcWindow.bottom));
        QRect clientPos = QRect(QPoint(windowInfo.rcClient.left,
                                       windowInfo.rcClient.top),
                                QPoint(windowInfo.rcClient.right,
                                       windowInfo.rcClient.bottom));
        retour += ", GEOMETRY_FULL:";
        retour += "(" + QString::number(windowPos.x())
                + "," + QString::number(windowPos.y())
                + " " + QString::number(windowPos.width())
                + "x" + QString::number(windowPos.height())
                + ")";
        retour += ", GEOMETRY_CLIENT:";
        retour += "(" + QString::number(clientPos.x())
                + "," + QString::number(clientPos.y())
                + " " + QString::number(clientPos.width())
                + "x" + QString::number(clientPos.height())
                + ")";
    }

    return retour;
}

QString anti_ghost_window::getTimestamp()
{
    //format : [13:22:07.532]
    return QString("[")
            + QTime::currentTime().toString("HH:mm:ss.zzz")
            + QString("]");
}
void anti_ghost_window::addLog(QString log)
{
    QString log2 = getTimestamp() + " : " + log;
    if(fen)
        fen->addLogWindow(log2);
    //qDebug("%s", log2.toStdString().c_str());
    if(verifFileOut()) {
        out << log2 << "\n";
        nbDataSaved++;
    }
    else
        qDebug() << "Addlog n'a pas pu ajouter de log dans" << currentFile.fileName();
}

void anti_ghost_window::cleanLog() const//retire les logs créés (peut etre) par anti-ghost-window
{
    if(!dirLog.exists())
        return;//ça sert à riend e continuer
    QStringList files = dirLog.entryList(QStringList(fileLog), QDir::Files | QDir::CaseSensitive);
    for(int i=0; i<files.size(); i++) {
        QFile::remove(dirLog.filePath(files.at(i)));//efface tous les logs qui correspondent au critère
    }
}

QString anti_ghost_window::newFileName() const
{
    QString fileNameConflict = fileLog;
    fileNameConflict.replace("*", QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")+"*");
    QString fileName = fileNameConflict;
    fileName.remove("*");
    if(QFile::exists(dirLog.filePath(fileName))) {//si il existe déja
        fileNameConflict.replace("*", "_(*)");
        //on recupere directement tous les fichiers qui ont se noms
        QStringList conflictFile = dirLog.entryList(QStringList(fileNameConflict));
        int version = 2;
        while(conflictFile.contains(QString(fileNameConflict).replace("*", QString::number(version))))
            version++;//tant que le fileName
        fileName = QString(fileNameConflict).replace("*", QString::number(version));
    }
    return dirLog.filePath(fileName);
}

bool anti_ghost_window::verifFileOut(bool boucle)
{
    try {
        needLogDir();//on a besoin du dossier !
        if(currentFile.fileName().isEmpty())//ça sert à rien on va faire des boucles pour rien
            currentFile.setFileName(newFileName());

        if(!currentFile.isOpen()) {
            nbDataSaved=0;
            if(!currentFile.open(QFile::Append)) {//si on arrive pas à l'ouvrir
                if(boucle) {
                    qDebug() << "QFile : impossible d'ouvrir le fichier à l'emplacement " << currentFile.fileName();
                    return false;
                }
                currentFile.setFileName(newFileName());//on en créé un nouveau
                return verifFileOut(true);//si ça a marché
            }
            else {
                addLog("Open a new file log");//on sait juste qu'on commence un nouveau fichier (pas de conflits)
                //il est ouvert pour le moment
            }
        }
        //else => il est ouvert (pour le moment)

        //si on a dépassé la taille, on en ouvre un autre
        if(currentFile.size() > 10000000) {//10000 = 16000 octets => 16Mo = 10M
            QString fileName = newFileName();
            QString oldfileName = "";
            if(currentFile.isOpen()) {
                oldfileName = currentFile.fileName();
                out << getTimestamp() << " : Moved to \"" +fileName + "\"";//conflits avec out de addLog donc on fait ici
                currentFile.close();
            }

            currentFile.setFileName(fileName);
            if(!verifFileOut(true))//si on a pas pu l'ouvrir
                return false;
            //out.setDevice(&currentFile);

            //le fichier est ouvert donc il va direct écrire dans addLog
            addLog("Log file moved from \"" + oldfileName + "\" to \"" + fileName + "\"");//(pas de conflits)
        }

        if(50 < nbDataSaved && lastSecWrite + 20 < QDateTime::currentSecsSinceEpoch()) {
            //tous les 50 data (au min 20 sec pour pas surcharger) on actualise le fichier
            lastSecWrite = QDateTime::currentSecsSinceEpoch();
            currentFile.close();
            currentFile.open(QFile::Append);
        }

        return true;
    }
    catch(const std::exception &error) {
        qDebug() << "verifFileOut:" << error.what();
    }
    return false;
}


void anti_ghost_window::needLogDir() const
{
    if(!dirLog.exists())
        dirLog.mkdir(".");
}

