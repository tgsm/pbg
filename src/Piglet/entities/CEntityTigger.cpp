#include "entities/CEntityTigger.h"
#include <cstring>
#include <iostream>

CEntityTigger::CEntityTigger(CEntityManager* entity_manager, std::string name) : CEntityHero(entity_manager, name) {
    ReInit();
}

CEntityTigger::~CEntityTigger() {

}

void CEntityTigger::ReInit() {
    m_unk13C = 1;
    m_unk260 = -1.0f;
    m_unk244 = 1.0f;
    m_unk248 = 2.0f;
    m_unk24C = 9.0f;
    m_unk250 = 1.0f;
    m_unk254 = 2.0f;
    m_unk258 = 9.0f;
    m_unk264 = 0;
    m_unk124 = 0;
    m_state = 0;
}

void CEntityTigger::ManageMessage(SDkMessage& message) {
    if (strcmp(message.unk20, "EXIT") == 0) {
        CEntity* entity = m_entity_manager->GetEntity(message.unk0);
        if (entity != NULL) {
            int type = entity->GetType();
            if (type > ENTITY_UNK5 && type < ENTITY_UNK9) {
                m_unk1CC = ((CEntityObject*)entity)->GetPosition();
                SetMode(10);
            }
        }
    }

    if (strcmp(message.unk20, "HERO_DETECT") == 0) {
        m_unk264++;
        if (m_unk264 == TRUE) {
            Detected();
        }
    } else if (strcmp(message.unk20, "HERO_UNDETECT") == 0) {
        m_unk264--;
        if (m_unk264 == FALSE) {
            m_unk260 = 0.0f;
        }
    }

    CEntityHero::ManageMessage(message);
}

void CEntityTigger::ResolveContact(const DkPh::Collider::Body& body, int unk1, int unk2) {
    CEntityHero::ResolveContact(body, unk1, unk2);
}

void CEntityTigger::AddFlag(U32 flag) {
    CEntityHero::AddFlag(flag);
}

void CEntityTigger::DelFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK3:
            ReInit();
            break;
    }

    CEntityHero::DelFlag(flag);
}

void CEntityTigger::Detected() {
    m_unk260 = -1.0f;
    if (GetMode() != 10 && m_state != 1) {
        m_animation_star_controller->Play("BASE_TO_WORRIED");
        m_state = 1;
        SetMode(4);
    }
}

void CEntityTigger::UnDetected() {
    m_unk260 = -1.0f;
    if (GetMode() != 10 && m_state == 1) {
        m_animation_star_controller->Play("WORRIED_TO_BASE");
        m_state = 0;
        SetMode(4);
    }
}

void CEntityTigger::DicreaseLife(int life) {
    m_unk13C -= life;

    if (m_unk13C <= 0) {
        m_animation_star_controller->Play("AFFRAIDED");
        m_state = 1;
        SetMode(7);
    }
}

void CEntityTigger::SetMode(U32 mode) {
    switch (mode) {
        case 9:
            DelFlag(ENTITY_FLAG_UNK4);
            SetMode(0);
            return;
        case 2:
            m_unk1C8 = 0.0f;
            AddFlag(ENTITY_FLAG_UNK4);
            break;
        case 3:
            m_unk1C8 = 0.0f;
            break;
        case 1:
        case 12:
            CEntityHero::SetMode(0);
            return;
        case 7:
            m_animation_star_controller->Play("DEAD_0", 1);
            break;
        case 8: {
            m_animation_star_controller->Play("DEAD_1", 1, 1);
            CDKW_Frame* frame = m_clump->GetFrame();
            CDKW_V3d position = CDKW_V3d(frame->GetRwFrame()->modelling.pos);
            RwFrameTranslate(frame->GetRwFrame(), &(-position), 2);
            RwFrameRotate(180.0f, frame->GetRwFrame(), &CDKW_V3d::YAXIS, 2);
            RwFrameTranslate(frame->GetRwFrame(), &position, 2);

            m_unk1BC = -m_unk1BC;
            m_unk1C8 = m_speed_run;
            m_gravity = 0.0f;
            break;
        }
    }

    CEntityHero::SetMode(mode);
}

void CEntityTigger::UpdateAnimations(F32 unk) {
    switch (GetMode()) {
        case 0:
            UpdateAnimations_AdventureMode(unk);
            break;
        case 2:
            UpdateAnimations_AdventureMode(unk);
            break;
        case 6:
            UpdateAnimations_AdventureMode(unk);
            break;
        case 4:
            if ((m_animation_star_controller->IsPlayingAnimation("BASE_TO_WORRIED") == TRUE || m_animation_star_controller->IsPlayingAnimation("WORRIED_TO_BASE") == TRUE) &&
                m_animation_star_controller->IsPlayingAnimationLooped() == TRUE) {
                SetMode(0);
                UpdateAnimations_Wait(unk, 0);
            }
            break;
        case 3:
            if (m_animation_star_controller->IsPlayingAnimation("AFFRAIDED") == TRUE &&
                m_animation_star_controller->IsPlayingAnimationLooped() == TRUE) {
                SetMode(5);
                UpdateAnimations_Wait(unk, 0);
            }
            break;
        case 5:
            if ((m_animation_star_controller->IsPlayingAnimation("WAIT") == TRUE || m_animation_star_controller->IsPlayingAnimation("WAIT_WORRIED") == TRUE) &&
                m_animation_star_controller->GetPlayingAnimationTime() > 0.25f) {
                SetMode(6);
                UpdateAnimations_Wait(unk, 0);
            }
            break;
        case 7:
            if (m_animation_star_controller->IsPlayingAnimation("DEAD_0") == TRUE &&
                m_animation_star_controller->IsPlayingAnimationLooped() != FALSE) {
                SetMode(8);
            }
            break;
        case 10:
            UpdateAnimations_AdventureMode(unk);
            break;
    }

    CEntityHero::UpdateAnimations(unk);
}

void CEntityTigger::UpdateActions() {

}

void CEntityTigger::Update(F32 dt_maybe) {
    if (m_unk260 != -1.0f) {
        m_unk260 += dt_maybe;
        if (m_unk260 >= 2.0f) {
            UnDetected();
        }
    }

    CEntityHero::Update(dt_maybe);
}
