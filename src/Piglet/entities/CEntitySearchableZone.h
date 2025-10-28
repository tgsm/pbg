#ifndef PIGLET_ENTITIES_CENTITYSEARCHABLEZONE_H
#define PIGLET_ENTITIES_CENTITYSEARCHABLEZONE_H

#include "entities/CEntityZone.h"

class CEntitySearchableZone : public CEntityZone {
public:
    U8 m_unk1BC[4];
    int m_generated_entity_number;
    U8 m_unk1C4[0x21C - 0x1C4];
    int m_unk21C;
    U8 m_unk220[0x268 - 0x220];

public:
    CEntitySearchableZone(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntitySearchableZone, 0x268);

#endif
