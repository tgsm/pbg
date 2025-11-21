#ifndef PIGLET_ENTITIES_CENTITYPHIAL_H
#define PIGLET_ENTITIES_CENTITYPHIAL_H

#include "entities/CEntityMesh.h"

class CEntityPhial : public CEntityMesh {
public:
    U32 m_unkF4;
    U8 m_unkF8[0x10C - 0xF8];

public:
    CEntityPhial(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityPhial, 0x10C);

#endif
