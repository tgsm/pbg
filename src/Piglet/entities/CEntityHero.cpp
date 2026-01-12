#include "engine/input/IInputEngine.h"
#include "rwsdk/plcore/bavector.h"
#include "entities/CEntityHero.h"
#include <cstdlib>
#include <iostream>

CEntityHero::CEntityHero(CEntityManager* entity_manager, std::string name) : CEntityMesh(entity_manager, name) {
    m_delta_time = m_entity_manager->GetGame()->GetDeltaTime();
    m_mode = 11;
    m_unk124 = 0;
    m_state = 0;

    m_unk12C = 0.0f;
    m_pad_timer = 0.0f;

    m_unk134 = 0;

    m_unk1BC = CDKW_V3d::ZERO;

    m_unk1C8 = 1.0f;
    m_acceleration = 0.0f;
    m_friction = 0.0f;
    m_friction_turn_back = 0.0f;
    m_stop_speed = 0.0f;
    m_speed_rotate = 0.0f;
    m_speed_rotate_turn_back = 0.0f;
    m_gravity = 0.0f;
    m_rotate_angle = 0.0f;
    m_turn_back_angle = 0.0f;
    m_turn_back_speed = 0.0f;
    m_unk20C = 0.0f;
    m_pad_threshold_slow = 0.0f;
    m_pad_threshold_walk = 0.0f;
    m_pad_threshold_run = 0.0f;
    m_anim_threshold_slow = 0.0f;
    m_anim_threshold_walk = 0.0f;
    m_anim_threshold_run = 0.0f;
    m_anim_threshold_slow_worried = 0.0f;
    m_anim_threshold_walk_worried = 0.0f;
    m_anim_threshold_run_panic = 0.0f;
    m_anim_threshold_idle = 0.0f;
    m_speed_slow = 0.0f;
    m_speed_walk = 0.0f;
    m_speed_run = 0.0f;
    m_speed_walk_worried = 0.0f;
    m_speed_slow_worried = 0.0f;
    m_speed_run_panic = 0.0f;
    m_unk25C = 0;
    m_unk138 = 0.0f;
    m_speed_slow = 0.0f;
    m_speed_walk = 0.0f;
    m_speed_run = 0.0f;
    m_speed_slow_worried = 0.0f;
    m_speed_walk_worried = 0.0f;
    m_speed_run_panic = 0.0f;

    m_axe_h_input = DKI::IInputEngine::CreateInput("AxeH", 0, 14);
    m_axe_v_input = DKI::IInputEngine::CreateInput("AxeV", 0, 15);
    m_action_input = DKI::IInputEngine::CreateInput("Action", 0, 19);
    m_object_a_input = DKI::IInputEngine::CreateInput("ObjectA", 0, 20);
    m_object_b_input = DKI::IInputEngine::CreateInput("ObjectB", 0, 22);
    m_object_c_input = DKI::IInputEngine::CreateInput("ObjectC", 0, 21);
    m_pad_l1_input = DKI::IInputEngine::CreateInput("PadL1", 0, 23);
    m_pad_l2_input = DKI::IInputEngine::CreateInput("PadL2", 0, 23);
    m_pad_r1_input = DKI::IInputEngine::CreateInput("PadR1", 0, 24);
    m_pad_r2_input = DKI::IInputEngine::CreateInput("PadR2", 0, 24);
}

CEntityHero::~CEntityHero() {
    DKI::IInputEngine::DestroyInput(m_axe_h_input);
    DKI::IInputEngine::DestroyInput(m_axe_v_input);
    DKI::IInputEngine::DestroyInput(m_action_input);
    DKI::IInputEngine::DestroyInput(m_object_a_input);
    DKI::IInputEngine::DestroyInput(m_object_b_input);
    DKI::IInputEngine::DestroyInput(m_object_c_input);
    DKI::IInputEngine::DestroyInput(m_pad_r1_input);
    DKI::IInputEngine::DestroyInput(m_pad_r2_input);
    DKI::IInputEngine::DestroyInput(m_pad_l1_input);
    DKI::IInputEngine::DestroyInput(m_pad_l2_input);
}

void CEntityHero::Reset() {
    CEntityMesh::Reset();
}

void CEntityHero::SetMode(U32 mode) {
    if (m_mode != mode) {
        m_unk138 = 0.0f;
        m_mode = mode;
    }
}

void CEntityHero::AddFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK3:
            m_unk1BC = CDKW_V3d::ZERO;
            m_unk90.unk8 = CDKW_V3d::ZERO;
            m_unk90.unk14 = CDKW_V3d::ZERO;
            SetMode(11);
            break;
        case ENTITY_FLAG_UNK4:
            m_unk134 = 0;
            m_pad_timer = 0.0f;
            break;
    }

    CEntityMesh::AddFlag(flag);
}

void CEntityHero::DelFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK3:
            SetToGround();
            SetMode(0);
            m_controller->Resume();
            break;
    }

    CEntityMesh::DelFlag(flag);
}

void CEntityHero::ManageMessage(SDkMessage& message) {
    CEntity::ManageMessage(message);
}

void CEntityHero::ResolveContact(const DkPh::Collider::Body& body, int, int) {
    CEntity* entity4 = body.entity4;
    if (m_unk124 & (1 << 0)) {
        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, entity4->m_unk0, "ACTION", 0);
    }
}

U32 CEntityHero::GetActions() {
    if (m_entity_manager->GetGame()->FadeRelatedInline()) {
        return 0;
    }

    U32 actions = 0;
    if (m_action_input->GetState().m_unk0 == TRUE) {
        actions |= (1 << 0);
        m_unk12C = 0.0f;
    }
    if (m_object_a_input->GetState().m_unk0 == TRUE) {
        actions |= (1 << 1);
        m_unk12C = 0.0f;
    }
    if (m_object_b_input->GetState().m_unk0 == TRUE) {
        actions |= (1 << 2);
        m_unk12C = 0.0f;
    }
    if (m_object_c_input->GetState().m_unk0 == TRUE) {
        actions |= (1 << 3);
        m_unk12C = 0.0f;
    }

    return actions;
}

CDKW_V3d CEntityHero::GetPadDirection() {
    CDKW_V3d direction = CDKW_V3d::ZERO;
    direction.x = m_axe_h_input->GetState().m_unk4;
    direction.z = -m_axe_v_input->GetState().m_unk4;
    direction.y = 0.0f;

    if (RwV3dLength(&direction) > 1.0f) {
        NormalizeInline(direction);
    }

    return direction;
}

void CEntityHero::UpdateAnimations_Idle(F32 dt) {
    static int s_count = 0;
    static int s_last = 0;

    int anim = 0;
    if (s_count >= 2) {
        if (s_last == 0) {
            anim = 1;
        }
    } else {
        anim = rand() % 2;
    }
    if (anim == s_last) {
        s_count++;
    } else {
        s_count = 0;
    }

    switch (m_state) {
        case 0:
            m_animation_star_controller->Play((anim == 0) ? "IDLE_0" : "IDLE_1", 1);
            break;
        case 1:
            m_animation_star_controller->Play((anim == 0) ? "IDLE_WORRIED_0" : "IDLE_WORRIED_1", 1);
            break;
        case 2:
            m_animation_star_controller->Play((anim == 0) ? "IDLE_PANIC_0" : "IDLE_PANIC_1", 1);
            break;
    }

    s_last = anim;
}

void CEntityHero::UpdateAnimations_Wait(F32 dt, int a2) {
    switch (m_state) {
        case 0:
            m_animation_star_controller->Play("WAIT", 1, a2);
            break;
        case 1:
            m_animation_star_controller->Play("WAIT_WORRIED", 1, a2);
            break;
        case 2:
            m_animation_star_controller->Play("WAIT_PANIC", 1, a2);
            break;
    }
}

void CEntityHero::UpdateAnimations(F32 dt) {
    CEntityMesh::UpdateAnimations(dt);
}

void CEntityHero::Parse(DkXmd::CChunkIterator iter) {
    CEntityMesh::Parse(iter);

    DkPh::Collider::Body& body = m_entity_manager->GetUnkCollider()->GetBodyRef(m_unk90.unk0);
    body.unk34 = ~(1 << 16);

    std::string str;
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            str = dest.GetName();

            if (str == "Acceleration") {
                m_acceleration = dest.GetFloatValue();
            } else if (str == "Friction") {
                m_friction = dest.GetFloatValue();
            } else if (str == "FrictionTurnBack") {
                m_friction_turn_back = dest.GetFloatValue();
            } else if (str == "StopSpeed") {
                m_stop_speed = dest.GetFloatValue();
            } else if (str == "SpeedRotate") {
                m_speed_rotate = dest.GetFloatValue();
            } else if (str == "SpeedRotateTurnBack") {
                m_speed_rotate_turn_back = dest.GetFloatValue();
            } else if (str == "TurnBackSpeed") {
                m_turn_back_speed = dest.GetFloatValue();
            } else if (str == "Gravity") {
                m_gravity = dest.GetFloatValue();
            } else if (str == "SpeedSlow") {
                m_speed_slow = dest.GetFloatValue();
            } else if (str == "SpeedWalk") {
                m_speed_walk = dest.GetFloatValue();
            } else if (str == "SpeedRun") {
                m_speed_run = dest.GetFloatValue();
            } else if (str == "SpeedSlowWorried") {
                m_speed_slow_worried = dest.GetFloatValue();
            } else if (str == "SpeedWalkWorried") {
                m_speed_walk_worried = dest.GetFloatValue();
            } else if (str == "SpeedRunPanic") {
                m_speed_run_panic = dest.GetFloatValue();
            } else if (str == "PadTresholdSlow") {
                m_pad_threshold_slow = dest.GetFloatValue();
            } else if (str == "PadTresholdWalk") {
                m_pad_threshold_walk = dest.GetFloatValue();
            } else if (str == "PadTresholdRun") {
                m_pad_threshold_run = dest.GetFloatValue();
            } else if (str == "AnimTresholdIdle") {
                m_anim_threshold_idle = dest.GetFloatValue();
            } else if (str == "AnimTresholdSlow") {
                m_anim_threshold_slow = dest.GetFloatValue();
            } else if (str == "AnimTresholdWalk") {
                m_anim_threshold_walk = dest.GetFloatValue();
            } else if (str == "AnimTresholdRun") {
                m_anim_threshold_run = dest.GetFloatValue();
            } else if (str == "AnimTresholdSlowWorried") {
                m_anim_threshold_slow_worried = dest.GetFloatValue();
            } else if (str == "AnimTresholdWalkWorried") {
                m_anim_threshold_walk_worried = dest.GetFloatValue();
            } else if (str == "AnimTresholdRunPanic") {
                m_anim_threshold_run_panic = dest.GetFloatValue();
            } else if (str == "RotateAngle") {
                m_rotate_angle = dest.GetFloatValue();
            } else if (str == "TurnBackAngle") {
                m_turn_back_angle = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 3) {
        SetMode(11);
    } else {
        SetMode(0);
    }

    UpdateAnimations_AdventureMode(0.0f);
}

void CEntityHero::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {

}

void CEntityHero::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) && !IsFlagged(ENTITY_FLAG_UNK8)) {
        CEntityMesh::Render(dt);
    }
}

void CEntityHero::UpdateActions() {

}

void CEntityHero::ResetPadTimer(F32 dt) {
    if (m_state == 1) {
        if (m_pad_timer > 0.0f && m_pad_timer < 1.3f) {
            m_pad_timer = -0.4f;
            return;
        }

        if (m_pad_timer < 0.0f) {
            m_pad_timer += dt;
        }

        if (m_pad_timer > 0.0f) {
            m_pad_timer = 0.0f;
        }
    } else {
        m_pad_timer = 0.0f;
    }
}
