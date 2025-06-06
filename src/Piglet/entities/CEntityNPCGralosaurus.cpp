#include "entities/CEntityNPCGralosaurus.h"
#include "entities/CEntityHero.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityNPCGralosaurus::CEntityNPCGralosaurus(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    m_unk2D8 = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk2E4 = CDKW_V3d(0.0f, 0.0f, 1.5f);
    m_unk2F0 = CDKW_V3d(0.0f, 0.0f, 0.0f);
}

CEntityNPCGralosaurus::~CEntityNPCGralosaurus() {

}

// Incomplete
void CEntityNPCGralosaurus::Init() {
    CEntityNPC::Init();

    // FIXME: Need CClump(?) vtable, and more

    m_unk2F0.m_y = 0.13f;
    m_unkF4 |= (1 << 14);
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
    m_unk2FC = 0;
}

void CEntityNPCGralosaurus::Reset() {
    CEntityMesh::Reset();
}

void CEntityNPCGralosaurus::UpdatePursuitBehaviour(F32 unk) {
    m_unk2FC = 0;
    m_unkF4 &= ~(1 << 15);

    switch (m_unk1A8) {
        case 8:
            m_unk1A8 = 0;
            break;
        case 5:
            m_animation_star_controller->Play("LAUGH", 0.0f, 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 10;
            m_rotation_angle = 0.0f;
            break;
        case 10:
            m_unkF4 &= ~(1 << 8);
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                if (m_unkF4 & (1 << 0)) {
                    m_animation_star_controller->Play("BLEND_NORMAL", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 1)) {
                    m_animation_star_controller->Play("BLEND_WORRIED", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 2)) {
                    m_animation_star_controller->Play("BLEND_FRIGHTENED", 0.0f, 1, 1);
                }
                UpdateAnimations(0.001f);
                m_unk1A8 = 6;
            }
            break;
        case 6:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_unk1A8 = 0;
                m_unkF4 &= ~(1 << 8);
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_unkF4 &= ~(1 << 8);

                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_entity_manager->GetHero()->m_unk0, "HERO_UNDETECT", 0);
            }
            break;
        case 0:
            if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_NORMAL") != 0 &&
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_WORRIED") != 0 &&
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_FRIGHTENED") != 0) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
            }
            if (!MoveAlongZ(unk) || (m_unkF4 & (1 << 8))) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            }
            break;
    }
}

void CEntityNPCGralosaurus::UpdateDetectionBehaviour(F32) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
}

void CEntityNPCGralosaurus::UpdateFightBehaviour(F32 unk) {
    SDkMessage message;
    if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
        m_unk2FC = 1;
    }

    switch (m_unk1A8) {
        case 101:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();
                if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_NORMAL") != 0 &&
                    strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_WORRIED") != 0 &&
                    strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_FRIGHTENED") != 0) {
                    if (m_unkF4 & (1 << 0)) {
                        m_animation_star_controller->Play("BLEND_NORMAL", 0.0f, 1, 1);
                    } else if (m_unkF4 & (1 << 1)) {
                        m_animation_star_controller->Play("BLEND_WORRIED", 0.0f, 1, 1);
                    } else if (m_unkF4 & (1 << 2)) {
                        m_animation_star_controller->Play("BLEND_FRIGHTENED", 0.0f, 1, 1);
                    }
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 102;
                } else {
                    m_unk1A8 = 102;
                }
                m_unkF4 &= ~(1 << 8);
            }
            break;
        case 102:
            if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "BLEND_NORMAL") == 0 ||
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "BLEND_WORRIED") == 0 ||
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "BLEND_FRIGHTENED") == 0 ||
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_NORMAL") == 0 ||
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_WORRIED") == 0 ||
                strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "WALK_FRIGHTENED") == 0) {
                if (m_animation_star_controller->IsPlayingAnimationLooped() && m_unk2FC != 0) {
                    PlayWalkAnim(0);
                    UpdateAnimations(0.001f);
                    m_unk2D4 = m_animation_star_controller->GetPlayingAnimationSpeed();
                    m_unk1A8 = 103;
                    m_unk2FC = 0;
                }
            } else {
                if (m_unkF4 & (1 << 0)) {
                    m_animation_star_controller->Play("BLEND_NORMAL", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 1)) {
                    m_animation_star_controller->Play("BLEND_WORRIED", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 2)) {
                    m_animation_star_controller->Play("BLEND_FRIGHTENED", 0.0f, 1, 1);
                }
                UpdateAnimations(0.001f);
            }
            break;
        case 103:
            if (!MoveAlongZ(unk) || (m_unkF4 & (1 << 8))) {
                m_unk1A8 = 104;
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            } else {
                std::string* unused = m_animation_star_controller->GetPlayingAnimationName();
            }
            break;
        case 105: {
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CGamePartIngame* game_part = (CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer();
                CEntityHero* hero = game_part->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                MakeNPCLaughing();
            }
            break;
        }
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 0.0f, 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 105;
            break;
    }
}

void CEntityNPCGralosaurus::UpdateGrimaceBehaviour(F32 unk) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 0.0f, 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 10;
            break;
        case 10:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                if (m_unkF4 & (1 << 0)) {
                    m_animation_star_controller->Play("BLEND_NORMAL", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 1)) {
                    m_animation_star_controller->Play("BLEND_WORRIED", 0.0f, 1, 1);
                } else if (m_unkF4 & (1 << 2)) {
                    m_animation_star_controller->Play("BLEND_FRIGHTENED", 0.0f, 1, 1);
                }
                UpdateAnimations(0.001f);
                m_unk1A8 = 102;
            }
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
            }
            break;
    }
}

void CEntityNPCGralosaurus::UpdateDeathBehaviour(F32) {

}

void CEntityNPCGralosaurus::Render(F32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCGralosaurus::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCGralosaurus::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCGralosaurus::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}

void CEntityNPCGralosaurus::PlayWalkAnim(int unk) {
    if (m_unkF4 & (1 << 0)) {
        m_animation_star_controller->Play("WALK_NORMAL", 0.0f, 1, 1);
    } else if (m_unkF4 & (1 << 1)) {
        m_animation_star_controller->Play("WALK_WORRIED", 0.0f, 1, 1);
    } else if (m_unkF4 & (1 << 2)) {
        m_animation_star_controller->Play("WALK_FRIGHTENED", 0.0f, 1, 1);
    }
}

void CEntityNPCGralosaurus::Idle() {
    if (!(m_unkF4 & (1 << 10))) {
        m_unk2C4 = m_animation_star_controller->GetPlayingAnimationName();
        if (m_unk2C4 != NULL) {
            strcmp(m_unk2C4->c_str(), "SLEEP");
        }
        m_unkF4 |= (1 << 10);
    }
}

void CEntityNPCGralosaurus::UnIdle(int) {
    if (m_unkF4 & (1 << 10)) {
        m_unkF4 &= ~(1 << 10);
    }
}
