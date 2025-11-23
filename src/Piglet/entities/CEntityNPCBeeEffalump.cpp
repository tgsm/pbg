#include "entities/CEntityNPCBeeEffalump.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <iostream>

CEntityNPCBeeEffalump::CEntityNPCBeeEffalump(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCBeeEffalump::~CEntityNPCBeeEffalump() {

}

void CEntityNPCBeeEffalump::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCBeeEffalump::UpdateDetectionBehaviour(F32 a1) {
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
            CDKW_V3d at = CDKW_V3d(m_clump->GetFrame()->m_rwframe->modelling.at);

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

                SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            }
            break;
        }
    }
}

void CEntityNPCBeeEffalump::UpdatePursuitBehaviour(F32 a1) {
    CEntityNPC::UpdatePursuitBehaviour(a1);
}

void CEntityNPCBeeEffalump::UpdateFightBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_unk1A8 = 102;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_unk1A8 = 103;
            }

            break;
        }
        case 103:
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 104;
            }
            break;
        case 107:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                MakeNPCLaughing();
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 107;
            break;

        // What cases did they put here?
        // 104, 105, and 106 work, but not all three at the same time
        case 104:
        case 105:
            break;
    }
}

// Very incomplete
void CEntityNPCBeeEffalump::UpdateGrimaceBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            m_pig_spline->SwapMovingDirection();
            m_unk2D4 = 0.0f;
            m_unk1A8 = 105;

            break;
        case 105:
            m_unk2D4 += a1;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
            }
            break;
        case 106:
            m_unk2D4 += a1;
            if (m_unk2D4 < 1.0f && m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_pig_spline->SwapMovingDirection();
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
            }
            break;
    }
}

void CEntityNPCBeeEffalump::UpdateDeathBehaviour(F32) {

}

void CEntityNPCBeeEffalump::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt);
    }
}

void CEntityNPCBeeEffalump::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCBeeEffalump::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCBeeEffalump::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
