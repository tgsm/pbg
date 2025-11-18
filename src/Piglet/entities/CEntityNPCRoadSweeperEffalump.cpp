#include "engine/display/CParticleEmitterDefinition.h"
#include "entities/CEntityNPCRoadSweeperEffalump.h"
#include "entities/CEntitySeqKey.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <cstring>
#include <iostream>

CEntityNPCRoadSweeperEffalump::CEntityNPCRoadSweeperEffalump(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
    m_arriving_particle_emitter = NULL;
    m_dust_particle_emitter = NULL;
}

CEntityNPCRoadSweeperEffalump::~CEntityNPCRoadSweeperEffalump() {
    if (m_arriving_particle_emitter != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveParticleEmitter(m_arriving_particle_emitter);
        m_arriving_particle_emitter = NULL;
    }
    if (m_dust_particle_emitter != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveParticleEmitter(m_dust_particle_emitter);
        m_dust_particle_emitter = NULL;
    }
}

void CEntityNPCRoadSweeperEffalump::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCRoadSweeperEffalump::UpdateDetectionBehaviour(F32 a1) {
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

                SetGenericBehaviour(GENERIC_BEHAVIOUR_2);
            }
            break;
        }
    }
}

void CEntityNPCRoadSweeperEffalump::UpdateFightBehaviour(F32 a1) {
    SDkMessage message;

    // BUG: Checks for 106 twice.
    if (m_unk1A8 == 106 || m_unk1A8 == 102 || m_unk1A8 == 106 || m_unk1A8 == 103) {
        ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

        if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
            m_unk2DC = TRUE;
        }
    }

    switch (m_unk1A8) {
        case 101: {
            m_unk2E0 = 0;
            m_animation_star_controller->Play("WAIT");
            UpdateAnimations(0.001f);
            m_unk2D8 = TRUE;
            m_unk2DC = FALSE;

            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1) && strcmp(message.unk20, "START_FIGHT") == 0) {
                m_unk2DC = TRUE;
            }

            m_unk1A8 = 103;
            break;
        }
        case 103: {
            CEntitySeqKey* key_sequence = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence != NULL && key_sequence->m_unk148E8 == 1) {
                m_animation_star_controller->Play("ROADSWEEPER");
                UpdateAnimations(0.001f);
                key_sequence->CreateRoadSweeperArrivingFX(this, m_arriving_particle_emitter);
                key_sequence->m_unk148E4 = 0.0f;
                m_unk1A8 = 106;
                m_unk2EC = 0.0f;
            }
            break;
        }
        case 102:
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (m_unk2DC == TRUE) {
                m_unk1A8 = 104;
            }
            break;
        case 104:
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 105;
            } else {
                if (m_animation_star_controller->IsPlayingAnimation("WALK_NORMAL") ||
                    m_animation_star_controller->IsPlayingAnimation("WALK_WORRIED") ||
                    m_animation_star_controller->IsPlayingAnimation("WALK_FRIGHTENED")) {
                    if (m_animation_star_controller->IsPlayingAnimationLooped() || (m_unkF4 & (1 << 1) && m_animation_star_controller->GetPlayingAnimationTime() / m_animation_star_controller->GetPlayingAnimationDuration() > 0.5f)) {
                        m_unk2E0++;
                        if (m_unk2E0 % 2 == 0) {
                            m_animation_star_controller->Play("ROADSWEEPER");
                            UpdateAnimations(0.001f);
                            m_unk1A8 = 106;

                            CEntitySeqKey* key_sequence = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
                            if (key_sequence != NULL) {
                                key_sequence->CreateRoadSweeperArrivingFX(this, m_arriving_particle_emitter);
                                key_sequence->m_unk148E4 = 0.0f;
                                m_unk2EC = 0.0f;
                            }
                        }
                    }
                }
            }
            break;
        case 106: {
            CEntitySeqKey* key_sequence = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence != NULL) {
                F32 unk2EC = m_unk2EC;
                m_unk2EC += a1;
                if (unk2EC < 0.5f && m_unk2EC >= 0.5f) {
                    key_sequence->CreateRoadSweeperDustFX(this, m_dust_particle_emitter);
                }
            }

            if (m_animation_star_controller->IsPlayingAnimation("ROADSWEEPER")) {
                if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                    ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer()); // unused

                    if (m_unk2D8 == TRUE) {
                        m_unk2D8 = FALSE;
                    }
                    PlayWalkAnim(0);
                    UpdateAnimations(0.001f);

                    m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                    m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);
                    m_unk1A8 = 102;
                }
            } else {
                m_animation_star_controller->Play("ROADSWEEPER", 1, 1);
                UpdateAnimations(0.001f);
            }
            break;
        }
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

void CEntityNPCRoadSweeperEffalump::UpdateGrimaceBehaviour(F32 a1) {
    switch (m_unk1A8) {
        case 101: {
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            UpdateAnimations(0.001f);
            m_unk1A8 = 102;
            CEntitySeqKey* key_sequence = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->m_key_sequence_entity;
            if (key_sequence != NULL) {
                key_sequence->StopRoadSweeperFX();
            }
            break;
        }
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                if ((int)m_unkF4 & (1 << 3)) {
                    m_animation_star_controller->Play("WAIT");
                    UpdateAnimations(0.001f);
                    SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                    m_unk1A8 = 106;
                } else {
                    m_animation_star_controller->Play("WAIT", 1, 1);
                    UpdateAnimations(0.001f);
                    SetGenericBehaviour(GENERIC_BEHAVIOUR_4);
                    m_unk1A8 = 103;
                }
            }
            break;
    }
}

void CEntityNPCRoadSweeperEffalump::UpdateDeathBehaviour(F32) {

}

void CEntityNPCRoadSweeperEffalump::Render(F32 dt_maybe) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCRoadSweeperEffalump::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCRoadSweeperEffalump::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);

    if (m_arriving_particle_emitter == NULL) {
        ParseParticleEmitter("FX/FX_C340_03.xmd", (DKDSP::IParticleEmitter**)&m_arriving_particle_emitter);
    }
    if (m_dust_particle_emitter == NULL) {
        ParseParticleEmitter("FX/FX_C340_04.xmd", (DKDSP::IParticleEmitter**)&m_dust_particle_emitter);
    }
}

void CEntityNPCRoadSweeperEffalump::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}

BOOL CEntityNPCRoadSweeperEffalump::ParseParticleEmitter(std::string filename, DKDSP::IParticleEmitter** emitter) {
    m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(9, filename);

    DKDSP::CParticleEmitterDefinition* definition = m_entity_manager->GetGame()->GetObjectDictionary()->FindParticleEmitterDefinition(filename);
    *emitter = m_entity_manager->GetGame()->GetScene()->CreateParticleEmitterFromDefinition(m_unk0, definition);

    return TRUE;
}
