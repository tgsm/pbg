#include "engine/display/CLight.h"
#include "engine/display/DkDisplay.h"
#include "CLoading.h"
#include "CGame.h"
#include "CGCNFont.h"
#include <iostream>

CLoadingCallback::CLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
    m_scene = NULL;
    m_camera = NULL;
    m_light = NULL;
    m_clump1C = NULL;
    m_clump24 = NULL;
    m_shadow_map = NULL;
    m_light2 = NULL;
    m_controller = NULL;
    m_unk30 = DKW_RGBA_WHITE;
    m_unk38 = 0;
    m_time = 0.0f;
    m_unk2C = 0.0f;
    m_unk3C = 8;
}

CLoadingCallback::~CLoadingCallback() {
    Destroy();
}

extern "C" void Rt2dDeviceSetCamera(RwCamera*);

// Equivalent: downcast to 8bit, may need to set "-bool off"
void CLoadingCallback::Create() {
    m_game->m_unk8 |= (1 << 8);
    m_game->m_gui_manager->Reset();

    if (m_scene == NULL) {
        if ((m_scene = m_game->m_display_engine->CreateScene()) == NULL) {
            return;
        }
    }
    m_scene->SetAmbient(0.4f, 0.4f, 0.4f);

    if (m_camera == NULL) {
        if ((m_camera = m_scene->CreateCamera()) == NULL) {
            return;
        }
    }
    m_camera->SetZNear(0.1f);
    m_camera->SetZFar(20.0f);
    m_camera->SetViewWindow(4.0f/3.0f, 1.0f);
    m_camera->SetFOV(45.0f, 4.0f/3.0f);
    Rt2dDeviceSetCamera(m_camera->m_wrap_camera->m_rw_camera);

    m_game->m_gui_manager->Update(1.0f/30.0f);

    if (m_light == NULL) {
        if ((m_light = m_scene->CreateLight(DKDSP::LIGHT_TYPE_DIRECTIONAL)) == NULL) {
            return;
        }
    }
    m_scene->EnableLight(m_light, 0);

    if (m_light2 == NULL) {
        if ((m_light2 = m_scene->CreateLight(DKDSP::LIGHT_TYPE_DIRECTIONAL)) == NULL) {
            return;
        }
    }
    m_scene->EnableLight(m_light2, 0);

    if (m_clump24 == NULL) {
        if ((m_clump24 = m_scene->CloneClump("Models/700_FIGHT_PATH/CAR_701.dff", NULL)) == NULL) {
            return;
        }
    }
    m_unk40 = 1;

}

F32 CLoadingCallback::UpdateTimer() {
    F32 dt = m_game->m_timer->GetDeltaTime();
    m_time += dt;
    if (m_time < 1.0f/30.0f) {
        return 0.0f;
    } else {
        m_time = dt;
        return 1.0f/30.0f;
    }
}

void CLoadingCallback::UpdateObjects(F32 dt) {
    if (m_clump1C != NULL && m_controller != NULL) {
        m_controller->Update(dt);
        m_clump1C->Update(dt);
    }
}

void CLoadingCallback::RenderObjects() {
    if (m_clump1C != NULL) {
        m_scene->RenderClump(m_clump1C);
    }
    if (m_clump24 != NULL) {
        m_scene->RenderClump(m_clump24);
    }
}

CInGameLoadingCallback::CInGameLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
    m_unkC = 0;
    m_unk10 = 0.0f;
}

CInGameLoadingCallback::~CInGameLoadingCallback() {
    Destroy();
}

CBootUpLoadingCallback::CBootUpLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
}

CBootUpLoadingCallback::~CBootUpLoadingCallback() {
    Destroy();
}

void CBootUpLoadingCallback::Destroy() {
    m_game->m_unk8 &= ~(1 << 8);
    m_unkC = 0.0f;
}

// Incomplete
void CBootUpLoadingCallback::Update() {
    if (m_game == NULL) {
        return;
    }

    m_game->ComputeDeltaTime();
    CGame::ManageReset();

    BOOL disc_cover_open;
    if (CGCNFont::m_pImage != NULL) {
        if (m_game->m_error_callback != NULL && m_game->m_error_callback->m_unk8 != 0) {
            disc_cover_open = FALSE;
            if (m_game->GetScene() != NULL) {
                m_game->GetScene()->SelectCamera(m_game->GetCamera());
                DkDisplayGetEngine()->Update();
                m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
                m_game->GetScene()->BeginRender();
            }

            if (m_game->m_error_callback->m_unk8 == 1) {
                CGCNFont::PrintText(0, 220, "The Disc Cover is open.");
                CGCNFont::PrintText(0, 250, "If you want to continue the game,");
                CGCNFont::PrintText(0, 280, "please close the Disc Cover.");
                disc_cover_open = TRUE;
            }
            if (m_game->m_error_callback->m_unk8 == 3 || m_game->m_error_callback->m_unk8 == 4) {
                CGCNFont::PrintText(0, 220, " ");
                CGCNFont::PrintText(0, 250, "Please insert the Piglet's BIG GAME Game Disc.");
                CGCNFont::PrintText(0, 280, " ");
                disc_cover_open = TRUE;
            }

            if (m_game->GetScene() != NULL) {
                m_game->GetScene()->EndRender();
                m_game->GetScene()->Flip(0);
            }

            if (disc_cover_open) {
                return;
            }
        }
    }


    CDKW_RGBA color;
    color.alpha = 0xFF;
    color.blue = 0xFF;
    color.green = 0xFF;
    color.red = 0xFF;
    if (m_unkC < 2.0f) {
        F32 calc = m_unkC * 255.0f;
        calc *= 0.5f;
        color.red = calc;
        color.green = calc;
        color.blue = calc;
    } else if (m_unkC < 5.0f) {
        color.red = 0xFF;
        color.green = 0xFF;
        color.blue = 0xFF;
    } else if (m_unkC < 7.0f) {
        F32 calc = (255.0f * (m_unkC - 5.0f));
        calc *= 0.5f;
        color.red = 0xFF - (U8)calc;
        color.green = 0xFF - calc;
        color.blue = 0xFF - calc;
    } else {
        color.red = 0;
        color.green = 0;
        color.blue = 0;
    }

    F32 width = m_game->GetCamera()->GetBuffer()->GetWidth();
    F32 height = m_game->GetCamera()->GetBuffer()->GetHeight();

    {
        F32 text_width = m_game->m_gui_engine->GetTextWidth("!!!Caution!!!", 16.0f / height, NULL);
        text_width = (width - (width * text_width));
        m_game->m_gui_engine->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->m_gui_engine->AddText((0.5f * (width - text_width)) / width, 240.0f / height, "!!!Caution!!!", 16.0f / height, NULL, 0.0f);
    }

    {
        F32 text_width = m_game->m_gui_engine->GetTextWidth("This game supports an Autosave feature to Memory Card Slot A.", 16.0f / height, NULL);
        text_width = width - (width * text_width);
        m_game->m_gui_engine->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->m_gui_engine->AddText((0.5f * (width - text_width)) / width, 260.0f / height, "This game supports an Autosave feature to Memory Card Slot A.", 16.0f / height, NULL, 0.0f);
    }

    {
        F32 text_width = m_game->m_gui_engine->GetTextWidth("Do not turn the power off or remove the Memory Card while saving.", 16.0f / height, NULL);
        text_width = (width * text_width);
        m_game->m_gui_engine->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->m_gui_engine->AddText((width - text_width) / width, 280.0f / height, "Do not turn the power off or remove the Memory Card while saving.", 16.0f / height, NULL, 0.0f);
    }

    m_game->GetGuiManager()->Update(m_game->GetDeltaTime());
    m_unkC += m_game->GetDeltaTime();

    m_game->GetScene()->SelectCamera(m_game->GetCamera());
    m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);

    m_game->GetScene()->BeginRender();
    m_game->GetGuiManager()->Render(m_game->GetDeltaTime());
    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flush();
    m_game->GetScene()->Flip(0);
}

void CPreBootUpLoadingCallback::Create() {

}

void CPreBootUpLoadingCallback::Destroy() {
}

void CPreBootUpLoadingCallback::Update() {
    CGame::ManageReset();

    if (CGCNFont::m_pImage != NULL) {
        if (m_game->m_error_callback != NULL) {
            if (m_game->GetScene() != NULL) {
                m_game->GetScene()->SelectCamera(m_game->GetCamera());
                DkDisplayGetEngine()->Update();
                m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
                m_game->GetScene()->BeginRender();
            }

            if (m_game->m_error_callback->m_unk8 == 1) {
                CGCNFont::PrintText(0, 220, "The Disc Cover is open.");
                CGCNFont::PrintText(0, 250, "If you want to continue the game,");
                CGCNFont::PrintText(0, 280, "please close the Disc Cover.");
            }
            if (m_game->m_error_callback->m_unk8 == 3 || m_game->m_error_callback->m_unk8 == 4) {
                CGCNFont::PrintText(0, 220, " ");
                CGCNFont::PrintText(0, 250, "Please insert the Piglet's BIG GAME Game Disc.");
                CGCNFont::PrintText(0, 280, " ");
            }

            if (m_game->GetScene() != NULL) {
                m_game->GetScene()->EndRender();
                m_game->GetScene()->Flip(0);
            }
        }
    }
}

CPreBootUpLoadingCallback::CPreBootUpLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
}

void CVideoLoadingCallback::Create() {

}

void CVideoLoadingCallback::Destroy() {

}

void CVideoLoadingCallback::Update() {
    CGame::ManageReset();

    if (m_game->m_error_callback != NULL) {
        if (m_game->m_error_callback->m_unk8 == 1) {
            if (!m_game->GetGuiManager()->IsActive("NO_DISC_COVER_NO_DISC_COVER")) {
                m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
                m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);

                m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
                m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->GetAnim()->GotoFrameLabel("NO_DISC_COVER");

                m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
                m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->GetAnim()->Play();
                m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 1);
                m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 1);
            }
        } else if (m_game->m_error_callback->m_unk8 == 3 || m_game->m_error_callback->m_unk8 == 4) {
            if (!m_game->GetGuiManager()->IsActive("NO_PIGLET_DISC_NO_PIGLET_DISC")) {
                m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 0);
                m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 0);

                m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
                m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->GetAnim()->GotoFrameLabel("NO_PIGLET_DISC");

                m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
                m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->GetAnim()->Play();
                m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 1);
                m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 1);
            }
        } else {
            m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
            m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 0);
            m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 0);

            m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
            m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
        }
    } else {
        if (m_game->GetGuiManager()->IsActive("NO_DISC_COVER_NO_DISC_COVER")) {
            m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
            m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 0);
            m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 0);
        }

        if (m_game->GetGuiManager()->IsActive("NO_PIGLET_DISC_NO_PIGLET_DISC")) {
            m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
            m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
        }
    }

    m_game->GetGuiManager()->Update(1.0f/30.0f);
    if (m_game->m_display_engine->Update()) {
        m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
        m_game->GetScene()->BeginRender();
        m_game->GetGuiManager()->Render(1.0f/30.0f);
        m_game->GetScene()->EndRender();
        m_game->GetScene()->Flip(0);
    }
}

CVideoLoadingCallback::CVideoLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
}
