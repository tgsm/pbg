#include "engine/display/CImmediate.h"
#include "engine/display/DkDisplay.h"
#include "entities/CEntityScaryFaceBox.h"
#include "CGamePartIngame.h"
#include "CGameRoomManager.h"
#include "CIcon.h"
#include <cstdio>
#include <cstring>
#include <iostream>

DKDSP::CIm2DBatch* CEntityScaryFaceBox::m_pImediatePolyBack;
DKDSP::CIm2DBatch* CEntityScaryFaceBox::m_pImediatePolyIcon;

CEntityScaryFaceBox::CEntityScaryFaceBox(CEntityManager* entity_manager, std::string name) : CEntityZone(entity_manager, name) {
    m_unk1BC = "";
    m_unk1C0 = "";
    m_nb_needed_cookies = 0;
    m_unk1DC = -1;
}

CEntityScaryFaceBox::~CEntityScaryFaceBox() {
    DKDSP::CImmediate* immediate = DkDisplayGetEngine()->GetImmediate();

    if (m_pImediatePolyBack != NULL) {
        immediate->RemoveBatch2D(m_pImediatePolyBack);
        m_pImediatePolyBack = NULL;
    }
    if (m_pImediatePolyIcon != NULL) {
        immediate->RemoveBatch2D(m_pImediatePolyIcon);
        m_pImediatePolyIcon = NULL;
    }
}

void CEntityScaryFaceBox::Update(F32 dt) {
    if (IsFlagged(1 << 6)) {
        DelFlag(1 << 0);
        DelFlag(1 << 1);
        DelFlag(1 << 2);
    }

    CEntityZone::Update(dt);
}

void CEntityScaryFaceBox::ManageMessage(SDkMessage& message) {
    if (IsFlagged(1 << 6)) {
        return;
    }

    if (IsFlagged(1 << 0)) {
        // We've already checked if this is active, but we'll check again for good measure :^)
        if (strcmp(message.unk0, "Piglet") == 0 && strcmp(message.unk20, "ACTION") == 0 && IsFlagged(1 << 0)) {
            HandleAction();
        }
    }

    CEntityZone::ManageMessage(message);
}

void CEntityScaryFaceBox::ResolveContact(const DkPh::Collider::Body& body, int a1, int a2) {
    CEntityZone::ResolveContact(body, a1, a2);
}

// Equivalent: regalloc
void CEntityScaryFaceBox::Parse(DkXmd::CChunkIterator iter) {
    DkXmd::CChunkIterator dest, unused;

    CEntityZone::Parse(iter);

    char buf[128];
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "RTCFail") == 0) {
                m_unk1BC = dest.GetStringValue();
            } else if (strcmp(buf, "RTCSuccess") == 0) {
                m_unk1C0 = dest.GetStringValue();
            } else if (strcmp(buf, "NeededCookiesNb") == 0) {
                m_nb_needed_cookies = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityScaryFaceBox::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityZone::ParseBehavior(iter, behavior);
}

void CEntityScaryFaceBox::Init() {
    CEntityZone::Init();
}

// Incomplete
void CEntityScaryFaceBox::RenderCookiesNbIcon(F32 dt) {
    CIcon icon1, icon2;
    CGame* game = m_entity_manager->GetGame();
    CGameRoomManager* manager = ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager;

    if (!manager->IsPlayingRTC() || (manager->GetState() == 3 && (manager->m_unkC == 0 || manager->m_unkC == 1))) {
        return;
    }

    if (manager->GetState() == 5) {
        m_unk1D8 = 0.0f;
        m_unk1DC = -1;
        return;
    }

    if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->m_game_room_manager->IsPlayingRTC() || m_unk1D8 < 0.1f) {
        m_unk1D8 += dt;
    } else {
        m_unk1D8 = 500000.0f;
    }

    int cookies1;
    int cookies2;
    F32 in_f31;
    F32 in_f30;
    F32 fVar3;
    if (m_unk1DC == 0) {
        cookies1 = NbCookiesForNextGrimaceLevel();
        fVar3 = 10.0f;
        in_f31 = 0.2125f;
        in_f30 = 0.86333334f;
    } else if (m_unk1DC == 1) {
        cookies2 = NbCookiesForNextGrimaceLevel2();
        // fVar3 = 7.3f;
        int yeah = ((cookies2 + 1) * m_unk1D8) / 7.3f;
        cookies1 = cookies2 - yeah;
        if (cookies1 < 0) {
            cookies1 = 0;
        }
        in_f31 = 0.78749996f;
        in_f30 = 0.13666667f;
        fVar3 = 0.0f;
    }

    if (m_unk1DC == -1 || m_unk1D8 > fVar3) {
        m_unk1D8 = 0.0f;
        m_unk1DC = -1;
        return;
    }

    if (m_unk1D8 < 0.5f) {
        cookies2 = (m_unk1D8 / 0.5f) * 255.0f;
    } else {
        if (m_unk1D8 >= fVar3 - 0.5f) {
            cookies2 = ((fVar3 - m_unk1D8) / 0.5f) * 255.0f;
        } else {
            cookies2 = 255;
        }
    }

    RenderCookiesNbIconDirect(game, cookies1, in_f31, in_f30, cookies2, 0.35f, 0.17333333f, 0, 1);
}

// Equivalent: GUI engine load scheduling
void CEntityScaryFaceBox::RenderCookiesNbIconDirect(CGame* game, int a1, F32 x, F32 y, int a4, F32 width, F32 height, int a7, int a8) {
    F32 dVar6;
    F32 dVar5;
    F32 unk;
    F32 text_width;
    int alpha_;
    CIcon icon1, icon2;

    if (m_pImediatePolyBack == NULL) {
        m_pImediatePolyBack = game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);
    }
    if (m_pImediatePolyIcon == NULL) {
        m_pImediatePolyIcon = game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);
    }

    CIcon::BeginRender(game->m_display_engine, game->GetCamera(), game->GetScene());
    icon2.m_width = width;
    icon2.m_height = height;
    alpha_ = a4;
    icon2.m_alpha = alpha_;
    icon2.m_x = x;
    icon2.m_y = y;

    switch (a7) {
        case 0:
            icon2.m_texture = game->m_texture_dictionary->FindTexture("BMP_324");
            break;
        case 1:
            icon2.m_texture = game->m_texture_dictionary->FindTexture("BMP_208");
            break;
    }
    icon2.Render(m_pImediatePolyBack, a8);

    CDKW_RGBA color = CDKW_RGBA(0xFF, 0xFF, 0x99, alpha_);
    game->GetGuiEngine()->SetTextColor(color.m_r, color.m_g, color.m_b, color.m_a);

    char str[10] = {};
    sprintf(str, "%d", a1);

    unk = 0.46f * height;
    // here
    text_width = game->GetGuiEngine()->GetTextWidth(str, unk, NULL);
    dVar6 = 0.23f * width;
    dVar5 = (dVar6 + (text_width + (0.03f * width))) / 2;

    game->GetGuiEngine()->AddText(x - dVar5, y + (0.65f * (unk / 2)), str, unk, NULL, 0.0f);

    icon1.m_width = dVar6;
    icon1.m_height = (4.0f * dVar6) / 3.0f;
    icon1.m_alpha = alpha_;
    icon1.m_x = (x + dVar5) - (dVar6 / 2);
    icon1.m_y = y;
    icon1.m_texture = game->m_texture_dictionary->FindTexture("BMP_201");
    icon1.Render(m_pImediatePolyIcon, a8);

    CIcon::EndRender();
}

void CEntityScaryFaceBox::Render(F32 dt) {
    if (IsFlagged(1 << 1)) {
        CEntityZone::Render(dt);
    }
    RenderCookiesNbIcon(dt);
}

BOOL CEntityScaryFaceBox::ShouldKick() {
    return FALSE;
}
