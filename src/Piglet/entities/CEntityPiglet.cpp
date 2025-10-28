#include "entities/CEntityPiglet.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <iostream>

U32 CEntityPiglet::GetSaveSize() {
    return 0x10;
}

void CEntityPiglet::AddFlag(U32 flag) {
    switch (flag) {
        case (1 << 3):
            m_unk284 = 0;
            m_unk280 = 0.0f;
            break;
        case (1 << 4):
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();
            break;
    }

    HideOSD();
    CEntityHero::AddFlag(flag);
}

void CEntityPiglet::DelFlag(U32 flag) {
    switch (flag) {
        case (1 << 3):
            SetMode(0);
            UpdateAnimations_Wait(0.0f, 1);
            break;
    }

    CEntityHero::DelFlag(flag);
}

void CEntityPiglet::SetGrimace(int grimace) {
    switch (grimace) {
        case 1:
            m_animation_star_controller->Play("GRIMACE01", 1);
            break;
        case 2:
            m_animation_star_controller->Play("GRIMACE02", 1);
            break;
        case 3:
            m_animation_star_controller->Play("GRIMACE03", 1);
            break;
        case 4:
            m_animation_star_controller->Play("GRIMACE04", 1);
            break;
        case 5:
            m_animation_star_controller->Play("GRIMACE05", 1);
            break;
        case 6:
            m_animation_star_controller->Play("GRIMACE06", 1);
            break;
        case 7:
            m_animation_star_controller->Play("GRIMACE08", 1);
            break;
        case 8:
            m_animation_star_controller->Play("GRIMACE09", 1);
            break;
        case 9:
            m_animation_star_controller->Play("GRIMACE10", 1);
            break;
        case 10:
            m_animation_star_controller->Play("GRIMACE11", 1);
            break;
    }
}

void CEntityPiglet::DicreaseLife(int life) {
    m_unk13C -= life;

    if (life == 0) {
        SetMode(5);
        return;
    }

    SetMode(4);

    if (m_unk13C <= 0) {
        m_state = 2;
        SetMode(7);
    } else if (m_unk13C <= 1) {
        m_animation_star_controller->Play("WORRIED_TO_PANIC", 1);
        m_state = 2;
        SetStateEnvironement();
    } else if (m_unk13C <= 2) {
        m_animation_star_controller->Play("BASE_TO_WORRIED", 1);
        m_state = 1;
        SetStateEnvironement();
    }
}

void CEntityPiglet::SetMode(U32 mode) {
    if (mode == GetMode()) {
        return;
    }

    switch (mode) {
        case 0:
            if (m_pushing_box != NULL) {
                return;
            }
            break;
        case 9:
            DelFlag(1 << 4);
            SetMode(0);
            return;
        case 1: {
            CDKW_Frame* frame = m_clump->GetFrame();
            m_unk260 = frame->m_rwframe->modelling.at;

            if (m_unk25C > 0) {
                m_animation_star_controller->Play("TURN_BACK_LEFT", 1);
            } else {
                m_animation_star_controller->Play("TURN_BACK_RIGHT", 1);
            }
            break;
        }
        case 2:
            if (GetMode() == 16 || GetMode() == 17) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->InterruptFightMode();
                return;
            }

            m_unk1C8 = 0.0f;
            AddFlag(1 << 4);
            break;
        case 14:
            m_animation_star_controller->Play("START_FIGHT", 1);
            break;
        case 15:
            // TODO
            if (!IsFlagged(1 << 0)) {
                SetMode(2);
            }

            m_animation_star_controller->Play("TURN_FIGHT", 1);
            RwV3d vec;
            vec.x = m_unk1BC.m_x;
            vec.y = m_unk1BC.m_y;
            vec.z = m_unk1BC.m_z;
            // This may be an inline
            if (!((vec.x - CDKW_V3d::ZERO.m_x) * (vec.x - CDKW_V3d::ZERO.m_x) +
                  (vec.y - CDKW_V3d::ZERO.m_y) * (vec.y - CDKW_V3d::ZERO.m_y) +
                  (vec.z - CDKW_V3d::ZERO.m_z) * (vec.z - CDKW_V3d::ZERO.m_z) <= CDKW_V3d::sm_Epsilon * CDKW_V3d::sm_Epsilon)) {
                      RwV3dNormalize(&vec, &vec);
            }
            m_unk260 = vec;
            break;
        case 16:
        case 17:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->InterruptFightMode();
            m_unk1BC = m_pushing_box->GetDirection();
            m_unk90.unk8 = CDKW_V3d::ZERO;
            m_unk90.unk14 = CDKW_V3d::ZERO;
            break;
        case 18:
            UnHideOSD(m_unk2A0);
            m_animation_star_controller->Play("USE_OBJECT", 1);
            break;
        case 21:
            m_animation_star_controller->Play("VICTORY", 1);
            SetMode(11);
            m_unk1C8 = 0.0f;
            break;
        case 19:
            if (m_unk138 < 0.2f) {
                return;
            }
            if (GetSpeed() <= m_unk210) {
                m_animation_star_controller->Play("SEARCH", 1);
                mode = 19;
            } else {
                m_animation_star_controller->Play("SEARCH_RUN", 1);
                mode = 20;
            }
            break;
        case 10:
            HideOSD();
            break;
        case 7:
            m_animation_star_controller->Play("DEAD_0", 1);
            break;
        case 8: {
            CDKW_V3d position;
            m_animation_star_controller->Play("DEAD_1", 1, 1);
            CDKW_Frame* frame = m_clump->GetFrame();
            position = frame->GetRwFrame()->modelling.pos;
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
    m_unk26C = 0;
}

void CEntityPiglet::Render(F32 dt_maybe) {
    CEntityHero::Render(dt_maybe);
    CEntityPiglet::RenderOSD();
}

void CEntityPiglet::Parse(DkXmd::CChunkIterator iter) {
    CEntityHero::Parse(iter);
    m_unk278 = m_unk1DC;

    std::string tmp;
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "FrictionShoot") {
                m_unk278 = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityPiglet::HideOSD() {
    switch (m_unk284) {
        case 0:
            break;
        case 2:
            PlayOSDSound("SND_202_2D", 0);
            m_unk284 = 3;
            m_unk280 = 0.0f;
            break;
        case 1:
            PlayOSDSound("SND_202_2D", 0);
            m_unk284 = 3;
            m_unk280 = 0.2f - m_unk280;
            break;
        case 3:
            PlayOSDSound("SND_202_2D", 0);
            break;
    }
}

void CEntityPiglet::UnHideOSD(U32 unk) {
    m_unk2A0 = unk;
    PlayOSDSound("SND_201_2D", 0);

    switch (m_unk284) {
        case 0:
            m_unk284 = 1;
            m_unk280 = 0.0f;
            break;
        case 2:
            m_unk280 = 0.2f;
            break;
        case 3:
            m_unk284 = 1;
            m_unk280 = 0.2f - m_unk280;
            break;
    }
}
