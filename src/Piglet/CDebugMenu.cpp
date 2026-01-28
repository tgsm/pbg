#include "engine/input/IInputEngine.h"
#include "engine/display/DkDisplay.h"
#include "engine/xmd/CXmdFile.h"
#include "CDebugMenu.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

extern "C" {
extern void Rt2dDeviceSetFlat(float);
extern void Rt2dDeviceSetCamera(RwCamera*);
}

CDebugMenu::CDebugMenu(CGame* game, char* xmd_path)
    : m_uv1(0.0f, 1.0f),
      m_uv2(1.0f, 1.0f),
      m_uv3(1.0f, 0.0f),
      m_uv4(0.0f, 0.0f)
{
    m_input_valid = DKI::IInputEngine::CreateInput("DM_VALID", 0, 19);
    m_input_cancel = DKI::IInputEngine::CreateInput("DM_CANCEL", 0, 20);
    m_input_ncontrol = DKI::IInputEngine::CreateInput("DM_NCONTROL", 0, 3);
    m_input_pcontrol = DKI::IInputEngine::CreateInput("DM_PCONTROL", 0, 2);
    m_input_nvalue = DKI::IInputEngine::CreateInput("DM_NVALUE", 0, 0);
    m_input_pvalue = DKI::IInputEngine::CreateInput("DM_PVALUE", 0, 1);

    m_display_engine = DkDisplayGetEngine();
    m_game = game;
    m_texture = NULL;
    m_path = NULL;
    m_brush = NULL;

    Rt2dDeviceSetFlat(1.0f);

    DKDSP::CCamera* camera = m_game->GetCamera();
    SDKW_Frustum frustum = camera->GetFrustum();
    frustum.unk8 = 0.5f;
    frustum.unkC = -0.5f;
    frustum.unk0 = -0.5f;
    frustum.unk4 = 0.5f;
    camera->SetFrustum(frustum);
    Rt2dDeviceSetCamera(camera->m_wrap_camera->m_rw_camera);

    m_cam_raster_width = m_game->GetCamera()->GetBuffer()->GetWidth();
    m_cam_raster_height = m_game->GetCamera()->GetBuffer()->GetHeight();

    m_current_control_no = 0;

    void* xmd_data = m_game->GetResourceFactory()->LoadPureFile(xmd_path, NULL);
    DkXmd::CXmdFile xmd;
    xmd.Parse(xmd_data);

    DkXmd::CChunkIterator iter;
    if (xmd.m_chunk_iterator.GetFirstChildChunk(iter)) {
        do {
            if (strcmp(iter.GetName(), "Resolution") == 0) {
                DkXmd::CChunkIterator resolution_iter;
                if (iter.GetFirstChildChunk(resolution_iter)) {
                    do {
                        if (strcmp(resolution_iter.GetName(), "Width") == 0) {
                            m_resolution_width = resolution_iter.GetS32Value();
                        } else if (strcmp(resolution_iter.GetName(), "Height") == 0) {
                            m_resolution_height = resolution_iter.GetS32Value();
                        }
                    } while (resolution_iter.GetNextSiblingChunk(resolution_iter));
                }
            } else if (strcmp(iter.GetName(), "UsingTextureDictionary") == 0) {
                m_game->GetResourceFactory()->LoadResource(6, iter.GetStringValue());
            } else if (strcmp(iter.GetName(), "Background") == 0) {
                int x_, y_, width_, height_;
                DkXmd::CChunkIterator background_iter;
                BOOL has_x = FALSE;
                if (iter.GetFirstChildChunk(background_iter)) {
                    do {
                        if (strcmp(background_iter.GetName(), "x") == 0) {
                            x_ = background_iter.GetS32Value();
                            has_x = TRUE;
                        } else if (strcmp(background_iter.GetName(), "y") == 0) {
                            y_ = background_iter.GetS32Value();
                        } else if (strcmp(background_iter.GetName(), "width") == 0) {
                            width_ = background_iter.GetS32Value();
                        } else if (strcmp(background_iter.GetName(), "height") == 0) {
                            height_ = background_iter.GetS32Value();
                        } else if (strcmp(background_iter.GetName(), "Image") == 0) {
                            m_texture = m_display_engine->GetTextureDictionary()->FindTexture(background_iter.GetStringValue());
                            if (m_texture == NULL) {
                                m_texture = m_display_engine->GetTextureDictionary()->LoadTextureFromFile(background_iter.GetStringValue(), 0x10, 0x100);
                            }
                        }
                    } while (background_iter.GetNextSiblingChunk(background_iter));
                }

                if (has_x) {
                    int res_height = GetResolutionHeight();
                    int res_width = GetResolutionWidth();

                    F32 x = (F32)x_ / (F32)res_width;
                    F32 y = (F32)y_ / (F32)res_height;
                    F32 width = (F32)width_ / (F32)res_width;
                    F32 height = (F32)height_ / (F32)res_height;

                    m_path = new CDKW_2dPath;
                    m_path->Lock();
                    m_path->Rect(x, 1.0f - y, width, -height);
                    m_path->Unlock();

                    m_brush = new CDKW_2dBrush;
                    m_brush->Create();
                    m_brush->SetRGBA(&DKW_RGBA_WHITE, &DKW_RGBA_WHITE, &DKW_RGBA_WHITE, &DKW_RGBA_WHITE);
                    m_brush->SetTexture(m_texture->GetDkWrapTexture());
                    m_brush->SetUV(&m_uv1, &m_uv2, &m_uv3, &m_uv4);
                }
            } else if (strcmp(iter.GetName(), "ControlList") == 0) {
                DkXmd::CChunkIterator list_chunk_iter;
                if (iter.GetFirstChildChunk(list_chunk_iter)) {
                    do {
                        CMenuControl* control = new CMenuControl(this);
                        if (control->Init(list_chunk_iter)) {
                            AS_ULONG_VECTOR_HACK(m_controls).insert(reinterpret_cast<unsigned long*>(m_controls.data() + m_controls.size()), 1, reinterpret_cast<unsigned long>(control));
                            // AS_ULONG_VECTOR_HACK(m_controls).push_back(reinterpret_cast<unsigned long>(control_value));
                        } else {
                            delete control;
                        }
                    } while (list_chunk_iter.GetNextSiblingChunk(list_chunk_iter));
                }
            }
        } while (iter.GetNextSiblingChunk(iter));
    }

    delete[] xmd_data;
}

CDebugMenu::~CDebugMenu() {
    if (m_input_valid != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_valid);
    }
    if (m_input_cancel != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_cancel);
    }
    if (m_input_ncontrol != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_ncontrol);
    }
    if (m_input_pcontrol != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_pcontrol);
    }
    if (m_input_nvalue != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_nvalue);
    }
    if (m_input_pvalue != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_pvalue);
    }

    if (m_path != NULL) {
        delete m_path;
        m_path = NULL;
    }

    if (m_brush != NULL) {
        delete m_brush;
        m_brush = NULL;
    }

    for (U32 i = 0; i < m_controls.size(); i++) {
        delete m_controls[i];
    }
    // FIXME: m_controls.clear();
    m_controls.m_size = 0;
    if (m_controls.m_data != NULL) {
        delete m_controls.m_data;
    }
    m_controls.m_data = NULL;
    m_controls.m_capacity = 0;
}

CMenuControl* CDebugMenu::GetControl(char* name) {
    for (U32 i = 0; i < m_controls.size(); i++) {
        if (strcmp(name, m_controls[i]->m_id) == 0) {
            return m_controls[i];
        }
    }

    return NULL;
}

void CDebugMenu::RemoveControl(char* name) {
    for (std::vector<CMenuControl*>::iterator iter = m_controls.begin(); iter != m_controls.end(); iter++) {
        if (strcmp((*iter)->m_id, name) == 0) {
            delete *iter;
            AS_ULONG_VECTOR_HACK(m_controls).erase(reinterpret_cast<unsigned long*>(iter));
            return;
        }
    }
}

U32 CDebugMenu::UpdateAndDisplay() {
    U32 ret = 0;

    if (m_input_ncontrol->GetState().m_unk0 == TRUE) {
        if (m_current_control_no + 1 < (int)m_controls.size()) {
            m_current_control_no++;
        } else {
            m_current_control_no = 0;
        }
    }

    if (m_input_pcontrol->GetState().m_unk0 == TRUE) {
        if (m_current_control_no - 1 >= 0) {
            m_current_control_no--;
        } else {
            m_current_control_no = m_controls.size() - 1;
        }
    }

    if (m_input_nvalue->GetState().m_unk0 == TRUE) {
        m_controls[m_current_control_no]->NextValue();
    }

    if (m_input_pvalue->GetState().m_unk0 == TRUE) {
        m_controls[m_current_control_no]->PrevValue();
    }

    if (m_input_valid->GetState().m_unk0 == TRUE) {
        ret = 1;
    }
    if (m_input_cancel->GetState().m_unk0 == TRUE) {
        ret = 2;
    }

    DKDSP::CScene* scene = m_game->GetScene();
    scene->func80();
    scene->SelectCamera(m_game->GetCamera());
    scene->Clear(1, 0.0f, 0.0f, 0.0f);
    if (scene->BeginRender()) {
        m_display_engine->SetRenderState((RwRenderState)6, (void*)0);
        m_display_engine->SetRenderState((RwRenderState)8, (void*)0);
        m_display_engine->SetRenderState((RwRenderState)20, (void*)1);
        m_display_engine->SetRenderState((RwRenderState)7, (void*)2);
        m_display_engine->SetRenderState((RwRenderState)10, (void*)5);
        m_display_engine->SetRenderState((RwRenderState)11, (void*)6);
        m_display_engine->SetRenderState((RwRenderState)12, (void*)1);
        m_display_engine->SetRenderState((RwRenderState)9, (void*)2);

        if (m_texture != NULL) {
            m_path->Fill(m_brush);
        }

        for (U32 i = 0; i < m_controls.size(); i++) {
            m_controls[i]->Render(i == m_current_control_no);
        }

        m_display_engine->SetRenderState((RwRenderState)6, (void*)1);
        m_display_engine->SetRenderState((RwRenderState)8, (void*)1);
        m_display_engine->SetRenderState((RwRenderState)20, (void*)2);

        scene->EndRender();
    }

    scene->Flip(1);

    return ret;
}
