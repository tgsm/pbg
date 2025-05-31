#ifndef PIGLET_ENTITIES_CENTITYFX_H
#define PIGLET_ENTITIES_CENTITYFX_H

#include "entities/CEntityObject.h"

class CEntityFX : public CEntityObject {
public:
    u8 m_unk40[0x30];

public:
    CEntityFX(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityFX, 0x70);

#endif
