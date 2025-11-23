#include "entities/CEntityNPCTennisWoozle.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include "engine/input/IInputEngine.h"
#include "entities/CEntityHero.h"
#include "entities/CEntitySeqKey.h"
#include <cstring>
#include <iostream>

CEntityNPCTennisWoozle::CEntityNPCTennisWoozle(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCTennisWoozle::~CEntityNPCTennisWoozle() {

}

void CEntityNPCTennisWoozle::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCTennisWoozle::UpdateDetectionBehaviour(F32 a1) {
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
                PlayRunAnim(0);
                UpdateAnimations(0.001f);

                SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            }
            break;
        }
    }
}

static inline F32 DistanceSquared(const CDKW_V3d& vec) {
    return (F32)(F64)vec.m_x * (F32)(F64)vec.m_x + (F32)(F64)vec.m_y * (F32)(F64)vec.m_y + (F32)(F64)vec.m_z * (F32)(F64)vec.m_z;
}

static inline F32 DistanceSquared(F64 x, F64 y, F64 z) {
    return (F32)x * (F32)x + (F32)y * (F32)y + (F32)z * (F32)z;
}

// Incomplete/equivalent: stack offsets
void CEntityNPCTennisWoozle::UpdateFightBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 108;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                m_unk1A8 = 104;
            }

            break;
        }
        case 104: {
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 105;
                break;
            } else {
                CDKW_V3d delta = GetPosDelta2();
                volatile CDKW_V3d unused_probably_fake = delta;
                if (DistanceSquared(delta) > 12.0f) {
                    if (m_animation_star_controller->GetPlayingAnimationTime() / m_animation_star_controller->GetPlayingAnimationDuration() > 0.5f) {
                        m_animation_star_controller->Play("TENNIS");
                        UpdateAnimations(0.001f);
                        m_unk1A8 = 106;
                    }
                }
            }

            break;
        }
        case 106:
            if (m_animation_star_controller->GetPlayingAnimationTime() / m_animation_star_controller->GetPlayingAnimationDuration() > 0.65f) {
                CEntitySeqKey* key_sequence = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
                if (key_sequence != NULL) {
                    key_sequence->RotateOneRandomKey();
                    DKI::IInputEngine::GetDevice(0)->SendVibration(100);
                }
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
        case 108:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);
                MakeNPCLaughing();
            }
            break;
    }
}

void CEntityNPCTennisWoozle::UpdateGrimaceBehaviour(F32) {
    switch (m_unk1A8) {
        case 101: {
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        }
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("RUN_NORMAL");
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 104;
            }
            break;
    }
}

void CEntityNPCTennisWoozle::UpdateDeathBehaviour(F32) {

}

void CEntityNPCTennisWoozle::Render(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCTennisWoozle::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCTennisWoozle::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCTennisWoozle::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
