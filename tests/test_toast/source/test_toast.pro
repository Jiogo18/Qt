QT += widgets
requires(qtConfig(combobox))

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/desktop/systray
INSTALLS += target

SOURCES += \
    main.cpp \
    Window.cpp

HEADERS += \
    Window.h

RESOURCES += \
    systray.qrc
