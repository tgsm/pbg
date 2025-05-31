#ifndef PIGLET_ENTITIES_CENTITYNPCROADSWEEPEREFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCROADSWEEPEREFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCRoadSweeperEffalump : public CEntityNPC {
public:
    u8 m_unk2D4[0x1C];

public:
    CEntityNPCRoadSweeperEffalump(CEntityManager* entity_manager, std::string name);
};
REQUIRE_SIZE(CEntityNPCRoadSweeperEffalump, 0x2F0);

#endif
