#ifndef PIGLET_ENTITIES_CENTITYLIGHT_H
#define PIGLET_ENTITIES_CENTITYLIGHT_H

#include "entities/CEntityObject.h"

class CEntityLight : public CEntityObject {
public:
    u8 m_unk40[0x10];

public:
    CEntityLight(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityLight, 0x50);

#endif
