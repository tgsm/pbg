#include "engine/sound/DkSound.h"
#include "engine/sound/CSound3D.h"
#include "entities/CEntityPhial.h"
#include "CGamePartIngame.h"
#include <iostream>

CEntityPhial::CEntityPhial(CEntityManager* entity_manager, std::string name) : CEntityMesh(entity_manager, name) {
    m_state = 0;
    m_unkF8 = NULL;
    m_unk108 = NULL;
    m_unkFC = 0;
}

CEntityPhial::~CEntityPhial() {
    if (m_unk108 != NULL) {
        m_unk108->StopFx();
        m_unk108 = NULL;
    }
}

void CEntityPhial::Reset() {
    CEntityMesh::Reset();
}

void CEntityPhial::Update(F32 dt) {
    if (m_state == 5 && m_unk108 != NULL) {
        int grm_state = m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->GetState();
        if (grm_state == 6 || grm_state == 7 || grm_state == 8) {
            m_unk108->StopFx();
            m_unk108 = NULL;
            m_unkFC &= ~(1 << 1);
            m_unk104 = 0.0f;
        }
    }

    if (IsFlagged(ENTITY_FLAG_UNK7) == TRUE) {
        if (m_unk108 != NULL) {
            m_unk108->StopFx();
            m_unk108 = NULL;
        }
    } else {
        if (m_unkFC & (1 << 1)) {
            m_unk104 += dt;
            if (m_unk104 >= 10000.0f) {
                CDKW_V3d position = GetPosition();
                m_unk108 = m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_002.xmd", 10000.0f, position, 1);
                m_unk104 = 0.0f;
            }
        }

        if (IsFlagged(ENTITY_FLAG_ACTIVE) == TRUE) {
            CEntityObject::UpdateContacts();
            switch (m_state) {
                case 0: {
                    m_animation_star_controller->Play("FALL", 1, 1);
                    CEntityMesh::Update(dt);
                    m_state = 1;
                    DelFlag(ENTITY_FLAG_VISIBLE);
                    CDKW_V3d position = GetPosition();
                    position.y += 2.5f;
                    m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_DNPC_01.xmd", 1.0f, position, 1);
                    m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_DNPC_02.xmd", 1.0f, position, 1);
                    m_unk100 = 0.1f;
                    break;
                }
                case 1:
                    m_unk100 -= dt;
                    if (m_unk100 <= 0.0f) {
                        if (m_unkF8 != NULL) {
                            m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unkF8->m_unk0, "PHIAL_APEARED", 0);
                        }
                        AddFlag(ENTITY_FLAG_VISIBLE);
                        m_unk100 = 0.0f;
                        m_state = 2;
                    }
                    break;
                case 2: {
                    SDkMessage message;
                    strcpy(message.type, "PHIAL_CANFALL");
                    if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1)) {
                        m_state = 3;
                    }
                    break;
                }
                case 3:
                    m_unk100 += dt;
                    if (m_unk100 > 0.2f) {
                        m_state = 4;
                    }
                    if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                        m_animation_star_controller->Play("WAITBASE");
                        m_state = 5;
                    }
                    break;
                case 4:
                    m_unk100 += dt;
                    if (m_unk100 > 0.5f && m_unk108 == NULL) {
                        CDKW_V3d position = GetPosition();
                        m_unk108 = m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_002.xmd", 10000.0f, position, 1);
                        m_unk104 = 0.0f;
                        m_unkFC |= (1 << 1);
                    } else if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                        m_animation_star_controller->Play("WAITBASE");
                        if (m_unk108 != NULL) {
                            m_state = 5;
                        }
                        GetPosition();
                    }
                    break;
                case 5:
                    if (m_unk108 == NULL) {
                        int grm_state = m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->GetState();
                        if (grm_state != 6 && grm_state != 7 && grm_state != 8) {
                            CDKW_V3d position = GetPosition();
                            m_unk108 = m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_002.xmd", 10000.0f, position, 1);
                            m_unk104 = 0.0f;
                            m_unkFC |= (1 << 1);
                        }
                    }
                    break;
                case 6:
                    if (m_unk108 != NULL) {
                        m_unk108->StopFx();
                        m_unk108 = NULL;
                    }
                    break;
            }

            if (m_clump != NULL) {
                m_clump->Update(dt);
                if (m_state != 2 && m_state != 1) {
                    CEntityMesh::Update(dt);
                }
                CEntityObject::UpdateVolumes();
            }
        }
    }
}

void CEntityPhial::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) != TRUE) {
        return;
    }
    if (m_clump == NULL) {
        return;
    }
    if (m_state == 6) {
        return;
    }

    CEntityMesh::Render(dt);
}

void CEntityPhial::ResolveContact(const DkPh::Collider::Body& body, int, int) {
    if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
        return;
    }
    if (!IsFlagged(ENTITY_FLAG_HAS_COLLISION)) {
        return;
    }
    if (m_state == 6) {
        return;
    }
    if (body.entity4->GetType() != ENTITY_PIGLET) {
        return;
    }
    if (m_state != 4 && m_state != 5) {
        return;
    }

    m_state = 6;

    if (m_unkF8 != NULL) {
        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_unkF8->m_unk0, "PHIAL_COLLECT", 0);
    }

    m_unkFC |= (1 << 0);

    CDKW_V3d position = GetPosition();
    m_entity_manager->GetGame()->m_fx_manager->AddFx("FX/FX_003.xmd", 1.0f, position, 1);

    std::string name = "SCAR_650_01_3D";
    DKSND::CSound3D* sound = DkSoundGetEngine()->PlaySound3D(&name, GetPosition(), 1);
    if (sound != NULL) {
        sound->SetLayer(1);
    }
    if (sound != NULL) {
        sound->SetLoopMode(0);
    }

    DelFlag(ENTITY_FLAG_VISIBLE);
}

void CEntityPhial::Parse(DkXmd::CChunkIterator iter) {
    CEntityMesh::Parse(iter);
}

void CEntityPhial::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {

}
