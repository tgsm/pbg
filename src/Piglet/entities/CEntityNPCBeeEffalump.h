#ifndef PIGLET_ENTITIES_CENTITYNPCBEEEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCBEEEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCBeeEffalump : public CEntityNPC {
public:
    u8 m_unk2D4[0x14];

public:
    CEntityNPCBeeEffalump(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCBeeEffalump, 0x2E8);

#endif
