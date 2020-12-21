#ifndef SERVNAV_H
#define SERVNAV_H

#include "src/common/WidgetNav.h"

#include "src/server/connection/Server.h"
#include "src/server/game/GameServer.h"

namespace ServNav {
    Server *findServer() { QWidget *w = WidgetNav::find("Server"); return w ? (Server*)w : nullptr; }
    GameServer *findGameServer() { QWidget *w = WidgetNav::find("GameServer"); return w ? (GameServer*)w : nullptr; }
};

#endif // SERVNAV_H
