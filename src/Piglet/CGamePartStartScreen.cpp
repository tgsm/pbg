#include "CGame.h"
#include "CGamePartStartScreen.h"
#include <iostream>

extern "C" {
extern void Rt2dCTMSetIdentity(void);
}

CGamePartStartScreen::CGamePartStartScreen(CGame* game) {
    m_unk0 = 4;
    m_unk4 = m_unk0;

    m_game = NULL;
    m_unk10 = 0.0f;
    m_unk14 = 0;

    m_game = game;
    m_unk10 = 0.0f;
    U32 rf_unkC = m_game->GetResourceFactory()->m_unkC;
    m_game->GetResourceFactory()->m_unkC = 0;
    m_game->LoadConfigFile("MENUS/STARTSCR/STARTSCR.XMD");
    m_game->GetResourceFactory()->m_unkC = rf_unkC;

    m_game->m_gui_manager->Reset();
    m_game->m_gui_manager->GetGuiPtr("TITLE_TITLE")->menu->Reset();
    m_game->m_gui_manager->SetActive("TITLE_TITLE", 1);
    m_game->m_gui_manager->SetVisible("TITLE_TITLE", 1);

    Rt2dCTMSetIdentity();

    m_game->FadeInit(1.0f, CGame::FADE_TYPE_0, 0, 0, 0, 0.0f);

    m_game->m_unk5038 = 1;
}

CGamePartStartScreen::~CGamePartStartScreen() {
    m_game->m_fx_manager->Clear();
    m_game->m_sound_engine->DeleteAllSounds();
    m_game->m_gui_manager->UnLoadLevel(0);
    m_game->GetResourceFactory()->UnloadResources(0);
}

U32 CGamePartStartScreen::NextFrame() {
    m_game->m_display_engine->Update();

    if (m_unk10 == 0.0f) {
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

void CGamePartStartScreen::Update(F32 dt) {
    m_unk10 += dt;

    m_game->m_sound_engine->BeginUpdate();
    m_game->m_sound_engine->EndUpdate();

    m_game->m_gui_manager->Update(dt);
}

void CGamePartStartScreen::Render(F32 dt) {
    m_game->GetScene()->SelectCamera(m_game->GetCamera());

    m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);

    m_game->GetScene()->BeginRender();
    m_game->m_gui_manager->Render(dt);
    m_game->m_fx_manager->Render();
    m_game->GetScene()->Flush();
    m_game->RenderFade();
    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flip(0);
}

U32 CGamePartStartScreen::NextFrameExit() {
    m_game->m_gui_manager->SetActive("TITLE_TITLE", 0);
    m_game->m_gui_manager->SetVisible("TITLE_TITLE", 0);

    m_game->ResetOpcodeBuffer();
    m_game->PushOpcodeValue(13);
    m_game->PushOpcodeValue(14);
    m_game->PushOpcodeValue(10);
    m_game->PushOpcodeValue(2);
    m_game->PushOpcodeValue(8);
    m_game->PushOpcodeValue(1);
    m_game->PushOpcodeValue(1);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(0);
    m_game->PushOpcodeValue(1);
    m_game->PushOpcodeValue(6);

    m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

    return 0;
}
