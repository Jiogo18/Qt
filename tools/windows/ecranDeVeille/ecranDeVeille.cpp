#include "ecranDeVeille.h"

const EDVConfig *EDVConfig::operator =(const EDVConfig &config)
{
    ressourcePath = config.ressourcePath;
    timeOut = config.timeOut;
    timeRefresh = config.timeRefresh;

    screenshot = config.screenshot;
    screenshotMargin = config.screenshotMargin;
    screenshotTime = config.screenshotTime;

    mouse = config.mouse;
    mouseMargin = config.mouseMargin;
    mouseTime = config.mouseTime;

    mouseV = config.mouseV;
    mouseVMargin = config.mouseVMargin;
    mouseVRepeat = config.mouseVRepeat;

    keyboard = config.keyboard;
    keyboardRepeat = config.keyboardRepeat;
    return this;
}
bool EDVConfig::operator ==(const EDVConfig &config)
{
    return (ressourcePath == config.ressourcePath &&
            timeOut == config.timeOut &&
            timeRefresh == config.timeRefresh &&

            screenshot == config.screenshot &&
            screenshotMargin == config.screenshotMargin &&
            screenshotTime == config.screenshotTime &&

            mouse == config.mouse &&
            mouseMargin == config.mouseMargin &&
            mouseTime == config.mouseTime &&

            mouseV == config.mouseV &&
            mouseVMargin == config.mouseVMargin &&
            mouseVRepeat == config.mouseVRepeat &&

            keyboard == config.keyboard &&
            keyboardRepeat == config.keyboardRepeat);
}

bool EDVConfig::save(QString filePath) const
{
    try {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "EDVConfig::save error: can't open " << filePath;
            return false;
        }
        QTextStream out(&file);
        out << "ressourcePath:" << ressourcePath << "\n";
        out << "timeOut:" << timeOut << "\n";
        out << "timeRefresh:" << timeRefresh << "\n";

        out << "screenshotEnabled:" << screenshot << "\n";
        out << "screenshotMargin:" << screenshotMargin << "\n";
        out << "screenshotTime:" << screenshotTime << "\n";

        out << "mouseEnabled:" << mouse << "\n";
        out << "mouseMargin:" << mouseMargin << "\n";
        out << "mouseTime:" << mouseTime << "\n";

        out << "mouseVEnabled:" << mouseV << "\n";
        out << "mouseVMargin:" << mouseVMargin << "\n";
        out << "mouseVRepeat:" << mouseVRepeat << "\n";

        out << "keyboardEnabled:" << keyboard << "\n";
        out << "keyboardRepeat:" << keyboardRepeat << "\n";

        file.close();
    } catch (const std::exception &error) {
        qDebug() << "EDVConfig::save error: can't save " << filePath << error.what();
        return false;
    }
    return true;
}
bool EDVConfig::load(QString filePath)
{
    try {
        QFile file(filePath);
        if(!file.exists()) {
            qDebug() << "EDVConfig::load" << filePath << " doesn't exist, it was created";
            save(filePath);
            return true;
        }
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "EDVConfig::load error: can't load " << filePath;
            return false;
        }

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            QString arg=line.mid(line.indexOf(":")+1);
            arg = arg.left(arg.size()-1);//espace de la fin
            if(line.startsWith("ressourcePath"))
                setRessourcePath(arg);
            if(line.startsWith("timeOut"))
                setTimeout(arg.toInt());
            if(line.startsWith("timeRefresh"))
                setTimeRefresh(arg.toInt());

            if(line.startsWith("screenshotEnabled"))
                setScreenshot(arg == "1");
            if(line.startsWith("screenshotMargin"))
                setScreenshotMargin(arg.toDouble());
            if(line.startsWith("screenshotTime"))
                setScreenshotTime(arg.toInt());

            if(line.startsWith("mouseEnabled"))
                setMouse(arg == "1");
            if(line.startsWith("mouseMargin"))
                setMouseMargin(arg.toInt());
            if(line.startsWith("mouseTime"))
                setMouseTime(arg.toInt());

            if(line.startsWith("mouseVEnabled"))
                setMouseV(arg == "1");
            if(line.startsWith("mouseVMargin"))
                setMouseVMargin(arg.toInt());
            if(line.startsWith("mouseVRepeat"))
                setMouseVRepeat(arg == "1");

            if(line.startsWith("keyboardEnabled"))
                setKeyboard(arg == "1");
            if(line.startsWith("keyboardRepeat"))
                setKeyboardRepeat(arg == "1");

        }
        file.close();
    } catch(const std::exception &error) {
        qDebug() << "EDVConfig::load error: can't load " << filePath << error.what();
        return false;
    }
    save(filePath);
    return true;
}




ecranDeVeille::ecranDeVeille(QObject *parent) : QObject(parent)
{
    config.load("ecranDeVeille.txt");

    viewVideo = new videoWidget;
    QObject::connect(viewVideo, &videoWidget::mouseMoved, this, &ecranDeVeille::mouseMovedOnWindow);
    QObject::connect(viewVideo, &videoWidget::keyPressed, this, &ecranDeVeille::keyPressedOnWindow);
    QObject::connect(viewVideo, &videoWidget::mousePressed, this, &ecranDeVeille::mousePressedOnWindow);
    QObject::connect(viewVideo, &videoWidget::onDebug, this, &ecranDeVeille::debug);
    viewVideo->setVideo(config.getRessourcePath());

    QObject::connect(&timerTimeout, &QTimer::timeout, this, &ecranDeVeille::showIf);
    QObject::connect(&timerScreenshot, &QTimer::timeout, this, &ecranDeVeille::timeoutScreenshot);
    QObject::connect(&timerMouse, &QTimer::timeout, this, &ecranDeVeille::timeoutMouse);
    timerTimeout.start(config.getTimeout() * 1000);
    timerScreenshot.start(config.getScreenshotTime() * 1000);
    timerMouse.start(config.getMouseTime());
}

ecranDeVeille::~ecranDeVeille() { delete viewVideo; }

void ecranDeVeille::setConfig(EDVConfig config)
{
    if(this->config == config)
        return;
    this->config = config;
    config.save("ecranDeVeille.txt");
    timerTimeout.setInterval(config.getTimeout() * 1000);
    timerScreenshot.setInterval(config.getScreenshotTime() * 1000);
    timerMouse.setInterval(config.getMouseTime());
    viewVideo->setVideo(config.getRessourcePath());
}

void ecranDeVeille::showIf()
{
    if(config.isScreenshot())//en cas de screenshotTime < timeout
        if(timeoutScreenshot())
            return;//si finallement on a actu...
    show();
}

void ecranDeVeille::show()
{
    lastMousePos = QCursor::pos();//on l'actualise
    timerTimeout.stop();
    timerScreenshot.stop();
    timerMouse.stop();
    viewVideo->show();
    emit shown();
}

void ecranDeVeille::hide()
{
    viewVideo->hide();
    timerTimeout.start();
    timerScreenshot.start();
    timerMouse.start();
    emit hidden();
}


bool ecranDeVeille::timeoutScreenshot()
{

    if(config.getScreenshotMargin() >= 100)//toutes les images seront à 100% de diff max
        return false;//pas besoin de calculer, il y a pas de changement
    QImage currentScreenshot = imageTool::tookScreenshot();

    bool change = false;
    if(imageTool::nullImage(currentScreenshot) ||
            imageTool::difference(lastScreenShot, currentScreenshot) > config.getScreenshotMargin()/100.0)
    {
        //si nullImage c'est qu'on doit etre en plein écran sur un jeu
        //si il y a trop de diff (lecteur vidéo)
        //alors on veut pas de l'écran de veille
        timerTimeout.start();//restart
        change = true;
    }
    lastScreenShot = currentScreenshot;
    return change;
}

void ecranDeVeille::timeoutMouse()
{
    const int vari = config.getMouseMargin();
    QPoint currentPos = QCursor::pos();
    if(lastMousePos == currentPos)
        return;
    if(!(currentPos.x()-vari < lastMousePos.x() && lastMousePos.x() < currentPos.x()+vari &&
                currentPos.y()-vari < lastMousePos.y() && lastMousePos.y() < currentPos.y()+vari))
    {//legere variation du curseur (en carré)
        timerTimeout.start();//restart
        lastMousePos = currentPos;
    }
}

void ecranDeVeille::mouseMovedOnWindow()
{
    if(!config.isMouseV())
        return;
    int vari = config.getMouseVMargin();
    QPoint currentPos = QCursor::pos();
    if(lastMousePos.isNull())
        lastMousePos = currentPos;
    if(!(currentPos.x()-vari < lastMousePos.x() && lastMousePos.x() < currentPos.x()+vari &&
                currentPos.y()-vari < lastMousePos.y() && lastMousePos.y() < currentPos.y()+vari))
        hide();
}
void ecranDeVeille::keyPressedOnWindow(const QKeyEvent *event)
{
    if(!config.isKeyboard())
        return;
    hide();
    if(config.isKeyboardRepeat() && event->text().length()==1) {
        try {
            char touche = event->text().at(0).toLatin1();
            INPUT ip;
            ip.type = INPUT_KEYBOARD;
            ip.ki.wVk = VkKeyScan(touche);
            ip.ki.wScan = MapVirtualKeyEx(VkKeyScan(touche), 0, (HKL)0xf0010413);;

            ip.ki.dwFlags = 0; //key press
            SendInput(1, &ip, sizeof(INPUT));
            ip.ki.dwFlags = KEYEVENTF_KEYUP;//key release
            SendInput(1, &ip, sizeof(INPUT));
        } catch(const std::exception &error) { Q_UNUSED(error) }//ctrl marche pas
    }

}
void ecranDeVeille::mousePressedOnWindow(const QMouseEvent *event)
{
    if(!config.isMouseV())
        return;
    hide();
    if(config.isMouseVRepeat()) {
        switch(event->button()) {
        case Qt::LeftButton:
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        case Qt::RightButton:
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            break;
        case Qt::MiddleButton:
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            break;
        default: break;//TODO: gérer plus de boutons si besoin
        }
    }
}


