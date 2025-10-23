#ifndef PIGLET_CGAMEPARTMOVIECLIPS_H
#define PIGLET_CGAMEPARTMOVIECLIPS_H

#include "CGamePart.h"
#include "engine/display/CIm2DBatch.h"

class CGame;

class CGamePartMovieClips : public CGamePart {
public:
    CGame* m_game;
    F32 m_time;
    DKDSP::CIm2DBatch* m_batches[6];

public:
    CGamePartMovieClips(CGame* game);
    virtual ~CGamePartMovieClips();

    virtual U32 NextFrame();
    virtual U32 NextFrameExit();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void RenderIcons(F32 dt);
    virtual void RenderBackGround(F32 dt);
    virtual BOOL IsVideoOpen(int);
};
REQUIRE_SIZE(CGamePartMovieClips, 0x2C);

#endif
