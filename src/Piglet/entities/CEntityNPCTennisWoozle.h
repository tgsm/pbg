#ifndef PIGLET_ENTITIES_CENTITYNPCTENNISWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCTENNISWOOZLE_H

#include "entities/CEntityNPC.h"

class CEntityNPCTennisWoozle : public CEntityNPC {
public:
    U8 m_unk2D4[4];

public:
    CEntityNPCTennisWoozle(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCTennisWoozle, 0x2D8);

#endif
