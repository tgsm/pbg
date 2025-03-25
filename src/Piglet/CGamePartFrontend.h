#pragma once

#include "CGamePart.h"

class CGame;

class CGamePartFrontend : public CGamePart {
public:
    CGamePartFrontend(CGame* game);
    ~CGamePartFrontend();

    virtual u32 NextFrame();
};
