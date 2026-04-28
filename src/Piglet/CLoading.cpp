#include "engine/display/CLight.h"
#include "engine/display/DkDisplay.h"
#include "engine/wrap/DKW_Material.h"
#include "CLoading.h"
#include "CGame.h"
#include "CGCNFont.h"
#include <math.h>
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

void CLoadingCallback::Create() {
    m_game->m_unk8 |= (1 << 8);
    m_game->GetGuiManager()->Reset();

    if (m_scene == NULL) {
        if ((m_scene = m_game->GetDisplayEngine()->CreateScene()) == NULL) {
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

    m_game->GetGuiManager()->Update(1.0f/30.0f);

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

void CLoadingCallback::Destroy() {
    int i;
    int j;

    m_game->m_unk8 &= ~(1 << 8);
    Rt2dDeviceSetCamera(m_game->GetCamera()->m_wrap_camera->m_rw_camera);

    m_game->GetGuiManager()->Update(1.0f/30.0f);

    m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
    m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 0);
    m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 0);

    m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
    m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
    m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);

    m_game->GetGuiManager()->Reset();

    if (m_clump24 != NULL) {
        int nb_atomics = m_clump24->GetNumberOfAtomics();
        for (i = 0; i < nb_atomics; i++) {
            DKDSP::CAtomic* atomic = m_clump24->GetAtomic(i);
            atomic->GetNumberOfMaterials(); // unused
            CDKW_Geometry* geo = atomic->GetGeometry()->GetDkWrapGeometry();
            if (geo->Lock()) {
                geo->m_rw_geometry->flags |= (1 << 6);

                int nb_materials = geo->GetNumberOfMaterials();
                for (j = 0; j < nb_materials; j++) {
                    CDKW_Material* mat = CDKW_Material::GetInstance(geo->m_rw_geometry->matList.materials[j]);
                    mat->m_rw_material->color = m_unk34;
                    RpMaterialSetTexture(mat->m_rw_material, m_unk38);
                }

                RpGeometryUnlock(geo->m_rw_geometry);
            }
        }
    }

    if (m_shadow_map != NULL) {
        m_scene->RemoveShadowMap(m_shadow_map);
        m_shadow_map = NULL;
    }

    if (m_light2 != NULL) {
        m_scene->RemoveLight(m_light2);
        m_light2 = NULL;
    }

    if (m_clump24 != NULL) {
        m_scene->RemoveClump(m_clump24);
        m_clump24 = NULL;
    }

    if (m_controller != NULL) {
        m_game->m_anim_dictionary->RemoveController(m_controller->GetName());
        m_controller = NULL;
    }

    if (m_clump1C != NULL) {
        m_scene->RemoveClump(m_clump1C);
        m_clump1C = NULL;
    }

    if (m_camera != NULL) {
        m_scene->RemoveCamera(m_camera);
        m_camera = NULL;
    }

    if (m_light != NULL) {
        m_scene->RemoveLight(m_light);
        m_light = NULL;
    }

    if (m_scene != NULL) {
        m_game->GetDisplayEngine()->DestroyScene(m_scene);
        m_scene = NULL;
    }
}

void CLoadingCallback::Update() {
    F32 dt;
    F32 dVar17;
    F32 dVar16;
    F32 dVar15;
    F32 dVar9;
    F32 width;
    F32 height;
    int i;

    if (m_game != NULL && m_scene != NULL && m_camera != NULL) {
        CGame::ManageReset();

        dt = UpdateTimer();
        if (dt != 0.0f) {
            m_unk2C += dt;
            UpdateObjects(dt);

            if (m_unk40 == TRUE && m_game->GetScreenEffect() != NULL) {
                m_game->GetScreenEffect()->Update(dt);
            }

            CDKW_RGBA text_color = m_unk30 * 0.2f;
            text_color.alpha = 0x40;
            if (text_color.red == 0) {
                text_color.red = 0xFF;
                text_color.green = 0xFF;
                text_color.blue = 0xFF;
                text_color.alpha = 0x3F;
            }

            width = m_game->GetCamera()->GetBuffer()->GetWidth();
            height = m_game->GetCamera()->GetBuffer()->GetHeight();

            char loading_text[32] = "loading...";
            char loading_char[4];

            dVar15 = height - 86.0f;
            loading_char[1] = '\0';
            dVar16 = 0.0f;
            int len = RwEngineInstance->stringFuncs.rwstrlen(loading_text);
            for (i = 0; i < len; i++) {
                *loading_char = loading_text[i];
                if (*loading_char == '\0') {
                    break;
                }

                F32 tmp = cos((4.0f * m_unk2C) - (15.0f * i));
                dVar9 = 30.0f + (6.0f * tmp);
                dVar16 += width * m_game->GetGuiEngine()->GetTextWidth(loading_char, dVar9 / height, NULL);
                dVar16 += dVar9 / 12.0f;
            }

            dVar17 = (width - dVar16) / 2;
            dVar16 = 0.0f;

            for (i = 0; i < len; i++) {
                *loading_char = loading_text[i];
                if (*loading_char == '\0') {
                    break;
                }

                F32 tmp = cos((4.0f * m_unk2C) - (15.0f * i));
                dVar9 = 30.0f + (6.0f * tmp);
                dVar17 += dVar16 + (dVar9 / 12.0f);

                dVar16 = m_game->GetGuiEngine()->GetTextWidth(loading_char, dVar9 / height, NULL);
                dVar16 = width * dVar16;
                m_game->GetGuiEngine()->SetTextColor(text_color.red, text_color.green, text_color.blue, text_color.alpha);
                m_game->GetGuiEngine()->AddText(dVar17 / width, dVar15 / height, loading_char, dVar9 / height, NULL, 0.0f);
            }

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

            m_game->GetGuiManager()->Update(dt);

            if (m_game->GetDisplayEngine()->Update()) {
                if (m_shadow_map != NULL) {
                    m_scene->UpdateShadowMaps();
                }
                m_scene->SelectCamera(m_camera);
                m_scene->Clear(3, m_unk30.red / 255.0f, m_unk30.green / 255.0f, m_unk30.blue / 255.0f);

                m_scene->BeginRender();
                RenderObjects();
                if (m_unk40 == TRUE && m_game->GetScreenEffect() != NULL) {
                    m_game->GetScreenEffect()->Render(NULL);
                }
                m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_camera->m_wrap_camera->m_rw_camera);
                m_game->GetGuiManager()->Render(1.0f/30.0f);
                m_scene->EndRender();

                m_scene->Flip(0);
            }
        }
    }
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
    m_batch = NULL;
    m_unk10 = 0.0f;
}

CInGameLoadingCallback::~CInGameLoadingCallback() {
    Destroy();
}

void CInGameLoadingCallback::Create() {
    if (m_batch == NULL) {
        m_unk10 = 0.0f;
        m_unk14 = 0.0f;
        m_batch = m_game->GetDisplayEngine()->GetImmediate()->CreateBatch2D(4, 0);
    }
}

void CInGameLoadingCallback::Destroy() {
    if (m_batch != NULL) {
        m_unk10 = 0.0f;
        m_unk14 = 0.0f;

        m_game->GetDisplayEngine()->GetImmediate()->RemoveBatch2D(m_batch);
        m_batch = NULL;

        m_game->GetGuiManager()->GetGuiPtr("NO_DISC_COVER_NO_DISC_COVER")->menu->Reset();
        m_game->GetGuiManager()->SetActive("NO_DISC_COVER_NO_DISC_COVER", 0);
        m_game->GetGuiManager()->SetVisible("NO_DISC_COVER_NO_DISC_COVER", 0);

        m_game->GetGuiManager()->GetGuiPtr("NO_PIGLET_DISC_NO_PIGLET_DISC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
        m_game->GetGuiManager()->SetVisible("NO_PIGLET_DISC_NO_PIGLET_DISC", 0);
    }
}

extern "C" {
extern float RwIm2DGetNearScreenZ(void);
}

// Incomplete
void CInGameLoadingCallback::Update() {
    F32 height, width;

    if (m_game == NULL) {
        return;
    }

    CGame::ManageReset();

    CDKW_RGBA fade_color;
    fade_color.alpha = 0xFF;
    fade_color.blue = 0xFF;
    fade_color.green = 0xFF;
    fade_color.red = 0xFF;

    width = m_game->GetCamera()->GetBuffer()->GetWidth();
    height = m_game->GetCamera()->GetBuffer()->GetHeight();

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

    if (m_game->GetScreenEffect() != NULL && m_game->m_unk4F54 < 7) {
        m_game->GetScreenEffect()->Update(m_game->GetDeltaTime());
    }

    m_game->m_sound_engine->BeginUpdate();
    m_game->m_sound_engine->EndUpdate();

    m_game->GetGuiManager()->Update(m_game->GetDeltaTime());

    fade_color = m_game->ComputeGameFadeColor();
    m_game->GetScene()->Clear(3, fade_color.red / 255.0f, fade_color.green / 255.0f, fade_color.blue / 255.0f);
    m_game->GetScene()->BeginRender();

    F32 time = (F32)m_game->m_timer->GetTime();
    F32 dVar11 = time - m_unk10;
    m_unk10 = time;
    m_unk14 += 180.0f * (dVar11);

    if (m_batch != NULL) {
        F32 dVar18 = 0.9f * width;
        F32 dVar17 = 0.84f * height;
        F32 dVar13 = (0.12f * width) * 0.5f;
        F32 dVar16 = (0.16f * height) * 0.5f;

        m_game->GetScene()->SelectCamera(m_game->GetCamera());
        m_game->GetCamera()->GetZNear(); // unused
        F32 dVar11 = RwIm2DGetNearScreenZ();
        RwIm2DVertex* vertices = m_batch->GetVerticeArray();
        F32 dVar14 = -dVar16;
        F32 dVar19 = -dVar13;

        F32 x__ = (3.1415927f * m_unk14) / 180.0f;
        F32 dVar15 = cos(x__);
        F32 dVar12 = sin(x__);
        F32 unk = dVar14 * dVar12;

        vertices[0].x = dVar18 + (dVar13 * dVar15) + (dVar16 * dVar12);
        vertices[0].y = dVar17 + (dVar19 * dVar12) + (dVar16 * dVar15);
        vertices[0].z = dVar11;
        const F32& v0u = 1.0f;
        vertices[0].u = v0u;
        const F32& v0v = 1.0f;
        vertices[0].v = v0v;
        vertices[0].r = 0xFF;
        vertices[0].g = 0xFF;
        vertices[0].b = 0xFF;
        vertices[0].a = 0xFF;

        vertices[1].x = dVar18 + (dVar13 * dVar15) + unk;
        vertices[1].y = dVar17 + (dVar19 * dVar12) + (dVar14 * dVar15);
        vertices[1].z = dVar11;
        const F32& v1u = 1.0f;
        vertices[1].u = v1u;
        const F32& v1v = 0.0f;
        vertices[1].v = v1v;
        vertices[1].r = 0xFF;
        vertices[1].g = 0xFF;
        vertices[1].b = 0xFF;
        vertices[1].a = 0xFF;

        vertices[2].x = dVar18 + (dVar19 * dVar15) + unk;
        vertices[2].y = dVar17 + (-dVar19 * dVar12) + (dVar14 * dVar15);
        vertices[2].z = dVar11;
        const F32& v2u = 0.0f;
        vertices[2].u = v2u;
        const F32& v2v = 0.0f;
        vertices[2].v = v2v;
        vertices[2].r = 0xFF;
        vertices[2].g = 0xFF;
        vertices[2].b = 0xFF;
        vertices[2].a = 0xFF;

        vertices[3].x = dVar18 + (dVar19 * dVar15) + (dVar16 * dVar12);
        vertices[3].y = dVar17 + (-dVar19 * dVar12) + (dVar16 * dVar15);
        vertices[3].z = dVar11;
        const F32& v3u = 0.0f;
        vertices[3].u = v3u;
        const F32& v3v = 1.0f;
        vertices[3].v = v3v;
        vertices[3].r = 0xFF;
        vertices[3].g = 0xFF;
        vertices[3].b = 0xFF;
        vertices[3].a = 0xFF;

        DKDSP::CTexture* texture = m_game->m_texture_dictionary->FindTexture("BMP_310");
        m_game->GetDisplayEngine()->SetRenderState((RwRenderState)12, (void*)1);
        m_game->GetDisplayEngine()->SetRenderState((RwRenderState)10, (void*)5);
        m_game->GetDisplayEngine()->SetRenderState((RwRenderState)11, (void*)6);
        if (texture != NULL) {
            m_game->GetDisplayEngine()->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            m_game->GetDisplayEngine()->SetRenderState((RwRenderState)1, NULL);
        }
        m_batch->Render(rwPRIMTYPETRIFAN);
    }

    if (m_game->GetScreenEffect() != NULL && m_game->m_unk4F54 < 7) {
        m_game->GetScreenEffect()->Render(NULL);
    }

    m_game->GetGuiManager()->Render(m_game->GetDeltaTime());
    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flush();
    m_game->GetScene()->Flip(0);
}

CBootUpLoadingCallback::CBootUpLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
}

CBootUpLoadingCallback::~CBootUpLoadingCallback() {
    Destroy();
}

void CBootUpLoadingCallback::Create() {
    m_game->m_unk8 |= (1 << 8);
    m_unkC = 0.0f;

    m_game->m_timer->Reset();
    m_game->ComputeDeltaTime();
#ifdef VERSION_GPLP9G
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
#endif

    while (m_unkC < 4.0f) {
        m_game->ComputeDeltaTime();
        CGame::ManageReset();

        F32 width = m_game->GetCamera()->GetBuffer()->GetWidth();
        F32 height = m_game->GetCamera()->GetBuffer()->GetHeight();

        {
            F32 text_width = m_game->GetGuiEngine()->GetTextWidth("Licensed by Nintendo", 16.0f / height, NULL);
            text_width = width * text_width;
            m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);
            m_game->GetGuiEngine()->AddText((((width - text_width) / 2) / width), 240.0f / height, "Licensed by Nintendo", 16.0f / height, NULL, 0.0f);
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

    m_unkC = 0.0f;
}

void CBootUpLoadingCallback::Destroy() {
    m_game->m_unk8 &= ~(1 << 8);
    m_unkC = 0.0f;
}

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
        color.red = (m_unkC * 255.0f) / 2;
        color.green = (m_unkC * 255.0f) / 2;
        color.blue = (m_unkC * 255.0f) / 2;
    } else if (m_unkC < 5.0f) {
        color.red = 0xFF;
        color.green = 0xFF;
        color.blue = 0xFF;
    } else if (m_unkC < 7.0f) {
        color.red = 0xFF - (U8)(((m_unkC - 5.0f) * 255.0f) / 2);
        color.green = 0xFF - (U8)(((m_unkC - 5.0f) * 255.0f) / 2);
        color.blue = 0xFF - (U8)(((m_unkC - 5.0f) * 255.0f) / 2);
    } else {
        color.red = 0;
        color.green = 0;
        color.blue = 0;
    }

    F32 width = m_game->GetCamera()->GetBuffer()->GetWidth();
    F32 height = m_game->GetCamera()->GetBuffer()->GetHeight();

    {
        F32 text_width = m_game->GetGuiEngine()->GetTextWidth("!!!Caution!!!", 16.0f / height, NULL);
        text_width = (width - (width * text_width)) / 2;
        m_game->GetGuiEngine()->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->GetGuiEngine()->AddText(text_width / width, 240.0f / height, "!!!Caution!!!", 16.0f / height, NULL, 0.0f);
    }

    {
        F32 text_width = m_game->GetGuiEngine()->GetTextWidth("This game supports an Autosave feature to Memory Card Slot A.", 16.0f / height, NULL);
        text_width = (width * text_width);
        m_game->GetGuiEngine()->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->GetGuiEngine()->AddText(((width - text_width) / 2) / width, 260.0f / height, "This game supports an Autosave feature to Memory Card Slot A.", 16.0f / height, NULL, 0.0f);
    }

    {
        F32 text_width = m_game->GetGuiEngine()->GetTextWidth("Do not turn the power off or remove the Memory Card while saving.", 16.0f / height, NULL);
        text_width = (width * text_width);
        m_game->GetGuiEngine()->SetTextColor(color.red, color.green, color.blue, color.alpha);
        m_game->GetGuiEngine()->AddText(((width - text_width) / 2) / width, 280.0f / height, "Do not turn the power off or remove the Memory Card while saving.", 16.0f / height, NULL, 0.0f);
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
    if (m_game->GetDisplayEngine()->Update()) {
#ifdef VERSION_GPLP9G
        m_game->GetScene()->SelectCamera(m_game->GetCamera());
#endif
        m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
        m_game->GetScene()->BeginRender();
        m_game->GetGuiManager()->Render(1.0f/30.0f);
        m_game->GetScene()->EndRender();
#ifdef VERSION_GPLP9G
        m_game->GetScene()->Flush();
#endif
        m_game->GetScene()->Flip(0);
    }
}

CVideoLoadingCallback::CVideoLoadingCallback(CGame* game) : CBaseLoadingCallback(game) {
    m_game = game;
}
