#pragma once on

#include "entities/CEntityHero.h"

class CEntityPiglet : public CEntityHero {
public:
    u8 m_unk260[0x27C - 0x260];
    u32 m_unk27C;
    u8 m_unk280[0x32C - 0x280];
};
REQUIRE_SIZE(CEntityPiglet, 0x32C);
