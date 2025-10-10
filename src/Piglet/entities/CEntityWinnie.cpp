#include "entities/CEntityWinnie.h"
#include <cstring>
#include <iostream>

CEntityWinnie::CEntityWinnie(CEntityManager* entity_manager, std::string name) : CEntityHero(entity_manager, name) {
    m_unk13C = 1;
    m_unk244 = 1.0f;
    m_unk248 = 4.0f;
    m_unk24C = 1.0f;
    m_unk250 = 1.0f;
    m_unk254 = 1.0f;
    m_unk258 = 1.0f;
}

CEntityWinnie::~CEntityWinnie() {

}

void CEntityWinnie::ManageMessage(SDkMessage& message) {
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

    CEntityHero::ManageMessage(message);
}

void CEntityWinnie::ResolveContact(const DkPh::Collider::Body& body, int unk1, int unk2) {
    CEntityHero::ResolveContact(body, unk1, unk2);
}

void CEntityWinnie::DicreaseLife(int life) {
    m_unk13C -= life;

    if (m_unk13C <= 0) {
        SetMode(7);
    }
}

void CEntityWinnie::SetMode(U32 mode) {
    switch (mode) {
        case 9:
            DelFlag(1 << 4);
            SetMode(0);
            return;
        case 2:
            m_unk1C8 = 0.0f;
            AddFlag(1 << 4);
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
            CDKW_V3d position = frame->GetRwFrame()->modelling.pos;
            RwFrameTranslate(frame->GetRwFrame(), (RwV3d*)&(-position), 2);
            RwFrameRotate(180.0f, frame->GetRwFrame(), (RwV3d*)&CDKW_V3d::YAXIS, 2);
            RwFrameTranslate(frame->GetRwFrame(), (RwV3d*)&position, 2);

            m_unk1BC = -m_unk1BC;
            m_unk1C8 = m_unk218;
            m_unk1F0 = 0.0f;
            break;
        }
    }

    CEntityHero::SetMode(mode);
}

void CEntityWinnie::UpdateAnimations(F32 unk) {
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
        case 3:
            if (m_animation_star_controller->IsPlayingAnimation("AFFRAIDED") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped() == TRUE) {
                SetMode(6);
                UpdateAnimations_Wait(unk, 0);
            }
            break;
        case 7:
            if (m_animation_star_controller->IsPlayingAnimation("DEAD_0") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped() != FALSE) {
                SetMode(8);
            }
            break;
        case 10:
            UpdateAnimations_AdventureMode(unk);
            break;
    }

    CEntityHero::UpdateAnimations(unk);
}
