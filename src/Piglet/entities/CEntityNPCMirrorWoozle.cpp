#include "entities/CEntityNPCMirrorWoozle.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

CEntityNPCMirrorWoozle::CEntityNPCMirrorWoozle(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCMirrorWoozle::~CEntityNPCMirrorWoozle() {

}

void CEntityNPCMirrorWoozle::Reset() {
    CEntityNPC::Reset();
}

// Incomplete
void CEntityNPCMirrorWoozle::UpdateDetectionBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 1) {
                m_animation_star_controller->Play("DETECT_VISUAL");
                UpdateAnimations(0.001f);
            } else if (m_unk15C == 1) {
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

void CEntityNPCMirrorWoozle::UpdateFightBehaviour(F32 a1) {
    if (m_unk1A8 == 102 || m_unk1A8 == 106 || m_unk1A8 == 107 || m_unk1A8 == 108 || m_unk1A8 == 103) {
        ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

        SDkMessage message;
        if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1)) {
            if (strcmp(message.unk20, "START_FIGHT") == 0) {
                m_unk2EC = TRUE;
            }
        }
    }

    switch (m_unk1A8) {
        case 101:
            srand((int)m_entity_manager->GetGame()->m_timer->GetTime());
            m_unk2E4 = 0.0f;
            m_unk2EC = FALSE;
            m_unk2E8 = 1;
            m_unk1A8 = 102;
            break;
        case 102: {
            CEntitySeqKey* key_sequence_entity = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence_entity == NULL) {
                break;
            }
            key_sequence_entity->DelFlag(1 << 0);
            m_unk2D8 = 0.0f;

            if (rand() / 32767.0f > 0.5f) {
                m_unk2DC = 2.0f;
            } else {
                m_unk2DC = 1.0f;
            }

            if (rand() / 32767.0f > 0.5f) {
                m_unk2DC = -m_unk2DC;
            }

            m_animation_star_controller->Play("MIRROR");
            UpdateAnimations(0.001f);

            m_unk1A8 = 106;
            break;
        }
        case 103: {
            CEntitySeqKey* key_sequence_entity = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence_entity != NULL) {
                key_sequence_entity->AddFlag(1 << 0);
            }

            if (m_unk2EC == TRUE) {
                PlayWalkAnim(0);
                UpdateAnimations(0.001f);
                m_unk2D4 = m_animation_star_controller->GetPlayingAnimationSpeed();
                m_unk1A8 = 104;
            }
            break;
        }
        case 104: {
            CEntitySeqKey* key_sequence_entity = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence_entity != NULL) {
                // Weird double negation
                F32 thing = -m_unk2E4;
                thing = -thing;
                key_sequence_entity->m_unk54 = thing;
            }

            m_animation_star_controller->SetPlayingAnimationSpeed(m_unk2D4);

            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 105;
                break;
            } else if (!m_animation_star_controller->IsPlayingAnimationLooped()) {
                if (!(m_unkF4 & (1 << 1)) || !(m_animation_star_controller->GetPlayingAnimationTime() / m_animation_star_controller->GetPlayingAnimationDuration() > 0.5f)) {
                    break;
                }
            }

            m_unk2D8 = 0.0f;

            if (rand() / 32767.0f > 0.5f) {
                m_unk2DC = 2.0f;
            } else {
                m_unk2DC = 1.0f;
            }
            if (rand() / 32767.0f > 0.5f) {
                m_unk2DC = -m_unk2DC;
            }

            m_animation_star_controller->Play("MIRROR");
            UpdateAnimations(0.001f);
            m_unk1A8 = 106;
            break;
        }
        case 106:
            if (m_animation_star_controller->IsPlayingAnimation("MIRROR")) {
                m_unk2E0 = m_animation_star_controller->GetPlayingAnimationDuration();
                if (m_animation_star_controller->GetPlayingAnimationTime() >= m_unk2E0 * 0.5f) {
                    m_unk1A8 = 107;
                }
            } else {
                m_animation_star_controller->Play("MIRROR");
                UpdateAnimations(0.001f);
            }
            break;
        case 107: {
            m_unk2D8 += a1;
            F32 angle;
            if (m_unk2D8 > 1.5f) {
                angle = 90.0f * m_unk2DC;
                m_unk1A8 = 108;
            } else {
                F32 unk = m_unk2D8 / 1.5f;
                F32 unk_3 = unk * unk * unk;
                F32 unk_4 = unk * (unk * unk) * unk;
                F32 unk_cos = cos(5.5f * unk_3 * 3.141592653f);
                angle = 90.0f * m_unk2DC * (1.0f - unk_cos / (1.0f + 40.0f * unk_4));
            }

            angle += m_unk2E4;
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->SetCamRollAngle(angle);

            CEntitySeqKey* sequence_key_entity = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (sequence_key_entity != NULL) {
                sequence_key_entity->m_unk54 = angle;
            }
            break;
        }
        case 108:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                if (m_unk2E8 == 0) {
                    PlayWalkAnim(0);
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 104;
                } else {
                    PlayWalkAnim(0);
                    UpdateAnimations(0.001f);
                    m_unk1A8 = 103;
                    m_unk2E8 = 0;
                }

                m_unk2E4 = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCamRollAngle();
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

void CEntityNPCMirrorWoozle::UpdateGrimaceBehaviour(F32) {
    switch (m_unk1A8) {
        case 101: {
            m_unk2E4 = 0.0f;
            m_unk2E8 = 1;

            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->SetCamRollAngle(m_unk2E4);

            CEntitySeqKey* key_sequence_entity = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence_entity != NULL) {
                key_sequence_entity->m_unk54 = -m_unk2E4;
            }

            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            break;
        }
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                if ((int)(m_unkF4 & (1 << 3)) != 0) {
                    m_animation_star_controller->Play("WAIT");
                } else {
                    m_animation_star_controller->Play("MIRROR", 0.0f, 1, 1);
                }

                UpdateAnimations(0.001f);
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                m_unk1A8 = 102;
            }
            break;
    }
}

void CEntityNPCMirrorWoozle::UpdateDeathBehaviour(F32) {

}

void CEntityNPCMirrorWoozle::Render(F32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCMirrorWoozle::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCMirrorWoozle::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCMirrorWoozle::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
