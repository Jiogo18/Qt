QT += widgets network

HEADERS += \
    src/client/connection/WidgetConnection.h \
    src/client/fenetre.h \
    src/client/Plateau.h \
    \
    src/common/client/Client.h \
    src/common/client/ClientAuth.h \
    src/common/client/ClientInfo.h \
    src/common/client/Password.h \
    src/common/client/UUID.h \
    \
    src/common/command/commands/Command.h \
    src/common/command/commands/CommandHelp.h \
    src/common/command/CommandMain.h \
    src/common/command/CommandManager.h \
    src/common/command/CommandRegister.h \
    src/common/command/CommonCommands.h \
    \
    src/common/connection/Packet.h \
    src/common/connection/PacketManager.h \
    src/common/connection/Socket.h \
    \
    src/common/game/Card.h \
    src/common/game/CardGroup.h \
    src/common/game/Player.h \
    \
    src/common/widget/Chat.h \
    src/common/WidgetNav.h \
    src/common/LogNav.h \


SOURCES += \
    src/client/connection/WidgetConnection.cpp \
    src/client/fenetre.cpp \
    src/client/main.cpp \
    src/client/Plateau.cpp \
    \
    src/common/client/Client.cpp \
    src/common/client/ClientAuth.cpp \
    src/common/client/ClientInfo.cpp \
    src/common/client/Password.cpp \
    src/common/client/UUID.cpp \
    \
    src/common/command/CommonCommands.cpp \
    src/common/command/commands/Command.cpp \
    src/common/command/commands/CommandHelp.cpp \
    src/common/command/CommandMain.cpp \
    src/common/command/CommandManager.cpp \
    src/common/command/CommandRegister.cpp \
    \
    src/common/connection/Packet.cpp \
    src/common/connection/PacketManager.cpp \
    src/common/connection/Socket.cpp \
    \
    src/common/game/Card.cpp \
    src/common/game/CardGroup.cpp \
    src/common/game/Player.cpp \
    \
    src/common/widget/Chat.cpp \
    src/common/WidgetNav.cpp \
    src/common/LogNav.cpp \


RESOURCES += \
    ressources/image.qrc
