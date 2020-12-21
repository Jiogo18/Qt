QT += widgets multimediawidgets multimedia core gui

SOURCES += \
    ecranDeVeille.cpp \
    imageTool.cpp \
    main.cpp \
    fenetre.cpp \
    toastIcon.cpp \
    videoWidget.cpp

FORMS += \
    fenetre.ui

HEADERS += \
    ecranDeVeille.h \
    fenetre.h \
    imageTool.h \
    toastIcon.h \
    videoWidget.h


DISTFILES +=
RC_ICONS = preferences-desktop-screensaver-2.ico


RESOURCES += \
    systray.qrc
