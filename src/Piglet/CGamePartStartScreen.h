#ifndef PIGLET_CGAMEPARTSTARTSCREEN_H
#define PIGLET_CGAMEPARTSTARTSCREEN_H

#include "CGamePart.h"

class CGame;

class CGamePartStartScreen : public CGamePart {
private:
    CGame* m_game;
    f32 m_unk10;
    u32 m_unk14;

public:
    CGamePartStartScreen(CGame* game);
    virtual ~CGamePartStartScreen();

    virtual void NextFrame();
    virtual u32 NextFrameExit();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
};
REQUIRE_SIZE(CGamePartStartScreen, 0x18);

#endif
