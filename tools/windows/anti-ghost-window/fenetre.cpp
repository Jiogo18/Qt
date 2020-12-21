#include "fenetre.h"

fenetre::fenetre(bool simple, QWidget *parent) : QWidget(parent)
{
    //Widget
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    if(simple)
        lblGhost = new QLabel("Anti Ghost Window (simplified):\nRecap every windows in the foreground.");
    else
        lblGhost = new QLabel("Anti Ghost Window (call 'app.exe --hide' to hide this window at start):\nRecap every windows in the foreground.");
    layout->addWidget(lblGhost);
    if(!simple) {
        lblCurrentWindow = new QLabel("Current window:");
        layout->addWidget(lblCurrentWindow);
        leCurrentWindow = new QLineEdit();
        leCurrentWindow->setReadOnly(true);
        leCurrentWindow->setCursor(Qt::IBeamCursor);
        layout->addWidget(leCurrentWindow);

        txtWindow = new QTextEdit(leCurrentWindow->text());
        txtWindow->setReadOnly(true);
        txtWindow->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard);
        layout->addWidget(txtWindow);
        QWidget::show();
        QRect geo = QWidget::geometry();
        geo.setWidth(500);
        QWidget::setGeometry(geo);//contraint la width à 540 de base
    }
    else {
        QWidget::showMinimized();
        QWidget::hide();
    }
}

fenetre::~fenetre()
{
    if(lblCurrentWindow) {
        delete lblCurrentWindow;
        lblCurrentWindow = nullptr;
    }
    if(txtWindow) {
        delete txtWindow;
        txtWindow = nullptr;
    }
    if(leCurrentWindow) {
        delete leCurrentWindow;
        leCurrentWindow = nullptr;
    }
}

void fenetre::addLogWindow(QString log)
{
    if(leCurrentWindow)
        leCurrentWindow->setText(log);
    if(txtWindow != nullptr) {
        txtWindow->append(log);
        if(txtWindow->toPlainText().split("\n").length() > 100) {
            QString txt = txtWindow->toPlainText();
            QStringList line = txt.split("\n");
            for(int i=0; i<50; i++)
                line.removeFirst();
            txtWindow->setText(line.join("\n"));// ! ne pas mettre de \n dans les addLog !
        }
    }
}

