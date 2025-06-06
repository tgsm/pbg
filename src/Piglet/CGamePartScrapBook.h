#ifndef PIGLET_CGAMEPARTSCRAPBOOK_H
#define PIGLET_CGAMEPARTSCRAPBOOK_H

#include "CGamePart.h"

class CGamePartScrapBook : public CGamePart {
public:
    virtual ~CGamePartScrapBook();

    virtual U32 NextFrame();
    virtual U32 NextFrameExit();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void RenderIcons(F32 dt_maybe);
    virtual void RenderBackGround(F32 dt_maybe);
    virtual BOOL IsVideoOpen(int);
};

#endif
