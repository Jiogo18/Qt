SOURCES += \
    main.cpp \
    fenetre.cpp \
    connexion.cpp \
    parametre.cpp \
    information.cpp \
    cartes.cpp

FORMS += \
    fenetre.ui \
    connexion.ui \
    parametre.ui \
    information.ui

HEADERS += \
    fenetre.h \
    connexion.h \
    parametre.h \
    information.h \
    cartes.h


QT += widgets core network #multimedia

RESOURCES += \
    cartes.qrc


DISTFILES +=
RC_ICONS = icone.ico
