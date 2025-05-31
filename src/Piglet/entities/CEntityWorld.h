#ifndef PIGLET_ENTITIES_CENTITYWORLD_H
#define PIGLET_ENTITIES_CENTITYWORLD_H

#include "entities/CEntityObject.h"

class CEntityWorld : public CEntityObject {
public:
    u8 m_unk40[0x10];

public:
    CEntityWorld(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityWorld, 0x50);

#endif
