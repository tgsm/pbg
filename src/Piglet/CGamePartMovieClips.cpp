#include <rwsdk/badevice.h>
#include <cstring>
#include <iostream>
#include "CGamePartMovieClips.h"
#include "CGame.h"
#include "CGuiMovieClipsEventHandler.h"
#include "CIcon.h"
#include "engine/input/IInputEngine.h"
#include "engine/wrap/DKW_RGBA.h"

extern "C" {
extern void Rt2dCTMSetIdentity(void);
}

CGamePartMovieClips::CGamePartMovieClips(CGame* game) {
    m_unk0 = 2;
    m_unk4 = m_unk0;
    m_game = NULL;
    m_time = 0.0f;

    m_game = game;

    CDKW_RGBA clear_color;
    clear_color = CDKW_RGBA(199, 226, 222, 0xFF);

    m_game->GetScene()->SelectCamera(m_game->GetCamera());
    m_game->GetScene()->Clear(3, clear_color.m_r / 255.0f, clear_color.m_g / 255.0f, clear_color.m_b / 255.0f);
    m_game->GetScene()->BeginRender();
    m_game->GetScene()->Flush();
    m_game->GetScene()->EndRender();
    m_game->GetScene()->Flip(0);

    m_time = 0.0f;

    U32 rf_unkC = m_game->GetResourceFactory()->m_unkC;
    m_game->GetResourceFactory()->m_unkC = 0;
    m_game->LoadConfigFile("MENUS/MOVIECLIPS/MOVIECLIPS.XMD");
    m_game->GetResourceFactory()->m_unkC = rf_unkC;

    m_game->GetGuiManager()->Reset();
    m_game->GetGuiManager()->SetActive("MOVIECLIPS_MAIN", 1);
    m_game->GetGuiManager()->SetVisible("MOVIECLIPS_MAIN", 1);

    for (int i = 0; i < 6; i++) {
        m_batches[i] = m_game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);
    }

    m_game->GetCamera()->SetZFar(300.0f);
    m_game->GetCamera()->SetZNear(0.6f);
    m_game->GetCamera()->SetViewWindow(0.5f, 0.5f);

    Rt2dCTMSetIdentity();

    m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, clear_color.m_r, clear_color.m_g, clear_color.m_b, 0.0f);

    m_game->m_unk5038 = 1;
}

CGamePartMovieClips::~CGamePartMovieClips() {
    m_game->m_fx_manager->Clear();
    m_game->m_sound_engine->DeleteAllSounds();

    for (int i = 0; i < 6; i++) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batches[i]);
        m_batches[i] = NULL;
    }

    m_game->GetGuiManager()->UnLoadLevel(0);
    m_game->GetResourceFactory()->UnloadResources(0);
}

U32 CGamePartMovieClips::NextFrame() {
    if (!m_game->m_display_engine->Update()) {
        return 9;
    }

    if (m_time == 0.0f) {
        m_game->m_timer->Reset();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
        m_game->ComputeDeltaTime();
    }
    F32 dt = m_game->GetDeltaTime();
    Update(dt);

    switch (m_game->m_unk5038) {
        case 1:
            if (m_game->FadeOut(dt) == 0) {
                m_game->m_unk5038 = 2;
            }
            break;
        case 0:
            if (m_game->FadeIn(dt) == 0) {
                m_game->m_unk5038 = 2;
            }
            break;
    }

    Render(dt);

    if (m_unk4 != m_unk0 && m_game->m_unk5038 == 2) {
        return NextFrameExit();
    }
    return m_unk0;
}

void CGamePartMovieClips::Update(F32 dt) {
    m_time += dt;
    m_game->m_sound_engine->BeginUpdate();
    m_game->m_sound_engine->EndUpdate();
    m_game->GetGuiManager()->Update(dt);
}

void CGamePartMovieClips::Render(F32 dt) {
    m_game->GetScene()->SelectCamera(m_game->GetCamera());
    m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);

    m_game->GetScene()->BeginRender();

    m_game->GetGuiManager()->Render(dt);
    if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
        RenderIcons(dt);
    }
    m_game->GetScene()->Flush();
    m_game->RenderFade();

    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flip(0);
}

void CGamePartMovieClips::RenderBackGround(F32 dt) {
    CIcon::BeginRender(m_game->m_display_engine, m_game->GetCamera(), m_game->GetScene());

    CIcon icon;
    icon.m_unk14 = 1.0f;
    icon.m_unk18 = 1.0f;
    icon.m_unk8 = 0.5f;
    icon.m_unkC = 0.5f;

    char name[256] = {};
    RwEngineInstance->stringFuncs.rwsprintf(name, "BMP_520");
    icon.m_texture = m_game->m_texture_dictionary->FindTexture(name);

    icon.Render(m_batches[0], 0);
    CIcon::EndRender();
}

BOOL CGamePartMovieClips::IsVideoOpen(int index) {
    if (m_game->m_unk8 & (1 << 4)) {
        return TRUE;
    }

    if (index < 3) {
        return TRUE;
    }

    U32 unk = (index - 3) * 2;
    if (m_game->GetMission(unk).m_unk2C == 1) {
        unk++;
        if (m_game->GetMission(unk).m_unk2C == 1) {
            return TRUE;
        }
    }

    return FALSE;
}

void CGamePartMovieClips::RenderIcons(F32 dt) {
    CIcon::BeginRender(m_game->m_display_engine, m_game->GetCamera(), m_game->GetScene());
    CIcon icon;

    for (int i = 0; i < 6; i++) {
        icon.m_unk14 = 0.18765625f;
        icon.m_unk18 = 0.18875f;
        switch (i) {
            case 0:
                icon.m_unk8 = 0.12875f;
                icon.m_unkC = 0.27562502f;
                break;
            case 1:
                icon.m_unk8 = 0.3878125f;
                icon.m_unkC = 0.27562502f;
                break;
            case 2:
                icon.m_unk8 = 0.64875f;
                icon.m_unkC = 0.27562502f;
                break;
            case 3:
                icon.m_unk8 = 0.12875f;
                icon.m_unkC = 0.5085417f;
                break;
            case 4:
                icon.m_unk8 = 0.3878125f;
                icon.m_unkC = 0.5085417f;
                break;
            case 5:
                icon.m_unk8 = 0.6509375f;
                icon.m_unkC = 0.5085417f;
                break;
        }

        icon.m_unk8 += icon.m_unk14 / 2;
        icon.m_unkC += icon.m_unk18 / 2;
        icon.m_unk14 *= 0.9f;
        icon.m_unk18 *= 0.9f;
        icon.m_unk10 = 0.0f;

        char name[256] = {};
        int unk_batch_no = ((CGuiMovieClipsEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiMovieClipsEventHandler"))->GetUnk10();

        if (IsVideoOpen(i)) {
            RwEngineInstance->stringFuncs.rwsprintf(name, "%s%d%d", "BMP_5", (i == unk_batch_no) ? 0 : 1, i + 1);
        } else {
            RwEngineInstance->stringFuncs.rwsprintf(name, "BMP_500");
        }

        icon.m_texture = m_game->m_texture_dictionary->FindTexture(name);
        icon.Render(m_batches[i], 0);
    }
    m_game->GetScene()->Flush();
    CIcon::EndRender();
}

U32 CGamePartMovieClips::NextFrameExit() {
    m_game->GetGuiManager()->SetActive("MOVIECLIPS_MAIN", 0);
    m_game->GetGuiManager()->SetVisible("MOVIECLIPS_MAIN", 0);
    m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
    return 0;
}
