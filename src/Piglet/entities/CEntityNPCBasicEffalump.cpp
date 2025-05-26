#include "entities/CEntityNPCBasicEffalump.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

CEntityNPCBasicEffalump::CEntityNPCBasicEffalump(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
    m_unk2D8 = 10.0f;
}

CEntityNPCBasicEffalump::~CEntityNPCBasicEffalump() {

}

void CEntityNPCBasicEffalump::Reset() {
    CEntityNPC::Reset();
}

// Incomplete
void CEntityNPCBasicEffalump::UpdateDetectionBehaviour(f32 a1) {
    CDKW_V3d pos_delta;
    CDKW_V3d frame_vec;

    switch (m_unk1A8) {
        case 0:
            if (m_unk15C == 2) {
                m_animation_star_controller->Play("DETECT_SOUND");
            } else {
                m_animation_star_controller->Play("DETECT_VISUAL");
            }
            m_unk1A8 = 1;

            if (strcmp(m_animation_star_controller->GetTargetAnimationName()->c_str(), "DETECT_VISUAL") == 0) {
                CDKW_Frame* frame = m_clump->GetFrame();
                RwFrame* rwframe = frame->m_rwframe;
                frame_vec.m_x = rwframe->unk34.x;
                frame_vec.m_y = rwframe->unk34.y;
                frame_vec.m_z = rwframe->unk34.z;

                const CDKW_V3d& npc_position = GetPosition();
                const CDKW_V3d& hero_position = m_entity_manager->GetHero()->GetPosition();
                pos_delta.m_x = hero_position.m_x - npc_position.m_x;
                pos_delta.m_y = hero_position.m_y - npc_position.m_y;
                pos_delta.m_z = hero_position.m_z - npc_position.m_z;
                ComputeRotationAngle(frame_vec, pos_delta);
                RotateAccordingToGarbageVar(a1);
            }
            break;
        case 1: {
            if (strcmp(m_animation_star_controller->GetTargetAnimationName()->c_str(), "DETECT_VISUAL") == 0) {
                CDKW_Frame* frame = m_clump->GetFrame();
                RwFrame* rwframe = frame->m_rwframe;
                frame_vec.m_x = rwframe->unk34.x;
                frame_vec.m_y = rwframe->unk34.y;
                frame_vec.m_z = rwframe->unk34.z;

                const CDKW_V3d& npc_position = GetPosition();
                const CDKW_V3d& hero_position = m_entity_manager->GetHero()->GetPosition();
                pos_delta.m_x = hero_position.m_x - npc_position.m_x;
                pos_delta.m_y = hero_position.m_y - npc_position.m_y;
                pos_delta.m_z = hero_position.m_z - npc_position.m_z;
                ComputeRotationAngle(frame_vec, pos_delta);
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

void CEntityNPCBasicEffalump::UpdateFightBehaviour(f32 a1) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("WAIT");
            m_unk1A8 = 102;
            break;
        case 102: {
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();

            SDkMessage message;
            if (m_entity_manager->GetGame()->GetMailbox()->GetMessage(&message, m_unk0, 1)) {
                if (strcmp(message.unk20, "START_FIGHT") == 0) {
                    PlayWalkAnim(0);
                    m_animation_speed = m_animation_star_controller->GetPlayingAnimationSpeed();
                    m_unk1A8 = 103;
                }
            }
            break;
        }
        case 103: {
            m_animation_star_controller->SetPlayingAnimationSpeed(m_animation_speed);

            if (!FollowSplinePath(a1, 1.0f, 1) || m_unkF4 & (1 << 8)) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
                m_unk1A8 = 105;
            }
            break;
        }
        case 104:
            m_unk2D8 += a1;
            if (m_unk2D8 >= 10.0f) {
                m_unk1A8 = 103;
                PlayWalkAnim(0);
            }
            break;
        case 106:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                CEntityHero* hero = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->GetCurrentHero();
                m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, hero->m_unk0, "FRITTEN_PIGLET", 0);

                CDKW_V3d position = GetPosition();
                hero->GetPosition().SquareDistToPoint(position);

                MakeNPCLaughing();
            }
            break;
        case 100:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->StartFightMode(this, 1);
            m_animation_star_controller->Play("FRIGHTEN_PIGLET", 1, 1);
            m_unk1A8 = 106;
            break;
    }
}

void CEntityNPCBasicEffalump::UpdateGrimaceBehaviour(f32) {
    switch (m_unk1A8) {
        case 101:
            m_animation_star_controller->Play("IS_FRIGHTENED", 1, 1);
            m_unk1A8 = 102;
            break;
        case 102:
            if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetGenericBehaviour(GENERIC_BEHAVIOUR_4);

                if (m_unk2D8 < 10.0f) {
                    m_unk1A8 = 104;
                } else {
                    m_animation_star_controller->Play("WAIT");
                    m_unk1A8 = 103;
                }
            }
            break;
    }
}

void CEntityNPCBasicEffalump::Render(f32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCBasicEffalump::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCBasicEffalump::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCBasicEffalump::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
