#include "entities/CEntityNPCJackpotEffalump.h"
#include "CGamePartIngame.h"
#include <iostream>

CEntityNPCJackpotEffalump::CEntityNPCJackpotEffalump(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCJackpotEffalump::~CEntityNPCJackpotEffalump() {

}

void CEntityNPCJackpotEffalump::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCJackpotEffalump::UpdateDetectionBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 1) {
                m_animation_star_controller->Play("DETECT_VISUAL");
                UpdateAnimations(0.001f);
            } else if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
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

                SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            }
            break;
        }
    }
}

void CEntityNPCJackpotEffalump::UpdateFightBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_unk1A8 = 102;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                m_animation_star_controller->Play("JACKPOT");
                UpdateAnimations(0.001f);
                m_unk1A8 = 103;
            }

            break;
        }
        case 103:
            if (m_animation_star_controller->IsPlayingAnimation("JACKPOT") == TRUE) {
                if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                    m_animation_star_controller->Play("LAUGH");
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 104;
                }
            } else {
                m_animation_star_controller->Play("JACKPOT");
                UpdateAnimations(0.001f);
            }
            break;
        case 104:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                m_unk1A8 = 105;
            }
            break;
        case 105:
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 106;
            }
            break;
        case 107:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                MakeNPCLaughing();
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 107;
            break;
    }
}

void CEntityNPCJackpotEffalump::UpdateGrimaceBehaviour(F32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("JACKPOT");
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
            }
            break;
    }
}

void CEntityNPCJackpotEffalump::UpdateDeathBehaviour(F32) {

}

void CEntityNPCJackpotEffalump::Render(F32 dt) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt);
    }
}

void CEntityNPCJackpotEffalump::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCJackpotEffalump::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCJackpotEffalump::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
