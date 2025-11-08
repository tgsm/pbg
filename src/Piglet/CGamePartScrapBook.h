#ifndef PIGLET_CGAMEPARTSCRAPBOOK_H
#define PIGLET_CGAMEPARTSCRAPBOOK_H

#include "engine/display/CIm2DBatch.h"
#include "CGamePart.h"

class CGame;

class CGamePartScrapBook : public CGamePart {
public:
    CGame* m_game;
    F32 m_time;
    DKDSP::CIm2DBatch* m_batches[6];

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
