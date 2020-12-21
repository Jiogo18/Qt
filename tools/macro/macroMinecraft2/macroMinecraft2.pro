QT += widgets gui

SOURCES += \
    debug.cpp \
    fenetre.cpp \
    image.cpp \
    main.cpp \
    mc_craft.cpp \
    mc_gui.cpp \
    minecraft.cpp

HEADERS += \
    debug.h \
    fenetre.h \
    image.h \
    mc_craft.h \
    mc_gui.h \
    minecraft.h

RESOURCES += \
    macroMinecraft2.qrc


DISTFILES +=
RC_ICONS = icons/world.ico

FORMS += \
    fenetre.ui
