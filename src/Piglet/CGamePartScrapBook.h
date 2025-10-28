#ifndef PIGLET_CGAMEPARTSCRAPBOOK_H
#define PIGLET_CGAMEPARTSCRAPBOOK_H

#include "CGamePart.h"

class CGame;

class CGamePartScrapBook : public CGamePart {
public:
    U8 m_unkC[0x20];

public:
    CGamePartScrapBook(CGame* game);
    virtual ~CGamePartScrapBook();

    virtual U32 NextFrame();
    virtual U32 NextFrameExit();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void RenderIcons(F32 dt_maybe);
    virtual void RenderBackGround(F32 dt_maybe);
    virtual BOOL IsVideoOpen(int);
};
REQUIRE_SIZE(CGamePartScrapBook, 0x2C);

#endif
