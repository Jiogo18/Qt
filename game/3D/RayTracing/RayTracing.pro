QT += widgets gui core #winextras

SOURCES += \
    DebugTime.cpp \
    GUI.cpp \
    Object3D.cpp \
    Point3D.cpp \
    RayTracing.cpp \
    Timestamp.cpp \
    Transfo.cpp \
    fenetre.cpp \
    main.cpp \
    map3D.cpp

HEADERS += \
    DebugTime.h \
    GUI.h \
    Object3D.h \
    Point3D.h \
    RayTracing.h \
    Timestamp.h \
    Transfo.h \
    fenetre.h \
    map3D.h

RC_ICONS = icone.ico

RESOURCES += \
    ressourpack.qrc

OTHER_FILES += bugs.txt
