#ifndef PIGLET_ENTITIES_CENTITYNPCTUBAEFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCTUBAEFFALUMP_H

#include "entities/CEntityNPC.h"

class CEntityNPCTubaEffalump : public CEntityNPC {
public:
    F32 m_animation_speed;
    F32 m_animation_duration;
    int m_unk2DC;

public:
    CEntityNPCTubaEffalump(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCTubaEffalump();

    virtual U32 GetType() { return ENTITY_NPC_TUBA_EFFALUMP; }
    virtual void Reset();
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Restore(void*);
    virtual void UpdateDetectionBehaviour(F32);
    virtual void UpdateFightBehaviour(F32);
    virtual void UpdateGrimaceBehaviour(F32);
    virtual void UpdateDeathBehaviour(F32);
    virtual void UpdateReturnBaseBehaviour(F32);
};
REQUIRE_SIZE(CEntityNPCTubaEffalump, 0x2E0);

#endif
