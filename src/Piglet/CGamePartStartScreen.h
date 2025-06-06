#ifndef PIGLET_CGAMEPARTSTARTSCREEN_H
#define PIGLET_CGAMEPARTSTARTSCREEN_H

#include "CGamePart.h"

class CGame;

class CGamePartStartScreen : public CGamePart {
private:
    CGame* m_game;
    F32 m_unk10;
    U32 m_unk14;

public:
    CGamePartStartScreen(CGame* game);
    virtual ~CGamePartStartScreen();

    virtual U32 NextFrame();
    virtual U32 NextFrameExit();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
};
REQUIRE_SIZE(CGamePartStartScreen, 0x18);

#endif
