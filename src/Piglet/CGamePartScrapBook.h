#ifndef PIGLET_CGAMEPARTSCRAPBOOK_H
#define PIGLET_CGAMEPARTSCRAPBOOK_H

#include "CGamePart.h"

class CGamePartScrapBook : public CGamePart {
public:
    virtual ~CGamePartScrapBook();

    virtual u32 NextFrame();
    virtual u32 NextFrameExit();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void RenderIcons(f32 dt_maybe);
    virtual void RenderBackGround(f32 dt_maybe);
    virtual BOOL IsVideoOpen(int);
};

#endif
