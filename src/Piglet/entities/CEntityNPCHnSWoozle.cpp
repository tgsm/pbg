#include "entities/CEntityNPCHnSWoozle.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <iostream>

CEntityNPCHnSWoozle::CEntityNPCHnSWoozle(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
    m_unk2D8 = FALSE;
    m_unk2DC = 0.0f;
}

CEntityNPCHnSWoozle::~CEntityNPCHnSWoozle() {

}

void CEntityNPCHnSWoozle::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCHnSWoozle::UpdateDetectionBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
                UpdateAnimations(0.001f);
            } else {
                m_animation_star_controller->Play("DETECT_VISUAL");
                UpdateAnimations(0.001f);
            }
            m_unk1A8 = 1;
            break;
        case 1: {
            CDKW_V3d at;
            at = m_clump->GetFrame()->m_rwframe->modelling.at;

            CDKW_V3d delta = GetPosDelta();
            ComputeRotationAngle(at, delta);

            if (strcmp(m_animation_star_controller->GetTargetAnimationName()->c_str(), "DETECT_VISUAL") == 0) {
                RotateAccordingToGarbageVar(a1);
            }

            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_unkF4 |= (1 << 13);
                if (m_unkF4 & (1 << 13)) {
                    PlayRunAnim(0);
                } else {
                    PlayWalkAnim(0);
                }
                UpdateAnimations(0.001f);

                SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            }
            break;
        }
    }
}

void CEntityNPCHnSWoozle::UpdateFightBehaviour(F32 a1) {
    if (m_unk1A8 == 105 || m_unk1A8 == 106 || m_unk1A8 == 107) {
        m_unk2DC += a1;
    } else {
        m_unk2DC = 0.0f;
    }

    if (m_unk2DC == 0.0f) {
        m_unk2D8 = FALSE;
    } else if (m_unk2DC > (7.0f / 3.0f)) {
        m_unk2D8 = FALSE;
    } else if (m_unk2DC > 0.2f) {
        m_unk2D8 = TRUE;
    }

    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                m_unk1A8 = 103;
            }

            break;
        }
        case 103:
            PlayWalkAnim(0);
            UpdateAnimations(0.001f);
            m_unk2D4 = m_animation_star_controller->GetPlayingAnimationSpeed();
            m_unk1A8 = 104;
            break;
        case 104:
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 108;
                break;
            }

            if (m_animation_star_controller->IsPlayingAnimationLooped() || (m_unkF4 & (1 << 1) && m_animation_star_controller->GetPlayingAnimationTime() / m_animation_star_controller->GetPlayingAnimationDuration() > 0.5f)) {
                m_animation_star_controller->Play("HIDE");
                UpdateAnimations(0.001f);
                m_unk1A8 = 105;
                m_unk2DC = 0.0f;
            }

            break;
        case 105:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("IS_HIDE");
                UpdateAnimations(0.001f);
                m_unk1A8 = 106;
            }
            break;
        case 106:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("END_HIDE");
                UpdateAnimations(0.001f);
                m_unk1A8 = 107;
            }
            break;
        case 107:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_unk1A8 = 104;
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 109;
            break;
        case 109:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                MakeNPCLaughing();
            }
            break;
    }
}

void CEntityNPCHnSWoozle::UpdateGrimaceBehaviour(F32) {
    if (!m_unk2D8) {
        switch (m_unk1A8) {
            case 101:
                m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
                UpdateAnimations(0.001f);
                m_unk1A8 = 102;
                break;
            case 102:
                if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                    m_animation_star_controller->Play("WAIT");
                    UpdateAnimations(0.001f);
                    m_unk2D8 = FALSE;
                    SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                    m_unk1A8 = 103;
                }
                break;
        }
    } else {
        std::string unused;

        switch (m_unk1A8) {
            case 101:
                if (m_unkF4 & (1 << 1)) {
                    m_unkF4 &= ~(1 << 1);
                    m_unkF4 |= (1 << 0);
                } else if (m_unkF4 & (1 << 2)) {
                    m_unkF4 &= ~(1 << 2);
                    m_unkF4 |= (1 << 1);
                } else if (m_unkF4 & (1 << 3)) {
                    m_unkF4 &= ~(1 << 3);
                    m_unkF4 |= (1 << 2);
                }

                m_animation_star_controller->Play("WAIT", 0.0f, 1, 1);
                UpdateAnimations(0.001f);
                m_animation_star_controller->Play("IS_HIDE", 0.0f, 1, 1);
                UpdateAnimations(0.001f);

                m_unk1A8 = 103;
                break;
            case 103:
                if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "IS_HIDE") == 0) {
                    if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                        m_animation_star_controller->Play("END_HIDE");
                        UpdateAnimations(0.001f);
                        m_unk1A8 = 104;
                    }
                } else {
                    m_animation_star_controller->Play("IS_HIDE", 0.0f, 1, 1);
                    UpdateAnimations(0.001f);
                }
                break;
            case 104:
                if (strcmp(m_animation_star_controller->GetPlayingAnimationName()->c_str(), "END_HIDE") == 0) {
                    if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                        m_animation_star_controller->Play("WAIT");
                        UpdateAnimations(0.001f);
                        m_unk2D8 = FALSE;
                        SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                        m_unk1A8 = 103;
                    }
                } else {
                    m_animation_star_controller->Play("END_HIDE");
                    UpdateAnimations(0.001f);
                }
                break;
        }
    }
}

void CEntityNPCHnSWoozle::UpdateDeathBehaviour(F32) {

}

void CEntityNPCHnSWoozle::Render(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCHnSWoozle::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCHnSWoozle::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCHnSWoozle::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
