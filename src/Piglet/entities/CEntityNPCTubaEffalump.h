#ifndef PIGLET_ENTITIES_CENTITYNPCTUBAEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCTUBAEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCTubaEffalump : public CEntityNPC {
public:
    u8 m_unk2D4[0xC];

public:
    CEntityNPCTubaEffalump(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCTubaEffalump, 0x2E0);

#endif
