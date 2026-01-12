#include "entities/CEntityFX.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>
#include <math.h>

CEntityFX::CEntityFX(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_particle_emitter = NULL;
    m_unk60 = 0.0f;
    m_lifetime = -1.0f;

    AddFlag(ENTITY_FLAG_ACTIVE);
    DelFlag(ENTITY_FLAG_VISIBLE);

    m_radial_velocity = 0.0f;
    m_radius = 0.0f;
    m_unk64 = 0.0f;
}

CEntityFX::~CEntityFX() {
    if (m_particle_emitter != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveParticleEmitter(m_particle_emitter);
        m_particle_emitter = NULL;
    }
}

BOOL CEntityFX::ParseParticleEmitter(std::string filename) {
    DkXmd::CChunkIterator unused;
    m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(9, filename);

    m_particle_emitter = m_entity_manager->GetGame()->GetScene()->CreateParticleEmitterFromDefinition(m_unk0, m_entity_manager->GetGame()->GetObjectDictionary()->FindParticleEmitterDefinition(filename));

    return TRUE;
}

void CEntityFX::Parse(DkXmd::CChunkIterator iter) {
    char buf10c[256];
    char buf20c[256];
    DkXmd::CChunkIterator dest;
    DkXmd::CChunkIterator unused;

    CEntityObject::Parse(iter);

    if (iter.GetChunk("TexDict", dest)) {
        strcpy(buf10c, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(6, buf10c);
    }

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf20c, dest.GetName());

            if (strcmp(buf20c, "ParticleEmitter") == 0) {
                ParseParticleEmitter(dest);
            } else if (strcmp(buf20c, "ParticleEmitterFile") == 0) {
                strcpy(buf10c, dest.GetStringValue());
                ParseParticleEmitter(buf10c);
            } else if (strcmp(buf20c, "LifeTime") == 0) {
                m_lifetime = dest.GetFloatValue();
            } else if (strcmp(buf20c, "RadialVelocity") == 0) {
                m_radial_velocity = dest.GetFloatValue();
            } else if (strcmp(buf20c, "Radius") == 0) {
                m_radius = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityFX::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    DkXmd::CChunkIterator unused;
    CEntityObject::ParseBehavior(iter, behavior);
}

void CEntityFX::Init() {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Start();
            m_particle_emitter->Resume();
            CEntityObject::AddFlag(ENTITY_FLAG_ACTIVE);
        }
    } else {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Stop();
            m_unk60 = 0.0f;
            CEntityObject::DelFlag(ENTITY_FLAG_ACTIVE);
        }
    }

    m_unk44 = CDKW_V3d(m_particle_emitter->GetFrame()->m_rwframe->modelling.pos);
}

void CEntityFX::ManageMessage(SDkMessage& message) {
    CEntity::ManageMessage(message);

    if (strcmp(message.unk20, "START") == 0) {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Start();
            m_particle_emitter->Resume();
            CEntityObject::AddFlag(ENTITY_FLAG_ACTIVE);
        }
    } else if (strcmp(message.unk20, "STOP") == 0) {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Stop();
            m_unk60 = 0.0f;
            CEntityObject::DelFlag(ENTITY_FLAG_ACTIVE);
        }
    } else if (strcmp(message.unk20, "PAUSE") == 0) {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Pause();
        }
    } else if (strcmp(message.unk20, "RESUME") == 0) {
        if (m_particle_emitter != NULL) {
            m_particle_emitter->Resume();
        }
    }
}

void CEntityFX::Update(F32 dt) {
    CEntityObject::Update(dt);

    if (IsFlagged(ENTITY_FLAG_VISIBLE) != TRUE) {
        return;
    }

    if (m_particle_emitter == NULL) {
        return;
    }

    if (m_radial_velocity > 0.0f) {
        m_unk64 += dt;
        CDKW_V3d unk44 = m_unk44;
        F32 x = (F32)cos(3.1415927f * ((m_unk64 * m_radial_velocity) / 180.0f));
        unk44.x += m_radius * x;
        F32 z = (F32)sin(3.1415927f * ((m_unk64 * m_radial_velocity) / 180.0f));
        unk44.z += m_radius * z;
        SetPosition(unk44);
    }

    m_particle_emitter->GetParticleEmissionGap(); // unused result
    m_particle_emitter->Update(dt);

    if (!m_particle_emitter->IsPaused() && !m_particle_emitter->IsStopped() && m_lifetime > 0.0f) {
        m_unk60 += dt;
        if (m_unk60 > m_lifetime && m_particle_emitter != NULL) {
            m_particle_emitter->Stop();
            m_unk60 = 0.0f;
            CEntityObject::DelFlag(ENTITY_FLAG_ACTIVE);
        }
    }
}

void CEntityFX::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) != TRUE) {
        return;
    }

    CEntityObject::Render(dt);

    CGamePartIngame* game_part = (CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer();
    if (m_particle_emitter != NULL && !game_part->m_game_room_manager->IsOnFight()) {
        m_entity_manager->GetGame()->GetScene()->RenderParticleEmitter(m_particle_emitter, 1);
    }
}

void CEntityFX::DelFlag(U32 flag) {
    if ((flag & ENTITY_FLAG_ACTIVE) && m_particle_emitter != NULL) {
        m_particle_emitter->Stop();
        m_unk60 = 0.0f;
        CEntityObject::DelFlag(ENTITY_FLAG_ACTIVE);
    }

    CEntityObject::DelFlag(flag);
}

void CEntityFX::AddFlag(U32 flag) {
    if ((flag & ENTITY_FLAG_ACTIVE) && m_particle_emitter != NULL) {
        m_particle_emitter->Start();
        m_particle_emitter->Resume();
        CEntityObject::AddFlag(ENTITY_FLAG_ACTIVE);
    }

    CEntityObject::AddFlag(flag);
}
