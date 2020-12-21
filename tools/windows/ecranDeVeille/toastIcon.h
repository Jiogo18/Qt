#ifndef TOASTICON_H
#define TOASTICON_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QWidget>
class QAction;
class QMenu;
class fenetre;

class toastIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    toastIcon(fenetre *parent);
    ~toastIcon();
    void setVisible(bool visible);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void createTrayIcon();

    fenetre *parentP;

    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif // TOASTICON_H
