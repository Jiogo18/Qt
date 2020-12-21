QT += widgets network

HEADERS += \
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
    \
    src/server/command/commands/Blacklist.h \
    src/server/command/ServCommands.h \
    src/server/command/ServNav.h \
    \
    src/server/connection/Blacklist.h \
    src/server/connection/Server.h \
    \
    src/server/fenetre.h \
    src/server/game/GameServer.h \

SOURCES += \
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
    \
    src/server/command/ServerCommands.cpp \
    src/server/command/commands/CommandBlacklist.cpp \
    \
    src/server/connection/Blacklist.cpp \
    src/server/connection/Server.cpp \
    \
    src/server/fenetre.cpp \
    src/server/game/GameServer.cpp \
    src/server/main.cpp \

TRANSLATIONS = \
    lang/server_fr.ts
#CREATE the file with lupdate gameCard-Server.pro -ts lang/server_fr.ts
#finish with lrelease server_fr.ts (mettre le .qm dans le BUILD)
