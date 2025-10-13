#ifndef PIGLET_ENTITIES_CENTITYNPCROADSWEEPEREFFALUMP_H
#define PIGLET_ENTITIES_CENTITYNPCROADSWEEPEREFFALUMP_H

#include "engine/display/CParticleEmitter.h"
#include "entities/CEntityNPC.h"

class CEntityNPCRoadSweeperEffalump : public CEntityNPC {
public:
    F32 m_animation_speed;
    BOOL m_unk2D8;
    BOOL m_unk2DC;
    int m_unk2E0;
    DKDSP::CParticleEmitter* m_arriving_particle_emitter;
    DKDSP::CParticleEmitter* m_dust_particle_emitter;
    F32 m_unk2EC;

public:
    CEntityNPCRoadSweeperEffalump(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityNPCRoadSweeperEffalump();

    virtual U32 GetType() { return ENTITY_NPC_ROAD_SWEEPER_EFFALUMP; }
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

    BOOL ParseParticleEmitter(std::string filename, DKDSP::IParticleEmitter** emitter);
};
REQUIRE_SIZE(CEntityNPCRoadSweeperEffalump, 0x2F0);

#endif
