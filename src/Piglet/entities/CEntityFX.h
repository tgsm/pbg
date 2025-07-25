#ifndef PIGLET_ENTITIES_CENTITYFX_H
#define PIGLET_ENTITIES_CENTITYFX_H

#include <engine/display/CParticleEmitter.h>
#include "entities/CEntityObject.h"

class CEntityFX : public CEntityObject {
public:
    DKDSP::CParticleEmitter* m_particle_emitter;
    U8 m_unk44[0x50 - 0x44];
    CDKW_V3d m_position;
    F32 m_lifetime;
    F32 m_unk60;
    F32 m_unk64;
    F32 m_radius;
    F32 m_radial_velocity;

public:
    CEntityFX(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityFX();

    virtual U32 GetType() { return ENTITY_FX; }
    virtual CDKW_V3d GetPosition() { return m_position; }
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void Init();
    virtual void ManageMessage(SDkMessage& message);
    virtual void Render(F32 dt);
    virtual void DelFlag(U32 flag);
    virtual void AddFlag(U32 flag);

    BOOL ParseParticleEmitter(std::string filename);
    void ParseParticleEmitter(DkXmd::CChunkIterator iter) {
        if (m_particle_emitter == NULL) {
            m_particle_emitter = m_entity_manager->GetGame()->GetScene()->LoadParticleEmitter(m_unk0, &iter);
        } else {
            m_particle_emitter->LoadParameters(&iter);
        }
    }
};
REQUIRE_SIZE(CEntityFX, 0x70);

#endif
