#ifndef PIGLET_ENTITIES_CENTITYPHIAL_H
#define PIGLET_ENTITIES_CENTITYPHIAL_H

#include "entities/CEntityMesh.h"

class CEntityPhial : public CEntityMesh {
public:
    u8 m_unkF4[0x18];

public:
    CEntityPhial(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityPhial, 0x10C);

#endif
