#ifndef PIGLET_CGAMEPART_H
#define PIGLET_CGAMEPART_H

#include "types.h"

class CGamePart {
public:
    int m_unk0;
    int m_unk4;

public:
    CGamePart() {
        m_unk0 = 0;
    }
    virtual ~CGamePart();

    virtual U32 NextFrame() = 0;
};
REQUIRE_SIZE(CGamePart, 0xC);

#endif
