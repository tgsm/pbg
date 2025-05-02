#include "entities/CEntityPiglet.h"
#include "CGamePartIngame.h"
#include <iostream>

u32 CEntityPiglet::GetSaveSize() {
    return 0x10;
}

void CEntityPiglet::AddFlag(u32 flag) {
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

void CEntityPiglet::DelFlag(u32 flag) {
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

void CEntityPiglet::Render(f32 dt_maybe) {
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

void CEntityPiglet::UnHideOSD(u32 unk) {
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
