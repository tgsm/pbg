#ifndef PIGLET_CGAMEPARTDMROOMLAUNCHER_H
#define PIGLET_CGAMEPARTDMROOMLAUNCHER_H

#include "CGamePart.h"
#include "CDebugMenu.h"

class CGame;

class CGamePartDMRoomLauncher : public CGamePart {
private:
    CGame* m_game;
    CDebugMenu* m_debug_menu;

public:
    CGamePartDMRoomLauncher(CGame* game);
    virtual ~CGamePartDMRoomLauncher();

    virtual u32 NextFrame();
};
REQUIRE_SIZE(CGamePartDMRoomLauncher, 0x14);

#endif
