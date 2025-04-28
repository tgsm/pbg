#pragma once

#include "entities/CEntityMesh.h"

// TODO
class CEntityHero : public CEntityMesh {
private:
    u8 m_unkF4[0x260 - 0xF4];
};
REQUIRE_SIZE(CEntityHero, 0x260);
