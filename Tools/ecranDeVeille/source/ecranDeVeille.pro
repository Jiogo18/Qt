QT += widgets multimediawidgets multimedia core

SOURCES += \
    main.cpp \
    fenetre.cpp \
    toastIcon.cpp \
    videoWidget.cpp

FORMS += \
    fenetre.ui

HEADERS += \
    fenetre.h \
    toastIcon.h \
    videoWidget.h


DISTFILES +=
RC_ICONS = preferences-desktop-screensaver-2.ico


RESOURCES += \
    systray.qrc
