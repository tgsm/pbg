#include "engine/display/CAtomic.h"
#include "engine/display/CSpline.h"
#include "engine/display/DkDisplay.h"
#include "engine/display/CTexture.h"
#include "engine/input/IInputEngine.h"
#include "engine/sound/DkSound.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include "CIcon.h"
#include "CMiniMap.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <math.h>

CDKW_RGBA BLUE_FILTER_COLOR = CDKW_RGBA(0x32, 0x4B, 0x4B, 0xFF);
CDKW_RGBA BLUE_FILTER_COLOR_COMPLEMENT = CDKW_RGBA(0x64, 0x32, 0x00, 0xFF);

char* CMiniMap::MINIMAP_TEX_FRIEND_NPC[] = {
    "",
    "BMP_211",
    "BMP_212",
    "BMP_213",
    "BMP_214",
    "BMP_215",
    "BMP_216",
    "BMP_217",
    "BMP_218",
    "BMP_219",
    "BMP_220",
    "BMP_221",
    "BMP_222",
};


CMiniMap::CMiniMap(CGame* game) {
    m_scene = NULL;
    m_game = NULL;
    m_spline_manager = NULL;
    m_clump20 = NULL;
    m_camera = NULL;
    m_select_input = NULL;
    m_hero_icon = NULL;
    m_icon40 = NULL;
    m_icon38 = NULL;
    m_icon44 = NULL;
    m_batch48 = NULL;
    m_batch4C = NULL;
    m_unkAC = 0;
    m_visible = FALSE;
    m_unk50.x = 999999.9f;
    m_unk50.y = 999999.9f;
    m_unk50.z = 999999.9f;
    m_unk5C.x = -999999.9f;
    m_unk5C.y = -999999.9f;
    m_unk5C.z = -999999.9f;
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
    if (m_hero_icon != NULL) {
        delete m_hero_icon;
        m_hero_icon = NULL;
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
    DKI::IInputEngine::DestroyInput(m_axev_input);

    m_game->GetDisplayEngine()->GetImmediate()->RemoveBatch2D(m_batch48);
    m_batch48 = NULL;
    m_game->GetDisplayEngine()->GetImmediate()->RemoveBatch2D(m_batch4C);
    m_batch4C = NULL;

    m_scene = NULL;
    m_game = NULL;
    m_spline_manager = NULL;
    m_clump20 = NULL;
    m_camera = NULL;
    m_select_input = NULL;
    m_hero_icon = NULL;
    m_icon40 = NULL;
    m_icon38 = NULL;
    m_icon44 = NULL;
    m_batch48 = NULL;
    m_batch4C = NULL;
    m_unkAC = 0;
    m_visible = FALSE;
    m_unk50.x = 999999.9f;
    m_unk50.y = 999999.9f;
    m_unk50.z = 999999.9f;
    m_unk5C.x = -999999.9f;
    m_unk5C.y = -999999.9f;
    m_unk5C.z = -999999.9f;
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
    m_axev_input = DKI::IInputEngine::CreateInput("MINIMAPAXEV", 0, 15);

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
    m_hero_icon = new CMiniMapIcon(position, 10.0f, 10.0f, 1.0f);
    m_icon44 = new CMiniMapIcon(position, 15.0f, 15.0f, 1.0f);

    position.z = -1.0f;
    m_icon40 = new CMiniMapIcon(position, 200.0f, 100.0f, 1.0f);

    m_icon38->SetRGBA(0xFF, 0xFF, 0xFF, 0xFF);
    m_hero_icon->SetRGBA(0xFF, 0xFF, 0xFF, 0xFF);

    m_batch48 = m_game->GetDisplayEngine()->GetImmediate()->CreateBatch2D(4, 0);
    m_batch4C = m_game->GetDisplayEngine()->GetImmediate()->CreateBatch2D(4, 0);

    m_unk7C = 0.0f;
    m_unk80 = 0.0f;
    m_unkB0 = 0.0f;

    return TRUE;
}

// Equivalent: stack offsets, regalloc
void CMiniMap::Show() {
    U32 mission_nb = m_game->GetCurrentMissionId();
    U32 mission_and_room = mission_nb * 100 + m_game->GetCurrentRoomId();
    if ((mission_nb == MISSION_FINAL || mission_nb == MISSION_MENUS) || (mission_nb == MISSION_ROO && mission_and_room == 209) || (mission_nb == MISSION_EEYORE && mission_and_room == 411) || (mission_nb == MISSION_TIGGER && mission_and_room == 613) || m_game->GetIngameGamePart()->GetGameRoomManager()->IsOnFight() || (m_game->GetCurrentHeroId() == HERO_TIGGER || m_game->GetCurrentHeroId() == HERO_WINNIE)) {
        std::string name = "SND_012_2D";
        m_unkAC = DkSoundGetEngine()->PlaySound2D(&name, 1);
        if (m_unkAC != NULL) {
            m_unkAC->SetLayer(1);
        }
        return;
    }

    ComputeMinimapBBox();
    m_visible = TRUE;

    m_game->GetIngameGamePart()->SetupSoundsLayer(3, 0);

    std::string name = "SND_010_2C";
    DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
    if (sound != NULL) {
        sound->SetLayer(3);
    }

    CDKW_V3d local_3c(0.0f, 0.0f, 0.0f);
    F32 dVar21 = local_3c.x;
    F32 dVar20 = local_3c.z;

    CDKW_V3d local_90, local_84, local_78;
    local_78.x = CDKW_V3d::YAXIS.x;
    local_78.y = CDKW_V3d::YAXIS.y;
    local_78.z = CDKW_V3d::YAXIS.z;
    local_84.x = 0.0f;
    local_84.y = 0.0f;
    local_84.z = 0.0f;
    local_90.x = 0.0f;
    local_90.y = 0.0f;
    local_90.z = 90.0f;
    m_camera->LookAtInline(local_90, local_84, local_78);

    if (m_clump20 != NULL) {
        int id = m_clump20->GetAtomicIndexFromID(mission_and_room);
        if (id >= 0) {
            DKDSP::CAtomic* atomic = m_clump20->GetAtomic(id);
            if (atomic != NULL) {
                CDKW_V3d pos = atomic->GetPosition();
                dVar21 = pos.x;
                dVar20 = pos.z;
            }
        }
    }

    CDKW_V3d local_48;
    CDKW_V3d local_60;

    // Likely something inlined here?

    DKDSP::CCamera* camera = m_camera;
    local_48 = (CDKW_V3d&)camera->GetFrame()->GetLTM().right;
    camera->GetFrame()->TranslateCopy(local_48 * -dVar21, 2);

    camera = m_camera;
    local_60 = (CDKW_V3d&)camera->GetFrame()->GetLTM().up;
    dVar20 = -dVar20;
    camera->GetFrame()->TranslateCopy(local_60 * -dVar20, 2);

    if (m_game->GetIngameGamePart() != NULL) {
        m_game->GetIngameGamePart()->GetGameRoomManager()->m_flags |= (1 << 3);
    }
    m_unk90 = 0.0f;
    m_unk98 = TRUE;
    m_unkB0 = 0.0f;
}

// Likely inlined in RenderIcons and was deadstripped, name unknown.
// Incomplete
U32 CMiniMap::UnkRoomNPCMaybeThing(int a1) {
    if (a1 <= 0 || a1 > 15) {
        return 0;
    }

    volatile int unk = ((a1 - 1) % 4);
    return unk;
    // return 0xF << () m_game->GetCurrentMission()->m_unk4C4.unk[unk] & ;
}

void CMiniMap::ShowFriendNPC(int a1, int a2, int a3) {
    if (a1 <= 0 || a1 > 15) {
        return;
    }

    m_game->GetCurrentMission()->m_unk4C4.unk[(a1 - 1) / 4] |= (a2 & 0xF) << (4 + ((a1 - 1) % 4) * 8);
    m_game->GetCurrentMission()->m_unk4C4.unk[(a1 - 1) / 4] |= (a3 & 0xF) << (0 + ((a1 - 1) % 4) * 8);
}

void CMiniMap::HideFriendNPC(int a1) {
    if (a1 <= 0 || a1 > 15) {
        return;
    }

    m_game->GetCurrentMission()->m_unk4C4.unk[(a1 - 1) / 4] &= 0xFF << (((a1 - 1) % 4) * 8) ^ 0xFFFFFFFF;
}

// Equivalent: regalloc
void CMiniMap::Render(F32 dt) {
    CMission* mission;
    U32 i;

    if (!m_visible) {
        return;
    }

    m_scene->SelectCamera(m_camera);
    m_scene->BeginRender();

    BOOL vertex_alpha_enable;
    BOOL zwrite_enable;
    BOOL ztest_enable;
    RwBlendFunction src_blend;
    RwBlendFunction dest_blend;
    RwCullMode cull_mode;
    RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &vertex_alpha_enable);
    RwRenderStateGet(rwRENDERSTATEZWRITEENABLE, &zwrite_enable);
    RwRenderStateGet(rwRENDERSTATEZTESTENABLE, &ztest_enable);
    RwRenderStateGet(rwRENDERSTATECULLMODE, &cull_mode);
    RwRenderStateGet(rwRENDERSTATESRCBLEND, &src_blend);
    RwRenderStateGet(rwRENDERSTATEDESTBLEND, &dest_blend);

    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    m_game->GetDisplayEngine()->AlphaEnable();

    RenderDarkenRooms(dt);

    if (m_unk94 >= 1.0f) {
        RenderHeroIcon(dt);

        if (!(m_unk94 < 1.0f)) {
            U32 mission_id = m_game->GetCurrentMissionId();
            mission = m_game->GetCurrentMission();
            if (mission != NULL) {
                i = 1;
                U32 mission_and_room = mission_id * 100 + 1;
                for (; i < mission->m_num_rooms + 1; i++) {
                    if (mission->IsRoomOpened(i)) {
                        if (i == m_game->GetCurrentRoomId()) {
                            m_scene->SetAmbient(1.0f, 1.0f, 1.0f);
                        } else {
                            m_scene->SetAmbient(0.6f, 0.6f, 0.6f);
                        }

                        if (m_clump20 != NULL) {
                            int id = m_clump20->GetAtomicIndexFromID(mission_and_room);
                            if (id >= 0) {
                                DKDSP::CAtomic* atomic = m_clump20->GetAtomic(id);
                                if (atomic != NULL) {
                                    m_scene->RenderAtomic(atomic);
                                    m_scene->Flush();
                                }
                            }
                        }
                    }

                    mission_and_room++;
                }
            }

            m_scene->SetAmbient(1.0f, 1.0f, 1.0f);

            if (m_clump20 != NULL) {
                int id = m_clump20->GetAtomicIndexFromID(100);
                DKDSP::CAtomic* atomic = m_clump20->GetAtomic(id);
                if (atomic != NULL) {
                    m_scene->RenderAtomic(atomic);
                    m_scene->Flush();
                }
            }
        }

        RenderIcons(dt);
        RenderCookiesNbIcon(dt);
    }

    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEVERTEXALPHAENABLE, (void*)vertex_alpha_enable);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEZWRITEENABLE, (void*)zwrite_enable);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEZTESTENABLE, (void*)ztest_enable);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATECULLMODE, (void*)cull_mode);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATESRCBLEND, (void*)src_blend);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEDESTBLEND, (void*)dest_blend);

    m_scene->Flush();
    m_scene->EndRender();
}

// Equivalent: stack offsets
void CMiniMap::RenderDarkenRooms(F32 dt) {
    CDKW_V3d position40;
    CDKW_V3d position44;
    CDKW_V3d unk_vec(0.0f, 0.0f, 0.0f);

    if (m_icon40 == NULL || m_icon44 == NULL) {
        return;
    }

    position40 = m_camera->GetPosition();
    position40.z = -1.0f;
    m_icon40->SetPosition(position40);
    m_icon40->SetTexture(NULL);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
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
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

    if (m_unk94 >= 1.0f) {
        DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_209");
        if (texture != NULL) {
            m_icon44->SetTexture(texture);
        } else {
            m_icon44->SetTexture(NULL);
        }
        m_icon44->SetRGBA(0x90, 0xBC, 0xD4, 0xFF);

        CMission* mission = m_game->GetCurrentMission();
        if (mission != NULL) {
            int iVar1 = m_game->GetCurrentMissionId() * 100;
            for (U32 i = 1; i < mission->m_num_rooms + 1; i++) {
                int id = iVar1 + i;
                if (!mission->IsRoomOpened(i) && m_clump20 != NULL) {
                    int atomic_index = m_clump20->GetAtomicIndexFromID(id);
                    if (atomic_index >= 0) {
                        DKDSP::CAtomic* atomic = m_clump20->GetAtomic(atomic_index);
                        if (atomic != NULL) {
                            position44 = atomic->GetPosition();
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
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
}

void CMiniMap::RenderHeroIcon(F32 dt) {
    if (m_unk94 < 1.0f) {
        return;
    }

    F32 dVar5 = 1.5f * (3.1415927f * m_unkB0);

    if (m_spline_manager == NULL || m_game == NULL) {
        return;
    }

    if (m_hero_icon == NULL) {
        return;
    }

    CMiniMapSpline* hero_spline = m_spline_manager->GetSpline("HEROSPLINE");
    if (hero_spline == NULL) {
        return;
    }
    DKDSP::CSpline* spline = hero_spline->m_spline;
    if (spline == NULL) {
        return;
    }

    spline->GetNumberOfControlPoints(); // unused
    CDKW_V3d vec;
    spline->GetControlPoint(m_game->GetCurrentRoomId() - 1, &vec);
    m_hero_icon->SetSize(10.0f + cosf(1.5f * dVar5) - 1.0f);
    m_hero_icon->SetPosition(vec);

    switch (m_game->GetCurrentHeroId()) {
        case HERO_PIGLET:
        case HERO_CATCH_THEM_ALL: {
            DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_205");
            if (texture != NULL) {
                m_hero_icon->SetTexture(texture);
            }
            break;
        }
        case HERO_TIGGER: {
            DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_207");
            if (texture != NULL) {
                m_hero_icon->SetTexture(texture);
            }
            break;
        }
        case HERO_WINNIE: {
            DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_206");
            if (texture != NULL) {
                m_hero_icon->SetTexture(texture);
            }
            break;
        }
    }

    m_hero_icon->Render(m_scene);
}

// Incomplete
void CMiniMap::RenderIcons(F32 dt) {
    if (m_unk94 < 1.0f || m_icon38 == NULL || m_spline_manager == NULL || m_game == NULL) {
        return;
    }

    DKDSP::CSpline* cookie_spline = m_spline_manager->GetSpline("COOKIESPLINE")->GetSpline();
    if (cookie_spline != NULL) {
        for (U32 i = 1; i < m_game->GetCurrentMission()->m_num_rooms + 1; i++) {
            if (m_game->GetCurrentMission()->IsRoomOpened(i)) {
                if (!(m_game->GetCurrentMission()->m_rooms.rooms[i] & (1 << 2))) {
                    CDKW_V3d control_point;
                    cookie_spline->GetControlPoint(i - 1, &control_point);
                    m_icon38->SetPosition(control_point);
                    DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_201");
                    if (texture != NULL) {
                        m_icon38->SetTexture(texture);
                    }
                    m_icon38->Render(m_scene);
                } else {
                    CDKW_V3d control_point;
                    cookie_spline->GetControlPoint(i - 1, &control_point);
                    m_icon38->SetPosition(control_point);
                    DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_202");
                    if (texture != NULL) {
                        m_icon38->SetTexture(texture);
                    }
                    m_icon38->Render(m_scene);
                }
            }
        }

        if (m_game->GetCurrentHeroId() == HERO_CATCH_THEM_ALL) {
            DKDSP::CSpline* npc_spline = m_spline_manager->GetSpline("NPCSPLINE")->GetSpline();
            if (npc_spline != NULL) {
                for (U32 i = 1; i < m_game->GetCurrentMission()->m_num_rooms + 1; i++) {
                    CDKW_V3d control_point;
                    npc_spline->GetControlPoint(i - 1, &control_point);
                    m_icon38->SetPosition(control_point);

                    if (m_game->GetCurrentMission()->IsRoomCompleted(i)) {
                        DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_204");
                        if (texture != NULL) {
                            m_icon38->SetTexture(texture);
                        }
                    } else {
                        DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture("BMP_203");
                        if (texture != NULL) {
                            m_icon38->SetTexture(texture);
                        }
                    }
                    m_icon38->Render(m_scene);
                }
            }
        }

        CMiniMapSpline* friend_npc_spline = m_spline_manager->GetSpline("FRIENDNPCSPLINE");
        if (friend_npc_spline != NULL) {
            DKDSP::CSpline* spline = friend_npc_spline->GetSpline();
            if (spline != NULL && m_icon38 != NULL) {
                for (int i = 0; i < spline->GetNumberOfControlPoints(); i++) {
                    int uVar9 = UnkRoomNPCMaybeThing(i + 1);
                    U32 uVar5 = UnkRoomNPCMaybeThing(i - 1);

                    if (uVar9 > 0 && uVar9 < 13) {
                        if (m_game->GetCurrentMission()->IsRoomOpened(uVar5)) {
                            CDKW_V3d control_point;
                            spline->GetControlPoint(i, &control_point);
                            m_icon38->SetPosition(control_point);

                            DKDSP::CTexture* texture = m_game->GetTextureDictionary()->FindTexture(MINIMAP_TEX_FRIEND_NPC[uVar9]);
                            if (texture != NULL) {
                                m_icon38->SetTexture(texture);
                            }
                            m_icon38->Render(m_scene);
                        }
                    }
                }
            }
        }
    }
}

void CMiniMap::RenderCookiesNbIcon(F32 dt) {
    CIcon icon1, icon2;

    CIcon::BeginRender(m_game->GetDisplayEngine(), m_camera, m_scene);

    icon2.m_width = 0.35f;
    icon2.m_height = 0.18f;
    icon2.m_x = 0.195f;
    icon2.m_y = 0.13f;
    icon2.m_texture = m_game->GetTextureDictionary()->FindTexture("BMP_208");
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
    icon1.m_texture = m_game->GetTextureDictionary()->FindTexture("BMP_201");
    icon1.Render(m_batch48, 0);

    CIcon::EndRender();

    m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_camera->m_wrap_camera->m_rw_camera);
}

BOOL CMiniMap::IsVisible() {
    return m_visible;
}

void CMiniMap::ComputeMinimapBBox() {
    U32 mission_nb = m_game->GetCurrentMissionId();
    U32 mission_and_room = mission_nb * 100 + m_game->GetCurrentRoomId();
    if ((mission_nb == MISSION_FINAL || mission_nb == MISSION_MENUS) || (mission_nb == MISSION_ROO && mission_and_room == 209) || (mission_nb == MISSION_EEYORE && mission_and_room == 411) || (mission_nb == MISSION_TIGGER && mission_and_room == 613)) {
        return;
    }

    m_unk50.x = 999999.9f;
    m_unk50.y = 999999.9f;
    m_unk50.z = 999999.9f;
    m_unk5C.x = -999999.9f;
    m_unk5C.y = -999999.9f;
    m_unk5C.z = -999999.9f;

    CMission* mission = m_game->GetCurrentMission();
    if (mission != NULL) {
        U32 i = 1;
        mission_and_room = m_game->GetCurrentMissionId() * 100 + 1;
        for (; i < mission->m_num_rooms + 1u; i++) {
            if (m_clump20 != NULL) {
                int id = m_clump20->GetAtomicIndexFromID(mission_and_room);
                DKDSP::CAtomic* atomic = NULL;
                if (id >= 0) {
                    atomic = m_clump20->GetAtomic(id);
                }
                if (atomic == NULL) {
                    return;
                }
                CDKW_V3d inf, sup;
                atomic->GetBBox(&inf, &sup);
                inf *= atomic->GetFrame()->GetLTM();
                sup *= atomic->GetFrame()->GetLTM();

                if (inf.x < m_unk50.x) {
                    m_unk50.x = inf.x;
                }
                if (inf.y < m_unk50.y) {
                    m_unk50.y = inf.y;
                }
                if (inf.z < m_unk50.z) {
                    m_unk50.z = inf.z;
                }

                if (sup.x > m_unk5C.x) {
                    m_unk5C.x = sup.x;
                }
                if (sup.y > m_unk5C.y) {
                    m_unk5C.y = sup.y;
                }
                if (sup.z > m_unk5C.z) {
                    m_unk5C.z = sup.z;
                }
            }

            mission_and_room++;
        }
    }

    m_unk50.z = 0.0f;
    m_unk5C.z = 0.0f;
}

volatile inline U32 bitflip_hack(int value) {
    return value ^ 0xFFFFFFFF;
}

// Equivalent: regalloc, scheduling
BOOL CMiniMap::Load(DkXmd::CChunkIterator* iter) {
    DkXmd::CChunkIterator dest;
    char buf[256];
    strcpy(buf, iter->GetName());

    if (strcmp(buf, "Minimap") == 0) {
        if (iter->GetFirstChildChunk(dest) == TRUE) {
            do {
                strcpy(buf, dest.GetName());

                if (strcmp(buf, "Name") == 0) {
                    m_name = dest.GetStringValue();
                }

                if (strcmp(buf, "Mesh") == 0) {
                    if (m_game == NULL) {
                        return FALSE;
                    }

                    m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_CLUMP, dest.GetStringValue());
                    DKDSP::CClump* clump = m_game->GetObjectDictionary()->FindClump(dest.GetStringValue());
                    m_clump20 = m_scene->CloneClump(clump);
                }

                if (strcmp(buf, "TextDic") == 0) {
                    m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_TEXTURE_DICTIONARY, dest.GetStringValue());
                }

                if (strcmp(buf, "Splines") == 0) {
                    if (m_spline_manager == NULL) {
                        return FALSE;
                    }

                    if (!m_spline_manager->Parse(dest)) {
                        return FALSE;
                    }
                }
            } while (dest.GetNextSiblingChunk(dest) == TRUE);
        }
    } else {
        return FALSE;
    }

    if (m_clump20 == NULL) {
        return FALSE;
    }

    m_unk68 = 0.0f;
    m_unk6C = 0.0f;

    CMiniMapSpline* map_spline = m_spline_manager->GetSpline("FRIENDNPCSPLINE");
    if (map_spline != NULL) {
        U32 nb_control_points = 0;
        if (map_spline->GetSpline() != NULL) {
            nb_control_points = map_spline->GetSpline()->GetNumberOfControlPoints();
        }
        for (int i = 1; i < nb_control_points + 1; i++) {
            if (i > 0 && i <= 15) {
                // FIXME: This is likely HideFriendNPC?
                int mod = (i - 1) % 4;
                int div = (i - 1) / 4;
                int mask = 0xFF;
                mask <<= mod * 8;
                // mask ^= 0xFFFFFFFF;

                m_game->GetCurrentMission()->m_unk4C4.unk[div] &= bitflip_hack(mask);
            }
        }
    }

    return TRUE;
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
