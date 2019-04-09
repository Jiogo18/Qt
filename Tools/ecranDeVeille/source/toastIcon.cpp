#include "toastIcon.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QMenu>
#include <QAction>
#include <QCoreApplication>
//#include <QCloseEvent>
#include "fenetre.h"

toastIcon::toastIcon(fenetre *parent) : QSystemTrayIcon((QObject*)parent)
{
    parentP=parent;
    createTrayIcon();

    connect(this, &QSystemTrayIcon::activated, this, &toastIcon::iconActivated);

    QSystemTrayIcon::setIcon(parentP->windowIcon());
    QSystemTrayIcon::setToolTip(parentP->windowTitle());
    QSystemTrayIcon::show();
}

void toastIcon::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    restoreAction->setEnabled(parentP->isMaximized() || !visible);
    parentP->setVisible(visible);
}

void toastIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger://presser
    case QSystemTrayIcon::DoubleClick:
        parentP->setVisible(!parentP->isVisible());//permer de recacher la fenetre
        break;
    case QSystemTrayIcon::MiddleClick:
        quitAction->trigger();//quit
        break;
    default: break;
    }
}

void toastIcon::createTrayIcon()
{
    trayIconMenu = new QMenu(parentP);
    minimizeAction = new QAction(tr("Mi&nimize"), parentP);
    connect(minimizeAction, &QAction::triggered, parentP, &QWidget::hide);
    trayIconMenu->addAction(minimizeAction);

    restoreAction = new QAction(tr("&Restore"), parentP);
    connect(restoreAction, &QAction::triggered, parentP, &QWidget::showNormal);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    trayIconMenu->addAction(quitAction);

    QSystemTrayIcon::setContextMenu(trayIconMenu);
}

#endif
