#include "entities/CEntityNPCBasicWoozle.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <cstdlib>
#include <iostream>

CEntityNPCBasicWoozle::CEntityNPCBasicWoozle(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCBasicWoozle::~CEntityNPCBasicWoozle() {

}

void CEntityNPCBasicWoozle::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCBasicWoozle::UpdateDetectionBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
            } else {
                m_animation_star_controller->Play("DETECT_VISUAL");
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

void CEntityNPCBasicWoozle::UpdateFightBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("WAIT");
            m_unk1A8 = 102;
            m_unkF4 &= ~(1 << 8);
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                PlayWalkAnim(0);
                m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                m_unk1A8 = 103;
            }

            break;
        }
        case 103:
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                m_unk1A8 = 104;
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            }
            break;
        case 105:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                MakeNPCLaughing();
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            m_unk1A8 = 105;
            break;
    }
}

void CEntityNPCBasicWoozle::UpdateGrimaceBehaviour(F32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
            }
            break;
    }
}

void CEntityNPCBasicWoozle::UpdateDeathBehaviour(F32) {

}

void CEntityNPCBasicWoozle::Render(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCBasicWoozle::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCBasicWoozle::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCBasicWoozle::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
