#include "engine/input/IInputEngine.h"
#include "entities/CEntityNPCTubaEffalump.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityNPCTubaEffalump::CEntityNPCTubaEffalump(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCTubaEffalump::~CEntityNPCTubaEffalump() {

}

void CEntityNPCTubaEffalump::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCTubaEffalump::UpdateDetectionBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 0:
            m_animation_star_controller->Play("DETECT_VISUAL");
            UpdateAnimations(0.001f);
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
        case 2:
            if (!m_animation_star_controller->IsPlayingAnimationLooped()) {
                break;
            }

            if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
                UpdateAnimations(0.001f);
            } else if (m_unk15C == 1) {
                m_animation_star_controller->Play("DETECT_VISUAL");
                UpdateAnimations(0.001f);
            } else {
                m_animation_star_controller->Play("FALL_ASLEEP");
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
                m_unk1A8 = 3;
                break;
            }

            m_unk1A8 = 1;
            break;
    }
}

void CEntityNPCTubaEffalump::UpdateFightBehaviour(F32 a1) {
    CEntitySeqKey* key_sequence;
    CGameRoomManager* manager;

    switch (m_unk1A8) {
        case 101: {
            m_unk2DC = 0;
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        }
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 107;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                m_unk1A8 = 103;
            }

            break;
        }
        case 103:
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 104;
            } else {
                if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                    m_unk2DC++;
                    if (m_unk2DC < 3) {
                        break;
                    }
                    m_animation_star_controller->Play("TUBA");
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 106;
                    m_unk2DC = 0;
                }
            }
            break;
        case 106: {
            if (m_animation_star_controller->IsPlayingAnimation("TUBA")) {
                m_animation_duration = m_animation_star_controller->GetPlayingAnimationDuration();
                if (m_animation_star_controller->GetPlayingAnimationTime() >= m_animation_duration * 0.35f) {
                    manager = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager;
                    key_sequence = manager->m_key_sequence_entity;
                    if (key_sequence != NULL) {
                        DKI::IInputEngine::GetDevice(0)->SendVibration(0xFF);
                        key_sequence->Explode();
                        key_sequence->m_unk40 |= (1 << 1);
                    }
                    manager->m_unk0 |= (1 << 23);
                    manager->m_unk118 = 0.7f;
                    manager->m_unk11C = 0.0f;
                    m_unk1A8 = 105;
                }
            }
            break;
        }
        case 105:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                manager = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager;
                key_sequence = manager->m_key_sequence_entity;
                if (key_sequence != NULL) {
                    key_sequence->UnExplode();
                    key_sequence->m_unk40 &= ~(1 << 1);
                }
                m_unk1A8 = 103;
            }
            break;
        case 107:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);

                MakeNPCLaughing();
            }
            break;
    }
}

void CEntityNPCTubaEffalump::UpdateGrimaceBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101: {
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        }
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 103;
                m_unk2DC = 0;
            }
            break;
    }
}

void CEntityNPCTubaEffalump::UpdateDeathBehaviour(F32) {

}

void CEntityNPCTubaEffalump::UpdateReturnBaseBehaviour(F32) {
    if (m_unkFC != 0) {
        if (m_unkF4 & (1 << 17)) {
            SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            return;
        }
        SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
        m_unk1A8 = m_unk1AC;
        m_unk1A9 = m_unk1AD;
        if (m_unk1A8 == 3) {
            m_animation_star_controller->Play("WAIT");
        } else {
            PlayWalkAnim(0);
        }
        return;
    }

    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("FALL_ASLEEP");
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                m_animation_star_controller->Play("SLEEP");
                SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
                m_unk1A8 = 3;
            }
            break;
    }
}

void CEntityNPCTubaEffalump::Render(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCTubaEffalump::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCTubaEffalump::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCTubaEffalump::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
