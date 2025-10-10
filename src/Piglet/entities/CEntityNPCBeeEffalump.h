#ifndef PIGLET_ENTITIES_CENTITYNPCBEEEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCBEEEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCBeeEffalump : public CEntityNPC {
public:
    F32 m_unk2D4;
    U8 m_unk2D8[0x10];

public:
    CEntityNPCBeeEffalump(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCBeeEffalump();

    virtual U32 GetType() { return ENTITY_NPC_BEE_EFFALUMP; }
    virtual void Reset();
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdatePursuitBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
};
REQUIRE_SIZE(CEntityNPCBeeEffalump, 0x2E8);

#endif
