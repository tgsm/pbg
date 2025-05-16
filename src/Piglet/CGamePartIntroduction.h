#ifndef PIGLET_CGAMEPARTINTRODUCTION_H
#define PIGLET_CGAMEPARTINTRODUCTION_H

#include "CGamePart.h"

class CGamePartIntroduction : public CGamePart {
public:
    CGamePartIntroduction();
    virtual ~CGamePartIntroduction();

    virtual u32 NextFrame();
};
REQUIRE_SIZE(CGamePartIntroduction, 0xC);

#endif
