#include "entities/CEntityZone.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityZone::CEntityZone(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_position = CDKW_V3d::ZERO;
    m_unk4C = 0.0f;

    for (U32 i = 0; i < 16; i++) {
        strcpy(m_unk50[i], "None");
        m_unk150[i] = 0;
        m_unk160[i] = 0;
    }

    m_unk188.emitter = NULL;
    m_unk18C = 0.0f;
    m_unk190 = 0.0f;

    m_unk170.emitter = NULL;
    m_unk174 = 0.0f;
    m_unk178 = 0.0f;

    m_unk1A8 = CDKW_V3d::ZERO;
    m_unk1A0 = 0.0f;
    m_unk1A4 = 0.0f;
}

CEntityZone::~CEntityZone() {
    m_position = CDKW_V3d::ZERO;
    m_unk4C = 0.0f;

    if (m_unk170.emitter != NULL) {
        m_entity_manager->GetGame()->m_scene->RemoveParticleEmitter(m_unk170.emitter);
        m_unk170.emitter = NULL;
    }
    if (m_unk188.emitter != NULL) {
        m_entity_manager->GetGame()->m_scene->RemoveParticleEmitter(m_unk188.emitter);
        m_unk188.emitter = NULL;
    }
}

void CEntityZone::Reset() {
    CEntityObject::Reset();
}

void CEntityZone::ResolveContact(const DkPh::Collider::Body& body, int, int) {
    CEntity* body_entity = body.entity4;
    BOOL bVar1;
    U32 i;
    U32 unused_type = body_entity->GetType();

    if (body_entity->GetType() >= ENTITY_MESH || body_entity->GetType() == ENTITY_ZONE) {
        m_entity_manager->GetGame()->GetGamePartPointer();

        bVar1 = FALSE;
        for (i = 0; i < 16; i++) {
            if (m_unk160[i] == 1) {
                if (strcmp(m_unk50[i], body_entity->m_unk0.c_str()) == 0) {
                    m_unk150[i] = 1;
                    bVar1 = TRUE;
                    break;
                }
            }
        }

        if (!bVar1) {
            for (i = 0; i < 16; i++) {
                if (m_unk150[i] == 0 && m_unk160[i] == 0) {
                    strcpy(m_unk50[i], body_entity->m_unk0.c_str());
                    m_unk150[i] = 1;
                    m_entity_manager->GetGame()->GetMailbox()->SendMessage(body_entity->m_unk0, m_unk0, "ENTER", 0);
                    break;
                }
            }
        }
    }
}

void CEntityZone::Render(F32 dt) {
    CEntityObject::Render(dt);

    if (!IsFlagged(1 << 1)) {
        return;
    }

    CGamePartIngame* game_part = (CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer();

    if (IsFlagged(1 << 0) != TRUE) {
        return;
    }

    if (!game_part->m_game_room_manager->IsPlayingRTC() && !game_part->m_game_room_manager->IsOnFight()) {
        if (m_unk170.emitter != NULL) {
            m_entity_manager->GetGame()->m_scene->RenderParticleEmitter(m_unk170.emitter, 1);
        }
        if (m_unk188.emitter != NULL) {
            m_entity_manager->GetGame()->m_scene->RenderParticleEmitter(m_unk188.emitter, 1);
        }
    }
}

// Incomplete
void CEntityZone::Parse(DkXmd::CChunkIterator iter) {
    char buf_98[128];
    char buf_118[128];
    DkXmd::CChunkIterator dest1, dest2;
    m_unk1B8 = 0;

    CEntityObject::Parse(iter);

    if (iter.GetChunk("TexDict", dest1)) {
        strcpy(buf_98, dest1.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(6, buf_98);
    }

    if (iter.GetFirstChildChunk(dest1) == TRUE) {
        do {
            strcpy(buf_118, dest1.GetName());

            if (strcmp(buf_118, "ZoneFx") == 0) {
                ParseZoneFx(dest1);
            } else if (strcmp(buf_118, "RejectNPC") == 0) {
                m_unk1B8 = dest1.GetS32Value();

                for (DkPh::BVolume** iter = m_unk24.begin(); iter < m_unk24.end(); iter++) {
                    m_entity_manager->GetUnkCollider()->GetBodyRef(*iter).unk34 = 0xFFFF0001;
                }
            }
        } while (dest1.GetNextSiblingChunk(dest1) == TRUE);
    }
}

BOOL CEntityZone::ParseParticleEmitter(std::string filename, SFX* fx) {
    m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(9, filename);

    DKDSP::CParticleEmitterDefinition* definition = m_entity_manager->GetGame()->m_object_dictionary->FindParticleEmitterDefinition(filename);
    // FIXME: Remove this cast
    fx->emitter = m_entity_manager->GetGame()->m_scene->CreateParticleEmitterFromDefinition(m_unk0, (DKDSP::IParticleEmitterDefinition*)definition);

    return TRUE;
}

void CEntityZone::ManageMessage(SDkMessage& message) {
    CEntity::ManageMessage(message);

    if (strcmp(message.unk0, "Piglet") == 0 || strcmp(message.unk0, "Tigger") == 0 || strcmp(message.unk0, "Winnie") == 0) {
        if (strcmp(message.unk20, "ENTER") == 0) {
            if (m_unk170.emitter != NULL) {
                m_unk170.emitter->Start();
            }
            m_unk1B4 = 0.0f;
        } else if (strcmp(message.unk20, "EXIT") == 0) {
            if (m_unk170.emitter != NULL) {
                m_unk170.emitter->Stop();
            }
        }
    }

    if (strcmp(message.unk20, "START_FX_IN") == 0) {
        m_unk170.emitter->Start();
    } else if (strcmp(message.unk20, "STOP_FX_IN") == 0) {
        m_unk170.emitter->Stop();
    } else if (strcmp(message.unk20, "PAUSE_FX_IN") == 0) {
        m_unk170.emitter->Pause();
    } else if (strcmp(message.unk20, "RESUME_FX_IN") == 0) {
        m_unk170.emitter->Resume();
    } else if (strcmp(message.unk20, "START_FX_OUT") == 0) {
        m_unk188.emitter->Start();
    } else if (strcmp(message.unk20, "STOP_FX_OUT") == 0) {
        m_unk188.emitter->Stop();
    } else if (strcmp(message.unk20, "PAUSE_FX_OUT") == 0) {
        m_unk188.emitter->Pause();
    } else if (strcmp(message.unk20, "RESUME_FX_OUT") == 0) {
        m_unk188.emitter->Resume();
    }
}

void CEntityZone::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityObject::ParseBehavior(iter, behavior);
}

void CEntityZone::DelFlag(U32 flag) {
    if (flag & (1 << 0)) {
        if (m_unk170.emitter != NULL) {
            m_unk170.emitter->Stop();
        }
        if (m_unk188.emitter != NULL) {
            m_unk188.emitter->Stop();
        }
    }

    CEntityObject::DelFlag(flag);
}

void CEntityZone::AddFlag(U32 flag) {
    if (flag & (1 << 0)) {
        CEntityObject::AddFlag(1 << 1);
        if (m_unk188.emitter != NULL) {
            m_unk188.emitter->Start();
        }
    }

    CEntityObject::AddFlag(flag);
}

BOOL CEntityZone::ShouldKick() {
    if (!IsFlagged(1 << 0)) {
        return TRUE;
    }

    if (m_unk170.emitter != NULL) {
        return m_unk170.emitter->IsInert();
    }

    return TRUE;
}
