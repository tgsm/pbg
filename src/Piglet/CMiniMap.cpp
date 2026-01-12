#include "engine/display/CAtomic.h"
#include "engine/display/DkDisplay.h"
#include "engine/input/IInputEngine.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CGame.h"
#include "CIcon.h"
#include "CMiniMap.h"
#include <cstdio>
#include <iostream>

CDKW_RGBA BLUE_FILTER_COLOR = CDKW_RGBA(0x32, 0x4B, 0x4B, 0xFF);
CDKW_RGBA BLUE_FILTER_COLOR_COMPLEMENT = CDKW_RGBA(0x64, 0x32, 0x00, 0xFF);

CMiniMap::CMiniMap(CGame* game) {
    m_scene = NULL;
    m_game = NULL;
    m_spline_manager = NULL;
    m_clump20 = NULL;
    m_camera = NULL;
    m_select_input = NULL;
    m_icon3C = NULL;
    m_icon40 = NULL;
    m_icon38 = NULL;
    m_icon44 = NULL;
    m_batch48 = NULL;
    m_batch4C = NULL;
    m_unkAC = 0;
    m_visible = FALSE;
    m_unk50 = 999999.9f;
    m_unk54 = 999999.9f;
    m_unk58 = 999999.9f;
    m_unk5C = -999999.9f;
    m_unk60 = -999999.9f;
    m_unk64 = -999999.9f;
    m_unk78 = 0;
    m_unk9C = 0;
    m_unkA0 = 0;
    m_unkA4 = 0;
    m_unkA8 = 0;
    m_unkB0 = 0.0f;
    m_unk90 = 0.0f;

    m_game = game;
    Create();
}

CMiniMap::~CMiniMap() {
    if (m_scene != NULL) {
        m_scene->RemoveCamera(m_camera);
        m_scene->Release();
    }

    if (m_spline_manager != NULL) {
        delete m_spline_manager;
        m_spline_manager = NULL;
    }

    if (m_icon38 != NULL) {
        delete m_icon38;
        m_icon38 = NULL;
    }
    if (m_icon3C != NULL) {
        delete m_icon3C;
        m_icon3C = NULL;
    }
    if (m_icon40 != NULL) {
        delete m_icon40;
        m_icon40 = NULL;
    }
    if (m_icon44 != NULL) {
        delete m_icon44;
        m_icon44 = NULL;
    }

    DKI::IInputEngine::DestroyInput(m_select_input);
    m_select_input = NULL;
    DKI::IInputEngine::DestroyInput(m_axeh_input);
    DKI::IInputEngine::DestroyInput(m_axevinput);

    m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch48);
    m_batch48 = NULL;
    m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch4C);
    m_batch4C = NULL;

    m_scene = NULL;
    m_game = NULL;
    m_spline_manager = NULL;
    m_clump20 = NULL;
    m_camera = NULL;
    m_select_input = NULL;
    m_icon3C = NULL;
    m_icon40 = NULL;
    m_icon38 = NULL;
    m_icon44 = NULL;
    m_batch48 = NULL;
    m_batch4C = NULL;
    m_unkAC = 0;
    m_visible = FALSE;
    m_unk50 = 999999.9f;
    m_unk54 = 999999.9f;
    m_unk58 = 999999.9f;
    m_unk5C = -999999.9f;
    m_unk60 = -999999.9f;
    m_unk64 = -999999.9f;
    m_unk78 = 0;
    m_unk9C = 0;
    m_unkA0 = 0;
    m_unkA4 = 0;
    m_unkA8 = 0;
    m_unkB0 = 0.0f;
    m_unk90 = 0.0f;
}

BOOL CMiniMap::Create() {
    DKDSP::CEngine* display_engine = DkDisplayGetEngine();
    if (display_engine == NULL) {
        return FALSE;
    }

    m_scene = display_engine->CreateScene();
    if (m_scene == NULL) {
        return FALSE;
    }

    m_spline_manager = new CMiniMapSplineMgr(m_game);
    if (m_spline_manager == NULL) {
        return FALSE;
    }

    m_select_input = DKI::IInputEngine::CreateInput("MINIMAPSELECT", 0, 18);
    m_axeh_input = DKI::IInputEngine::CreateInput("MINIMAPAXEH", 0, 14);
    m_axevinput = DKI::IInputEngine::CreateInput("MINIMAPAXEV", 0, 15);

    m_scene->CreateCamera();
    m_camera = m_scene->GetCamera(0);
    if (m_camera == NULL) {
        return FALSE;
    }

    m_camera->SetZFar(300.0f);
    m_camera->SetZNear(0.6f);
    m_camera->SetViewWindow(4.0f/3.0f, 1.0f);

    m_scene->SelectCamera(0);
    m_cam_raster_height = m_camera->GetBuffer()->GetHeight();
    m_cam_raster_width = m_camera->GetBuffer()->GetWidth();

    m_scene->SetAmbient(0.0f, 0.0f, 0.0f);

    CDKW_V3d position;
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;
    m_icon38 = new CMiniMapIcon(position, 8.0f, 8.0f, 1.0f);
    m_icon3C = new CMiniMapIcon(position, 10.0f, 10.0f, 1.0f);
    m_icon44 = new CMiniMapIcon(position, 15.0f, 15.0f, 1.0f);

    position.z = -1.0f;
    m_icon40 = new CMiniMapIcon(position, 200.0f, 100.0f, 1.0f);

    m_icon38->SetRGBA(0xFF, 0xFF, 0xFF, 0xFF);
    m_icon3C->SetRGBA(0xFF, 0xFF, 0xFF, 0xFF);

    m_batch48 = m_game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);
    m_batch4C = m_game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);

    m_unk7C = 0.0f;
    m_unk80 = 0.0f;
    m_unkB0 = 0.0f;

    return TRUE;
}

void CMiniMap::ShowFriendNPC(int a1, int a2, int a3) {
    if (a1 <= 0 || a1 > 15) {
        return;
    }

    m_game->GetCurrentMission()->m_unk4C4[(a1 - 1) / 4] |= (a2 & 0xF) << (4 + ((a1 - 1) % 4) * 8);
    m_game->GetCurrentMission()->m_unk4C4[(a1 - 1) / 4] |= (a3 & 0xF) << (0 + ((a1 - 1) % 4) * 8);
}

void CMiniMap::HideFriendNPC(int a1) {
    if (a1 <= 0 || a1 > 15) {
        return;
    }

    m_game->GetCurrentMission()->m_unk4C4[(a1 - 1) / 4] &= 0xFF << (((a1 - 1) % 4) * 8) ^ 0xFFFFFFFF;
}

// Equivalent: stack offsets
void CMiniMap::RenderDarkenRooms(F32 dt) {
    CDKW_V3d position40;
    CDKW_V3d position44;
    CDKW_V3d unk_vec(0.0f, 0.0f, 0.0f);

    if (m_icon40 == NULL || m_icon44 == NULL) {
        return;
    }

    position40 = CDKW_V3d(m_camera->GetFrame()->m_rwframe->modelling.pos);
    position40.z = -1.0f;
    m_icon40->SetPosition(position40);
    m_icon40->SetTexture(NULL);
    m_game->m_display_engine->SetRenderState((RwRenderState)10, (void*)5);
    m_game->m_display_engine->SetRenderState((RwRenderState)11, (void*)6);
    m_icon40->SetRGBA(0, 0, 0, 0.7f * (255.0f * m_unk94));
    m_icon40->Render(m_scene);

    if (m_unk94 >= 1.0f) {
        // Should be higher in the stack. Can't declare it above because the default
        // constructor is not inlined.
        CDKW_RGBAReal color = CDKW_RGBAReal(0.4f, 0.6f, 0.7f, 1.0f);
        color *= 0.5f;
        m_scene->SetAmbient(color.m_r, color.m_g, color.m_b);
        if (m_clump20 != NULL) {
            m_scene->RenderClump(m_clump20);
        }
    }
    m_game->m_display_engine->SetRenderState((RwRenderState)10, (void*)5);
    m_game->m_display_engine->SetRenderState((RwRenderState)11, (void*)6);

    if (m_unk94 >= 1.0f) {
        DKDSP::CTexture* texture = m_game->m_texture_dictionary->FindTexture("BMP_209");
        if (texture != NULL) {
            m_icon44->SetTexture(texture);
        } else {
            m_icon44->SetTexture(NULL);
        }
        m_icon44->SetRGBA(0x90, 0xBC, 0xD4, 0xFF);

        CMission* mission = m_game->GetCurrentMission();
        if (mission != NULL) {
            int iVar1 = m_game->m_unk4F54 * 100;
            for (U32 i = 1; i < mission->m_num_rooms + 1; i++) {
                int id = iVar1 + i;
                if (!mission->IsRoomOpened(i) && m_clump20 != NULL) {
                    int atomic_index = m_clump20->GetAtomicIndexFromID(id);
                    if (atomic_index >= 0) {
                        DKDSP::CAtomic* atomic = m_clump20->GetAtomic(atomic_index);
                        if (atomic != NULL) {
                            position44 = CDKW_V3d(atomic->GetFrame()->m_rwframe->modelling.pos);
                            F32 z = position44.z;
                            position44.z = -position44.y;
                            position44.y = z;
                            m_icon44->SetPosition(position44);
                            m_icon44->Render(m_scene);
                        }
                    }
                }
            }
        }
    }
    m_game->m_display_engine->SetRenderState((RwRenderState)10, (void*)5);
    m_game->m_display_engine->SetRenderState((RwRenderState)11, (void*)6);
}

void CMiniMap::RenderCookiesNbIcon(F32 dt) {
    CIcon icon1, icon2;

    CIcon::BeginRender(m_game->m_display_engine, m_camera, m_scene);

    icon2.m_width = 0.35f;
    icon2.m_height = 0.18f;
    icon2.m_x = 0.195f;
    icon2.m_y = 0.13f;
    icon2.m_texture = m_game->m_texture_dictionary->FindTexture("BMP_208");
    icon2.Render(m_batch4C, 0);

    char nb_cookies_str[10] = {};
    char x_str[2] = {};

    CDKW_RGBA text_color = CDKW_RGBA(0xFF, 0xFF, 0x99, 0xFF);
    m_game->GetGuiEngine()->SetTextColor(text_color.red, text_color.green, text_color.blue, text_color.alpha);
    int cookies = m_game->GetCurrentMission()->GetMaxCollectedNbCookies();

    sprintf(x_str, "x");
    sprintf(nb_cookies_str, "%d", cookies);
    F32 x_width = m_game->GetGuiEngine()->GetTextWidth(x_str, 0.05f, NULL);
    F32 nb_width = m_game->GetGuiEngine()->GetTextWidth(nb_cookies_str, 0.08f, NULL);
    F32 float1 = 0.195f - (0.02f + (x_width + nb_width)) / 2;
    F32 float2 = 0.01f + (0.0375f + (0.195f - (0.02f + (x_width + nb_width)) / 2));

    m_game->GetGuiEngine()->AddText(float2, 0.145f, x_str, 0.05f, NULL, 0.0f);
    m_game->GetGuiEngine()->AddText(float2 + x_width + 0.01f, 0.145f, nb_cookies_str, 0.08f, NULL, 0.0f);

    icon1.m_width = 0.075f;
    icon1.m_height = 0.1f;
    icon1.m_x = float1;
    icon1.m_y = 0.12f;
    icon1.m_texture = m_game->m_texture_dictionary->FindTexture("BMP_201");
    icon1.Render(m_batch48, 0);

    CIcon::EndRender();

    m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_camera->m_unk8->rw_camera);
}

BOOL CMiniMap::IsVisible() {
    return m_visible;
}

BOOL CMiniMap::Unload() {
    if (m_scene == NULL) {
        return FALSE;
    }

    m_scene->RemoveClump(m_clump20);
    m_spline_manager->ClearList();
    m_clump20 = NULL;
    return TRUE;
}
