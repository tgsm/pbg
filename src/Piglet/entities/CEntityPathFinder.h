#ifndef PIGLET_ENTITIES_CENTITYPATHFINDER_H
#define PIGLET_ENTITIES_CENTITYPATHFINDER_H

#include "entities/CEntityMesh.h"

class CEntityPathFinder : public CEntityMesh {
public:
    U32 m_unkF4;
    U8 m_unkF8[0x15C - 0xF8];

public:
    CEntityPathFinder(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityPathFinder, 0x15C);

#endif
