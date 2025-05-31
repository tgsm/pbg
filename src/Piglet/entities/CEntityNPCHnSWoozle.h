#ifndef PIGLET_ENTITIES_CENTITYNPCHNSWOOZLE_H
#define PIGLET_ENTITIES_CENTITYNPCHNSWOOZLE_H

#include "entities/CEntityNPC.h"

class CEntityNPCHnSWoozle : public CEntityNPC {
public:
    u8 m_unk2D4[0xC];

public:
    CEntityNPCHnSWoozle(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCHnSWoozle, 0x2E0);

#endif
