#ifndef PIGLET_ENTITIES_CENTITYSEARCHABLEZONE_H
#define PIGLET_ENTITIES_CENTITYSEARCHABLEZONE_H

#include "entities/CEntityZone.h"

class CEntitySearchableZone : public CEntityZone {
public:
    U8 m_unk1BC[0x268 - 0x1BC];

public:
    CEntitySearchableZone(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntitySearchableZone, 0x268);

#endif
