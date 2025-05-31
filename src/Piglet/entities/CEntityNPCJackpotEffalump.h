#ifndef PIGLET_ENTITIES_CENTITYNPCJACKPOTEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCJACKPOTEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCJackpotEffalump : public CEntityNPC {
public:
    u8 m_unk2D4[4];

public:
    CEntityNPCJackpotEffalump(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCJackpotEffalump, 0x2D8);

#endif
