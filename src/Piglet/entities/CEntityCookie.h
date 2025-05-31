#ifndef PIGLET_ENTITIES_CENTITYCOOKIE_H
#define PIGLET_ENTITIES_CENTITYCOOKIE_H

#include "entities/CEntityMesh.h"

class CEntityCookie : public CEntityMesh {
public:
    u8 m_unkF4[0x1C4 - 0xF4];

public:
    CEntityCookie(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityCookie, 0x1C4);

#endif
