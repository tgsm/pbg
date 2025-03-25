#pragma once

#include "CGamePart.h"

class CGamePartIntroduction : public CGamePart {
public:
    CGamePartIntroduction();
    virtual ~CGamePartIntroduction();

    virtual u32 NextFrame();
};
