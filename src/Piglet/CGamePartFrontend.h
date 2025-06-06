#ifndef PIGLET_CGAMEPARTFRONTEND_H
#define PIGLET_CGAMEPARTFRONTEND_H

#include "CGamePart.h"

class CGame;

class CGamePartFrontend : public CGamePart {
public:
    CGamePartFrontend(CGame* game);
    ~CGamePartFrontend();

    virtual U32 NextFrame();
};
REQUIRE_SIZE(CGamePartFrontend, 0xC);

#endif
