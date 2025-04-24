#pragma once on

#include "types.h"

class CGamePart {
public:
    u32 m_unk0;
    u32 m_unk4;

public:
    CGamePart() {
        m_unk0 = 0;
    }
    virtual ~CGamePart();

    virtual u32 NextFrame() = 0;
};
REQUIRE_SIZE(CGamePart, 0xC);
