#ifndef PIGLET_ENTITIES_CENTITYNPCJACKPOTEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCJACKPOTEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCJackpotEffalump : public CEntityNPC {
public:
    F32 m_animation_speed;

public:
    CEntityNPCJackpotEffalump(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCJackpotEffalump();

    virtual U32 GetType() { return ENTITY_NPC_JACKPOT_EFFALUMP; }
    virtual void Reset();
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
};
REQUIRE_SIZE(CEntityNPCJackpotEffalump, 0x2D8);

#endif
