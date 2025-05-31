#ifndef PIGLET_ENTITIES_CENTITYPATHFINDER_H
#define PIGLET_ENTITIES_CENTITYPATHFINDER_H

#include "entities/CEntityMesh.h"

class CEntityPathFinder : public CEntityMesh {
public:
    u8 m_unkF4[0x15C - 0xF4];

public:
    CEntityPathFinder(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityPathFinder, 0x15C);

#endif
