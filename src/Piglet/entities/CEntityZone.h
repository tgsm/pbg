#ifndef PIGLET_ENTITIES_CENTITYZONE_H
#define PIGLET_ENTITIES_CENTITYZONE_H

#include "entities/CEntityObject.h"

class CEntityZone : public CEntityObject {
public:
    u8 m_unk40[0x1BC - 0x40];

public:
    CEntityZone(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityZone, 0x1BC);

#endif
