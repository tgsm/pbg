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
    m_time = 0.0f;
    m_unk14 = 0;

    m_game = game;
    m_time = 0.0f;
    U32 rf_unkC = m_game->GetResourceFactory()->m_unkC;
    m_game->GetResourceFactory()->m_unkC = 0;
    m_game->LoadConfigFile("MENUS/STARTSCR/STARTSCR.XMD");
    m_game->GetResourceFactory()->m_unkC = rf_unkC;

#ifdef VERSION_GPLP9G
    m_time = 0.0f;

    m_game->m_timer->Reset();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();

    while (m_time < 8.0f) {
        m_game->ComputeDeltaTime();
        m_game->ManageReset();

        F32 y;
        F32 text_width;

        F32 screen_width = m_game->GetCamera()->GetBuffer()->GetWidth();
        F32 screen_height = m_game->GetCamera()->GetBuffer()->GetHeight();
        y = 160.0f;

        BOOL no_pad = m_game->GetGuiManager()->IsActive("NOPAD_NOPAD");
        BOOL no_disc_cover = m_game->GetGuiManager()->IsActive("NO_DISC_COVER_NO_DISC_COVER");
        BOOL no_piglet_disc = m_game->GetGuiManager()->IsActive("NO_PIGLET_DISC_NO_PIGLET_DISC");
        if (!no_pad && !no_disc_cover && !no_piglet_disc) {
            for (int i = 0; i < 12; i++) {
                char* text = NULL;
                switch (i) {
                    case 0:
                        text = " ";
                        break;
                    case 1:
                        text = "(c) 2003 Disney.";
                        break;
                    case 2:
                        text = "Based on the 'Winnie the Pooh' works,";
                        break;
                    case 3:
                        text = "by A.A. Milne and E.H. Shepard.";
                        break;
                    case 4:
                        text = "Doki Denki Studio, Doki Denki and the Doki Denki logo";
                        break;
                    case 5:
                        text = "are trademarks of Doki Denki Studio.";
                        break;
                    case 6:
                        text = "Gotham Games and the Gotham Games logo ";
                        break;
                    case 7:
                        text = "are trademarks of Take 2 Interactive Software.";
                        break;
                    case 8:
                        text = "All other trademarks are the property of their respective owners.";
                        break;
                    case 9:
                        text = "All rights reserved.";
                        break;
                    case 10:
                        text = " ";
                        break;
                    case 11:
                        text = " ";
                        break;
                }

                text_width = screen_width * m_game->GetGuiEngine()->GetTextWidth(text, 16.0f / screen_height, NULL);
                m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);
                F32 x = (screen_width - text_width) / 2.0f;
                m_game->GetGuiEngine()->AddText(x / screen_width, y / screen_height, text, 16.0f / screen_height, NULL, 0.0f);

                y += 17.0f;
            }

            m_time += m_game->GetDeltaTime();
        }

        DKI::IInputEngine::Update(m_game->GetDeltaTime());
        m_game->GetGuiManager()->Update(m_game->GetDeltaTime());

        m_game->GetScene()->SelectCamera(m_game->GetCamera());
        m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
        m_game->GetScene()->BeginRender();
        m_game->GetGuiManager()->Render(m_game->GetDeltaTime());
        m_game->GetScene()->EndRender();
        m_game->GetScene()->Flush();
        m_game->GetScene()->Flip(0);
    }

    m_time = 0.0f;
#endif

    m_game->GetGuiManager()->Reset();
    m_game->GetGuiManager()->GetGuiPtr("TITLE_TITLE")->menu->Reset();
    m_game->GetGuiManager()->SetActive("TITLE_TITLE", 1);
    m_game->GetGuiManager()->SetVisible("TITLE_TITLE", 1);

    Rt2dCTMSetIdentity();

    m_game->FadeInit(1.0f, CGame::FADE_TYPE_0, 0, 0, 0, 0.0f);

    m_game->m_unk5038 = 1;
}

CGamePartStartScreen::~CGamePartStartScreen() {
    m_game->m_fx_manager->Clear();
    m_game->m_sound_engine->DeleteAllSounds();
    m_game->GetGuiManager()->UnLoadLevel(0);
    m_game->GetResourceFactory()->UnloadResources(0);
}

U32 CGamePartStartScreen::NextFrame() {
    m_game->GetDisplayEngine()->Update();

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

void CGamePartStartScreen::Update(F32 dt) {
    m_time += dt;

    m_game->m_sound_engine->BeginUpdate();
    m_game->m_sound_engine->EndUpdate();

    m_game->GetGuiManager()->Update(dt);
}

void CGamePartStartScreen::Render(F32 dt) {
    m_game->GetScene()->SelectCamera(m_game->GetCamera());

    m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);

    m_game->GetScene()->BeginRender();
    m_game->GetGuiManager()->Render(dt);
    m_game->m_fx_manager->Render();
    m_game->GetScene()->Flush();
    m_game->RenderFade();
    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flip(0);
}

U32 CGamePartStartScreen::NextFrameExit() {
    m_game->GetGuiManager()->SetActive("TITLE_TITLE", 0);
    m_game->GetGuiManager()->SetVisible("TITLE_TITLE", 0);

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
