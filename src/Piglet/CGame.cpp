#include <cstddef>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include <rwa/rwastream.h>
#include "engine/backup/DkBackUp.h"
#include "engine/display/DkDisplay.h"
#include "engine/display/IImBatch.h"
#include "engine/filesys/DkFileSys.h"
#include "engine/gui/DkGUI.h"
#include "engine/sound/DkSound.h"
#include "engine/video/DkVideo.h"
#include "CGame.h"
#include "CGamePartCredits.h"
#include "CGamePartDMRoomLauncher.h"
#include "CGamePartFrontend.h"
#include "CGamePartIngame.h"
#include "CGamePartIntroduction.h"
#include "CGamePartMovieClips.h"
#include "CGamePartScrapBook.h"
#include "CGamePartStartScreen.h"
#include "CGCNFont.h"
#include "CResourceFactory.h"
#include "entities/CEntity.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityManager.h"
#include <cstring>
#include <iostream>

CGuiManager* CGame::gs_CurrentGuiManager;
DKVIDEO::CVideoEngineGCN* CGame::gs_CurrentVideoManager;

static BOOL s_bResetButtonPushed;

extern "C" void Rt2dOpen(RwCamera*);
extern "C" void Rt2dDeviceSetFlat(F32);
extern "C" void Rt2dDeviceSetCamera(RwCamera*);
extern "C" void Rt2dFontSetPath(char*);
extern "C" void Rt2dAnimOpen();
extern "C" void RwResourcesSetArenaSize(U32);

// *Very* incomplete
CGame::CGame(void* a1, U32 a2) {
    m_unk8 = a2;
    m_game_part = NULL;
    m_display_engine = NULL;
    m_texture_dictionary = NULL;
    m_object_dictionary = NULL;
    m_anim_dictionary = NULL;
    m_entity_manager = NULL;
    m_resource_factory = NULL;
    m_timer = NULL;
    m_scene = NULL;
    m_mailbox = NULL;
    m_sound_engine = NULL;
    m_sample_dictionary = NULL;
    m_gui_engine = NULL;
    m_gui_manager = NULL;
    m_fx_manager = NULL;
    m_game_backup = NULL;
    m_backup_engine = NULL;
    m_video_engine = NULL;
    m_batch5050 = NULL;
    m_unk5038 = 2;
    m_unk5054 = 0;
    m_unk505C = NULL;
    m_unk5060 = NULL;
    m_unk5064.reserve(40);
    m_video_descs.clear();
    m_video_descs.reserve(16);
    m_unk5004 = "";
    m_unk5008 = NULL;
    m_delta_time = (1.0f/60.0f);
    m_error_callback = NULL;
    m_screen_effect = NULL;
    m_unk508C = NULL;
    m_display_engine = DkDisplayGetEngine();
    m_display_engine->SetCharsetCreation(FALSE);
    m_sound_engine = DkSoundGetEngine();
    m_gui_engine = DkGUIGetEngine();
    m_backup_engine = DkBakUpGetEngine();
    m_video_engine = DkVideoGetEngine();

    if (m_display_engine != NULL) {
        void* data = m_display_engine->Open();

        m_display_engine->SetGCNVideoMode(&GXNtsc480IntDf);
        m_display_engine->Setup(0, 0);
        m_display_engine->Start();

        RwResourcesSetArenaSize(0x90000);

        DKI::IInputEngine::Open(a1, data);
        DKI::IInputEngine::GetDevice(0)->VibrationEnable();

        m_sound_engine->Initialize(data);
        m_sound_engine->SetGlobalVolume(0.75f);
        m_unk504C = 0.75f;
        m_sound_engine->SetStereoMode(OS_SOUND_MODE_STEREO);
        if (OSGetSoundMode() == OS_SOUND_MODE_STEREO) {
            m_sound_engine->SetStereoMode(OS_SOUND_MODE_STEREO);
        } else {
            m_sound_engine->SetStereoMode(OS_SOUND_MODE_MONO);
        }

        m_video_engine->Initialize(data);

        m_display_engine->SetImagePath("Data/NoTextures/");

        m_sample_dictionary = m_sound_engine->GetSampleDictionary();
        m_texture_dictionary = m_display_engine->GetTextureDictionary();
        m_object_dictionary = m_display_engine->GetObjectDictionary();
        m_anim_dictionary = m_display_engine->GetAnimDictionary();
        m_scene = m_display_engine->CreateScene();
        m_camera = m_scene->CreateCamera();

        Rt2dOpen(NULL);
        Rt2dDeviceSetFlat(1.0f);
        Rt2dDeviceSetCamera(m_camera->m_wrap_camera->m_rw_camera);
        Rt2dFontSetPath("data/common/menus/font/");
        Rt2dAnimOpen();

        m_camera->SetZFar(300.0f);
        m_camera->SetZNear(0.6f);
        m_scene->SelectCamera(m_camera);

        CGCNFont::Create();

        m_error_callback = new CErrorCallback;
        m_error_callback->m_game = this;
        CDkFileSys::SetErrorCallBack(m_error_callback);

        m_timer = m_display_engine->CreateTimer();
        m_timer->Reset();

        m_mailbox = new CMailBox(this, 128);

        m_loading_adventure = new CLoadingAdventure(this);
        m_loading_catch_them_all = new CLoadingCatchThemAll(this);
        m_loading_tigger = new CLoadingTigger(this);
        m_loading_winnie = new CLoadingWinnie(this);

        m_ingame_loading_callback = new CInGameLoadingCallback(this);
        m_bootup_loading_callback = new CBootUpLoadingCallback(this);
        m_prebootup_loading_callback = new CPreBootUpLoadingCallback(this);
        m_video_loading_callback = new CVideoLoadingCallback(this);
        m_current_loading_callback = m_prebootup_loading_callback;
        m_current_loading_callback->Create();
        CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

        m_gui_manager = new CGuiManager(this);
        m_fx_manager = new CFxManager(this);
        m_game_backup = new CGameBackup(this);
        m_game_backup->CreateNewGame();
        m_minimap = new CMiniMap(this);
        m_shadow_zone = new CShadowZone(this);

        RegisterVideo(0, "MOVIES/DISNEY");
        RegisterVideo(1, "MOVIES/DOKI");
        RegisterVideo(2, "MOVIES/CLIPS/CLIP01");
        RegisterVideo(3, "MOVIES/CLIPS/CLIP02");
        RegisterVideo(4, "MOVIES/CLIPS/CLIP03");
        RegisterVideo(5, "MOVIES/CLIPS/CLIP04");
        RegisterVideo(6, "MOVIES/CLIPS/CLIP05");
        RegisterVideo(7, "MOVIES/CLIPS/CLIP06");
        RegisterVideo(8, "MOVIES/CLIPS/CLIP07");
        RegisterVideo(9, "MOVIES/CLIPS/CLIP08");
        RegisterVideo(10, "MOVIES/CLIPS/CLIP09");
        RegisterVideo(11, "MOVIES/CLIPS/CLIP10");
        RegisterVideo(12, "MOVIES/CLIPS/CLIP11");
        RegisterVideo(13, "MOVIES/CLIPS/CLIP12");
        RegisterVideo(14, "MOVIES/FMV/FMV01");
        RegisterVideo(15, "MOVIES/FMV/FMV02");
        RegisterVideo(16, "MOVIES/FMV/FMV03");
        RegisterVideo(17, "MOVIES/FMV/FMV04");
        RegisterVideo(18, "MOVIES/FMV/FMV05");
        RegisterVideo(19, "MOVIES/FMV/FMV06");
        RegisterVideo(20, "MOVIES/FMV/FMV07");
        RegisterVideo(21, "MOVIES/FMV/FMV08");
        RegisterVideo(22, "MOVIES/FMV/FMV09");
        RegisterVideo(23, "MOVIES/FMV/FMV10");
        RegisterVideo(24, "MOVIES/FMV/FMV11");
        RegisterVideo(25, "MOVIES/FMV/FMV12");
        RegisterVideo(26, "MOVIES/FMV/FMV13");
        RegisterVideo(27, "MOVIES/FMV/FMV14");
        RegisterVideo(28, "MOVIES/FMV/FMV15");
        RegisterVideo(29, "MOVIES/FMV/FMV16");
        RegisterVideo(30, "MOVIES/FMV/FMV17");
        RegisterVideo(31, "MOVIES/ROLLING");
        RegisterVideo(32, "MOVIES/GOTHAM");

        gs_CurrentGuiManager = m_gui_manager;
        gs_CurrentVideoManager = m_video_engine;
        m_video_engine->SetCallBack(ReplayVideoCallback);
    }

    CDkFileSys::init("./PIGGCN.XMD", 1 << 0, 0x400);
    CDkFileSys::UnSetCallBackOnLoad();
    CDkFileSys::SetErrorCallBack(m_error_callback);

    std::string rws_filename = "GCNStereo.rws";

    // ...

    for (int i = 0; i < 5; i++) {
        m_sound_engine->BeginUpdate();
        m_sound_engine->EndUpdate();

        VIWaitForRetrace();
    }

    RwaStream* stream = RwaStreamCreate(NULL, 0, 0, 0, 0);
    static char tmpString[255];
    strcpy(tmpString, "GCNStereo.rws");

    RwaUnkMedia media;
    media.filename = tmpString;
    media.unk4 = 1;
    media.unkC = 0;
    media.unk10 = 0;
    media.unk8 = 0;
    media.unk14 = 0;
    media.unk18 = 8;
    media.unk1C = 4;
    media.unk20 = 1;
    _rwaStreamSetMedia(stream, &media);

    do {
        m_sound_engine->BeginUpdate();
        m_sound_engine->EndUpdate();
        VIWaitForRetrace();
    } while (_rwaStreamGetStatus(stream) == 2);

    // ...

    m_backup_engine->Initialize();
    m_backup_engine->SetGameTitle("Piglet's BIG GAME");
    m_backup_engine->SetIconFile("PIGLET.ICO");
    m_backup_engine->SetEventCB(&m_memory_card_save_event_callback);

    m_display_engine->RegisterEvent(0, "FX", &m_fx_event_callback);
    m_display_engine->RegisterEvent(1, "SND", &m_snd_event_callback);
    m_display_engine->RegisterEvent(2, "VIB", &m_vib_event_callback);
    m_display_engine->RegisterEvent(3, "VIB2D", &m_vib2d_event_callback);
    m_display_engine->RegisterEvent(4, "SHAKE", &m_shake_event_callback);

    m_entity_manager = new CEntityManager(this);
    m_resource_factory = new CResourceFactory(this);
    m_texture_dictionary->RegisterTextureCallback(m_resource_factory);
    m_entity_manager->SetModelFile("Models/Models.XMD");

    U32 rf_unkC = m_resource_factory->m_unkC;
    m_resource_factory->m_unkC = 2;

    m_display_engine->SetImagePath("Data/NoTextures/");
    LoadConfigFile("Piglet.XMD");
    Rt2dFontSetPath("./data/gcn/menus/font/");

    m_gui_engine->InitTexts();
    m_display_engine->SetImagePath("Data/NoTextures/");
    Rt2dFontSetPath("./data/gcn/menus/font/");

    m_current_loading_callback = m_bootup_loading_callback;
    m_delta_time = 0.0f;
    m_current_loading_callback->Create();
    CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

    // ...

    m_entity_manager->CreateEntity("Piglet", "PigletEntity", "100_PIGLET");
    m_entity_manager->DestroyEntity("Piglet");

    m_entity_manager->CreateEntity("FIOLE", ENTITY_PHIAL, "CAR_650");
    m_entity_manager->DestroyEntity("FIOLE");

    m_entity_manager->CreateEntity("COOKIE", ENTITY_COOKIE, "CAR_600");
    m_entity_manager->DestroyEntity("COOKIE");

    // ...

    std::string car700 = "Models/700_FIGHT_PATH/CAR_700.txd";
    std::string car701 = "Models/700_FIGHT_PATH/CAR_701.txd";
    m_resource_factory->LoadResource(6, car700);
    m_resource_factory->LoadResource(6, car701);
    car700 = "Models/700_FIGHT_PATH/CAR_700.dff";
    car701 = "Models/700_FIGHT_PATH/CAR_701.dff";
    m_resource_factory->LoadResource(1, car700);
    m_resource_factory->LoadResource(1, car701);

    m_resource_factory->LoadResource(9, "FX/FX_C340_03.xmd");
    m_resource_factory->LoadResource(6, "WARPS/WRP_COMBAT/WARPCOMBAT.TXD");
    m_resource_factory->LoadResource(1, "WARPS/WRP_COMBAT/WARPCOMBAT.DFF");

    m_resource_factory->m_unkC = rf_unkC;
    m_unk4F54 = 0;
    m_unk5090 = 1;

    for (int i = 0; i < 8; i++) {
        m_unk210[i].Initialize();
        m_unk210[i].m_game = this;
        m_unk210[i].m_mission_no = i + 1;
        m_unk210[i].LoadConfigFile(1);

        m_unk28B0[i].m_game = m_unk210[i].m_game;
        m_unk28B0[i].m_mission_name.assign(m_unk210[i].m_mission_name, 0);
        m_unk28B0[i] = m_unk210[i];
    }
}

extern "C" void Rt2dAnimClose();
extern "C" void Rt2dClose();

// Equivalent?: std::vector
CGame::~CGame() {
    if (m_unk508C != NULL) {
        DKI::IInputEngine::DestroyInput(m_unk508C);
        m_unk508C = NULL;
    }

    if (m_batch5050 != NULL) {
        m_display_engine->GetImmediate()->RemoveBatch2D(m_batch5050);
        m_batch5050 = NULL;
    }

    if (m_game_part != NULL) {
        delete m_game_part;
        m_game_part = NULL;
    }

    m_resource_factory->UnloadResources(2);

    if (m_mailbox != NULL) {
        delete m_mailbox;
    }
    m_mailbox = NULL;

    if (m_timer != NULL) {
        m_timer->Release();
    }
    m_timer = NULL;

    if (m_entity_manager != NULL) {
        delete m_entity_manager;
    }
    m_entity_manager = NULL;

    if (m_resource_factory != NULL) {
        delete m_resource_factory;
    }
    m_resource_factory = NULL;

    if (m_loading_adventure != NULL) {
        delete m_loading_adventure;
        m_loading_adventure = NULL;
    }
    if (m_loading_catch_them_all != NULL) {
        delete m_loading_catch_them_all;
        m_loading_catch_them_all = NULL;
    }
    if (m_loading_tigger != NULL) {
        delete m_loading_tigger;
        m_loading_tigger = NULL;
    }
    if (m_loading_winnie != NULL) {
        delete m_loading_winnie;
        m_loading_winnie = NULL;
    }

    if (m_ingame_loading_callback != NULL) {
        delete m_ingame_loading_callback;
        m_ingame_loading_callback = NULL;
    }
    if (m_bootup_loading_callback != NULL) {
        delete m_bootup_loading_callback;
        m_bootup_loading_callback = NULL;
    }
    if (m_prebootup_loading_callback != NULL) {
        delete m_prebootup_loading_callback;
        m_prebootup_loading_callback = NULL;
    }

    if (m_gui_manager != NULL) {
        delete m_gui_manager;
    }
    m_gui_manager = NULL;

    if (m_fx_manager != NULL) {
        delete m_fx_manager;
    }
    m_fx_manager = NULL;

    if (m_minimap != NULL) {
        delete m_minimap;
    }
    m_minimap = NULL;

    if (m_shadow_zone != NULL) {
        delete m_shadow_zone;
    }
    m_shadow_zone = NULL;

    if (m_game_backup != NULL) {
        delete m_game_backup;
    }
    m_game_backup = NULL;

    if (m_screen_effect != NULL) {
        delete m_screen_effect;
        m_screen_effect = NULL;
    }

    if (m_error_callback != NULL) {
        delete m_error_callback;
        m_error_callback = NULL;
    }

    CDkFileSys::UnSetErrorCallBack();

    if (m_texture_dictionary != NULL) {
        m_texture_dictionary->Release();
    }
    m_texture_dictionary = NULL;

    if (m_anim_dictionary != NULL) {
        m_anim_dictionary->Release();
    }
    m_anim_dictionary = NULL;

    if (m_object_dictionary != NULL) {
        m_object_dictionary->Release();
    }
    m_object_dictionary = NULL;

    m_camera = NULL;

    if (m_scene != NULL) {
        m_scene->Release();
    }
    m_scene = NULL;

    if (m_sound_engine != NULL) {
        m_sound_engine->Release();
    }
    m_sound_engine = NULL;

    DkBakUpRelease();
    m_gui_engine->ShutTexts();
    DkGUIRelease();
    Rt2dAnimClose();
    Rt2dClose();
    DkSoundRelease();
    DkVideoRelease();

    if (m_display_engine != NULL) {
        m_display_engine->Release();
    }
    m_display_engine = NULL;
    DkDisplayRelease();

    DKI::IInputEngine::Clear();
    DKI::IInputEngine::Close();

    CDkFileSys::exit();
}

extern "C" void RwFreeListPurgeAllFreeLists(void);

// Incomplete
BOOL CGame::NextFrame() {
    int iVar12;

    ComputeDeltaTime();
    BOOL bVar8 = FALSE;
    int iVar24 = 0;
    BOOL bVar9 = FALSE;
    DKI::IInputEngine::Update(m_delta_time);

    ManageReset();

    if (m_game_part != NULL) {
        if (m_unk5008 != NULL && m_sound_engine->GetStreamedSound() == m_unk5008 && m_unk5008->IsFinished()) {
            m_unk5004 = "";
            m_unk5008 = NULL;
            m_sound_engine->StopStreamedSound();
        }
        iVar12 = m_game_part->NextFrame();
    } else {
        iVar12 = 0;
    }

    if (m_game_part == NULL || iVar12 != m_game_part->m_unk0 || m_game_part == NULL) {
        if (m_current_loading_callback != NULL) {
            m_current_loading_callback->Destroy();
            CDkFileSys::UnSetCallBackOnLoad();
        }

        m_unk5004 = "";
        m_unk5008 = NULL;
        m_sound_engine->StopStreamedSound();

        m_gui_manager->Reset();

        DKI::IInputEngine::GetDevice(0)->StopVibration();

        if (iVar12 != 10) {
            int command;
            // Probably wrong
            for (int i = 0; i < m_opcode_buffer_size, command = m_opcode_buffer[i], command != -1;) {
                i++;
                switch (command) {
                    case 1: {
                        U32 unk4F58 = m_opcode_buffer[i++];
                        int unk4F5C = m_opcode_buffer[i++];
                        int unk4F60 = m_opcode_buffer[i++];
                        int unk4F64 = m_opcode_buffer[i++];
                        int unk4F68 = m_opcode_buffer[i++];
                        int unk4F6C = m_opcode_buffer[i++];
                        int unk4F70 = m_opcode_buffer[i++];
                        int unk4F74 = m_opcode_buffer[i++];
                        int unk5090 = m_opcode_buffer[i++];

                        m_unk4F58 = unk4F58;
                        m_unk4F5C = unk4F5C;
                        m_unk4F60 = unk4F60;
                        m_unk4F64 = unk4F64;
                        m_unk4F68 = unk4F68;
                        m_unk4F6C = unk4F6C;
                        m_unk4F70 = unk4F70;
                        m_unk4F74 = unk4F74;
                        m_unk5090 = unk5090;

                        if (unk4F58 == 1 && m_unk4F54 == 8) {
                            m_game_backup->CreateNewGame();
                            for (U32 j = 0; j < 8; j++) {
                                m_unk210[j].Initialize();
                                m_unk210[j].m_game = this;
                                m_unk210[j].m_mission_no = j + 1;
                                m_unk210[j].LoadConfigFile(1);
                            }
                            bVar8 = TRUE;
                        }

                        GetMission(m_unk4F54 - 1).m_rooms[unk4F58] |= (1 << 0);

                        m_game_backup->GetFromGameData(1);
                        m_game_backup->Backup();
                        iVar12 = 7;
                        break;
                    }

                    case 2: {
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }

                        U32 fortnite = m_opcode_buffer[i++];
                        if (GetMission(fortnite - 1).m_unk2C != 0) {
                            m_current_loading_callback = (CBaseLoadingCallback*)m_loading_catch_them_all;
                        } else {
                            m_current_loading_callback = (CBaseLoadingCallback*)m_loading_adventure;
                        }
                        m_current_loading_callback->Create();
                        CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

                        if (fortnite != m_unk4F54) {
                            U32 lol = m_unk4F54;
                            if (lol != 0) {
                                GetMission(lol - 1).UnloadConfigFile();
                            }
                            if (lol != 0) {
                                GetMission(lol - 1).LoadConfigFile(0);
                            }
                        }
                        m_unk4F54 = fortnite;
                        m_current_loading_callback->Destroy();
                        CDkFileSys::UnSetCallBackOnLoad();
                        iVar12 = 7;
                        break;
                    }

                    case 3:
                        GetCurrentMission()->m_unk2C = 1;
                        m_game_backup->GetFromGameData(1);

                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }

                        GetCurrentMission()->UnloadConfigFile();
                        switch (m_unk4F54) {
                            case 1:
                                PlayVideo(16);
                                if (GetMission(1).m_unk2C != 0) {
                                    PlayVideo(19);
                                }
                                break;
                            case 2:
                                PlayVideo(18);
                                if (GetMission(0).m_unk2C != 0) {
                                    PlayVideo(19);
                                }
                                break;
                            case 3:
                                PlayVideo(21);
                                if (GetMission(3).m_unk2C != 0) {
                                    PlayVideo(24);
                                }
                                break;
                            case 4:
                                PlayVideo(23);
                                if (GetMission(2).m_unk2C != 0) {
                                    PlayVideo(24);
                                }
                                break;
                            case 5:
                                PlayVideo(26);
                                if (GetMission(5).m_unk2C != 0) {
                                    PlayVideo(29);
                                }
                                break;
                            case 6:
                                PlayVideo(28);
                                if (GetMission(4).m_unk2C != 0) {
                                    PlayVideo(29);
                                }
                                break;
                            case 7:
                                PlayVideo(30);
                                iVar24 = 1;
                                break;
                        }

                        if (m_unk8 & (1 << 3)) {
                            iVar12 = 9;
                        } else if (iVar24 != 0) {
                            iVar12 = 1;
                        } else {
                            m_unk210[7].LoadConfigFile(0);
                            m_unk4F54 = 8;
                            m_unk4F58 = 3;
                            m_unk4F5C = 0;
                            m_unk4F60 = 0.0f;
                            m_unk4F64 = 0.0f;
                            m_unk4F68 = 0.0f;
                            m_unk4F6C = 0.0f;
                            m_unk4F70 = 0.0f;
                            m_unk4F74 = 0.0f;
                            m_unk5090 = 1;
                            GetCurrentMission()->m_rooms[3] |= (1 << 0);
                            m_game_backup->GetFromGameData(1);
                            iVar12 = 7;
                        }

                        break;

                    case 4:
                        m_game_backup->GetFromGameData(1);

                        if (m_unk8 & (1 << 3)) {
                            iVar12 = 9;
                        } else {
                            if (m_game_part != NULL) {
                                delete m_game_part;
                                m_game_part = NULL;
                            }

                            if (m_unk4F54 == 7) {
                                m_unk210[6].Initialize();
                                m_unk210[6].m_game = this;
                                m_unk210[6].m_mission_no = 7;
                                m_unk210[6].LoadConfigFile(1);
                                bVar8 = TRUE;
                            }

                            if (m_unk4F5C == 0) {
                                CMission* mission = GetCurrentMission();
                                F32 start_pos_x = mission->m_start_position.x;
                                F32 start_pos_y = mission->m_start_position.y;
                                F32 start_pos_z = mission->m_start_position.z;
                                F32 start_rot_x = mission->m_start_rotation.x;
                                F32 start_rot_y = mission->m_start_rotation.y;
                                F32 start_rot_z = mission->m_start_rotation.z;

                                m_unk4F58 = mission->m_unkC;
                                m_unk4F5C = GetCurrentMission()->m_unk10;
                                m_unk4F60 = start_pos_x;
                                m_unk4F64 = start_pos_y;
                                m_unk4F68 = start_pos_z;
                                m_unk4F6C = start_rot_x;
                                m_unk4F70 = start_rot_y;
                                m_unk4F74 = start_rot_z;
                                m_unk5090 = 0;

                                GetMission(m_unk4F54 - 1).m_rooms[GetMission(m_unk4F54 - 1).m_unkC] |= (1 << 0);

                                m_game_backup->GetFromGameData(1);
                                iVar12 = 7;
                            } else if (m_unk4F5C == 1) {
                                if (m_unk4F54 == 2) {
                                    m_unk4F58 = 4;
                                    m_mailbox->SendMessage("Tigger", "RTC_204_02_B", "START", 1);
                                } else if (m_unk4F54 == 5) {
                                    m_unk4F58 = 9;
                                    m_mailbox->SendMessage("Tigger", "RTC_509_09", "START", 1);
                                }

                                m_unk4F5C = 1;
                                m_unk4F60 = 0.0f;
                                m_unk4F64 = 0.0f;
                                m_unk4F68 = 0.0f;
                                m_unk4F6C = 0.0f;
                                m_unk4F70 = 0.0f;
                                m_unk4F74 = 0.0f;
                                m_unk5090 = 1;

                                GetMission(m_unk4F54 - 1).m_rooms[m_unk4F58] |= (1 << 0);

                                m_game_backup->GetFromGameData(1);
                                iVar12 = 7;
                            } else if (m_unk4F5C == 2) {
                                if (m_unk4F54 == 3) {
                                    m_unk4F58 = 3;
                                    m_mailbox->SendMessage("Winnie", "RTC_303_03_B", "START", 1);
                                } else if (m_unk4F54 == 6) {
                                    m_unk4F58 = 9;
                                    m_mailbox->SendMessage("Winnie", "RTC_609_03", "START", 1);
                                }

                                m_unk4F5C = 2;
                                m_unk4F60 = 0.0f;
                                m_unk4F64 = 0.0f;
                                m_unk4F68 = 0.0f;
                                m_unk4F6C = 0.0f;
                                m_unk4F70 = 0.0f;
                                m_unk4F74 = 0.0f;
                                m_unk5090 = 1;

                                GetMission(m_unk4F54 - 1).m_rooms[m_unk4F58] |= (1 << 0);

                                m_game_backup->GetFromGameData(1);
                                iVar12 = 7;
                            }
                        }
                        break;

                    case 5:
                        m_game_backup->GetFromGameData(1);

                        if (m_unk8 & (1 << 3)) {
                            iVar12 = 9;
                        } else {
                            if (m_game_part != NULL) {
                                delete m_game_part;
                                m_game_part = NULL;
                            }

                            if (m_unk4F54 != 8) {
                                if (m_unk4F54 != 0) {
                                    GetCurrentMission()->UnloadConfigFile();
                                }
                                GetMission(7).LoadConfigFile(0);
                            }

                            m_unk4F54 = 8;
                            m_unk4F58 = 3;
                            m_unk4F5C = 0;
                            m_unk4F60 = 0.0f;
                            m_unk4F64 = 0.0f;
                            m_unk4F68 = 0.0f;
                            m_unk4F6C = 0.0f;
                            m_unk4F70 = 0.0f;
                            m_unk4F74 = 0.0f;
                            m_unk5090 = 1;

                            GetCurrentMission()->m_rooms[3] |= (1 << 0);

                            m_game_backup->GetFromGameData(1);
                            iVar12 = 7;
                        }
                        break;

                    case 7:
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }
                        iVar12 = 1;
                        break;

                    case 8:
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }
                        iVar12 = 2;
                        break;

                    case 9:
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }
                        iVar12 = 3;
                        break;

                    case 13: {
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }
                        int video_id = m_opcode_buffer[i++];
                        PlayVideo(video_id);
                        break;
                    }

                    case 11:
                        m_game_backup->GetFromGameData(1);
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }
                        iVar12 = 4;
                        break;

                    case 6:
                        bVar8 = TRUE;
                        break;

                    case 14:
                        bVar9 = TRUE;
                        break;

                    case 10:
                        if (m_game_part != NULL) {
                            delete m_game_part;
                            m_game_part = NULL;
                        }

                        for (U32 j = 0; j < 8; j++) {
                            m_unk210[j].Initialize();
                            m_unk210[j].m_game = this;
                            m_unk210[j].m_mission_no = j + 1;
                            m_unk210[j].LoadConfigFile(1);
                        }

                        m_game_backup->CreateNewGame();
                        m_game_backup->GetFromGameData(1);
                        m_game_backup->Backup();

                        iVar12 = 0;
                        break;
                }
            }

            m_opcode_buffer_size = 0;
            m_opcode_buffer[0] = -1;
        }

        if (m_game_part != NULL) {
            delete m_game_part;
            m_game_part = NULL;
        }

        if (m_unk505C != NULL) {
            m_anim_dictionary->RemoveController(m_unk505C);
            m_unk505C = NULL;
        }

        if (m_batch5050 != NULL) {
            m_display_engine->GetImmediate()->RemoveBatch2D(m_batch5050);
        }
        m_batch5050 = NULL;

        RwFreeListPurgeAllFreeLists();

        F32 volume = m_sound_engine->GetGlobalVolume();
        m_sound_engine->SetGlobalVolume(0.0f);
        m_sound_engine->BeginUpdate();
        m_sound_engine->EndUpdate();

        CDKW_RGBA fade_color;
        switch (iVar12) {
            case 8:
                m_game_part = new CGamePartIntroduction;
                break;
            case 1: {
                CDkFileSys::UnSetCallBackOnLoad();

                U32 rf_unkC = m_resource_factory->m_unkC;
                m_resource_factory->m_unkC = 0;
                m_game_part = new CGamePartCredits(this, iVar24);
                m_resource_factory->m_unkC = rf_unkC;

                m_game_backup->Backup();
                m_game_backup->ApplyToGameData();

                m_timer->Reset();
                ComputeDeltaTime();

                break;
            }
            case 2: {
                CDkFileSys::UnSetCallBackOnLoad();

                U32 rf_unkC = m_resource_factory->m_unkC;
                m_resource_factory->m_unkC = 0;
                m_game_part = new CGamePartMovieClips(this);
                m_resource_factory->m_unkC = rf_unkC;

                m_game_backup->Backup();
                m_game_backup->ApplyToGameData();

                m_timer->Reset();
                ComputeDeltaTime();

                break;
            }
            case 3: {
                CDkFileSys::UnSetCallBackOnLoad();

                U32 rf_unkC = m_resource_factory->m_unkC;
                m_resource_factory->m_unkC = 0;
                m_game_part = new CGamePartScrapBook(this);
                m_resource_factory->m_unkC = rf_unkC;

                m_game_backup->Backup();
                m_game_backup->ApplyToGameData();

                m_timer->Reset();
                ComputeDeltaTime();

                break;
            }
            case 4: {
                CDkFileSys::UnSetCallBackOnLoad();

                U32 rf_unkC = m_resource_factory->m_unkC;
                m_resource_factory->m_unkC = 0;
                m_game_part = new CGamePartStartScreen(this);
                m_resource_factory->m_unkC = rf_unkC;

                m_game_backup->Backup();
                m_game_backup->ApplyToGameData();

                m_timer->Reset();
                ComputeDeltaTime();

                break;
            }
            case 6:
                m_game_part = new CGamePartFrontend(this);
                break;
            case 7: {
                switch (m_unk4F5C) {
                    case 1:
                        m_current_loading_callback = (CBaseLoadingCallback*)m_loading_tigger;
                        m_screen_effect->SetSequenceByIndex(4);
                        break;
                    case 2:
                        m_current_loading_callback = (CBaseLoadingCallback*)m_loading_winnie;
                        m_screen_effect->SetSequenceByIndex(3);
                        break;
                    case 3:
                        m_current_loading_callback = (CBaseLoadingCallback*)m_loading_catch_them_all;
                        break;
                    default:
                        m_current_loading_callback = (CBaseLoadingCallback*)m_loading_adventure;
                        break;
                }

                m_current_loading_callback->Create();
                CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

                m_game_part = new CGamePartIngame(this);

                m_game_backup->Backup();
                m_game_backup->ApplyToGameData();

                fade_color = ComputeGameFadeColor();
                ((CGamePartIngame*)m_game_part)->m_game_room_manager->m_unk154 = (fade_color.red << 0) | (fade_color.green << 8) | (fade_color.blue << 16) | (0x80 << 24);
                m_current_loading_callback->Destroy();

                if (!HasEntityOfType(ENTITY_SEARCHABLE_ZONE)) {
                    GetCurrentMission()->m_rooms[m_unk4F58] |= (1 << 2);
                }

                CDkFileSys::UnSetCallBackOnLoad();
                m_current_loading_callback = m_ingame_loading_callback;
                m_current_loading_callback->Create();
                CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

                m_timer->Reset();
                ComputeDeltaTime();

                if (bVar8 && m_unk4F54 != 8 && GetCurrentMission()->GetUnk34() == 0) {
                    GetCurrentMission()->GetUnk34() = 1;

                    m_sound_engine->SetGlobalVolume(volume);
                    m_sound_engine->BeginUpdate();
                    m_sound_engine->EndUpdate();

                    m_game_backup->GetFromGameData(0);
                    m_game_backup->Backup();

                    if (GetCurrentMission()->GetStartRTC() != "") {
                        m_mailbox->SendMessage("Piglet", GetCurrentMission()->GetStartRTC(), "START", 0);
                    }
                } else if (bVar8 && m_unk4F54 == 8 && m_unk4F58 == 1) {
                    m_mailbox->SendMessage("Piglet", "RTC_801_01", "START", 0);
                }

                if (bVar9) {
                    m_unk8 |= m_unk8 | (1 << 7);
                }

                break;
            }
            case 9: {
                if (m_unk4F54 != 0 && m_unk4F54 != 0) {
                    GetCurrentMission()->UnloadConfigFile();
                }
                m_unk4F54 = 0;

                U32 rf_unkC = m_resource_factory->m_unkC;
                m_resource_factory->m_unkC = 0;
                m_game_part = new CGamePartDMRoomLauncher(this);
                m_resource_factory->m_unkC = rf_unkC;

                break;
            }
            case 10:
                return FALSE;
        }

        m_sound_engine->SetGlobalVolume(volume);
        m_sound_engine->BeginUpdate();
        m_sound_engine->EndUpdate();
    }

    return TRUE;
}

F32 CGame::GetDeltaTime() {
    return m_delta_time;
}

void CGame::ComputeDeltaTime() {
    F32 old_dt = m_delta_time;
    m_delta_time = m_timer->GetDeltaTime();
    m_delta_time += old_dt;
    m_delta_time *= 0.5f;
}

CGamePart* CGame::GetGamePartPointer() {
    return m_game_part;
}

void CGame::SetCurrentRoomReturnType(ERoomReturnType return_type, int a2) {
    if (a2 == -1) {
        m_room_return_type = return_type;
    } else {
        m_room_return_type = return_type | (a2 << 4);
    }
}

void CGame::ResetOpcodeBuffer() {
    m_opcode_buffer_size = 0;
    m_opcode_buffer[0] = -1;
}

void CGame::PushOpcodeValue(int opcode) {
    m_opcode_buffer[m_opcode_buffer_size] = opcode;
    m_opcode_buffer_size++;
    m_opcode_buffer[m_opcode_buffer_size] = -1;
}

// Incomplete
BOOL CGame::LoadConfigFile(char* config_xmd_filename) {
    void* config_xmd_data = m_resource_factory->LoadPureFile(config_xmd_filename, NULL);
    if (config_xmd_data == NULL) {
        return FALSE;
    }

    DkXmd::CXmdFile config_xmd;
    if (!config_xmd.Parse(config_xmd_data)) {
        delete config_xmd_data;
        return FALSE;
    }

    char buf[256];
    DkXmd::CChunkIterator iter = config_xmd.m_chunk_iterator;
    DkXmd::CChunkIterator dest, dest2;
    if (iter.GetFirstChildChunk(dest)) {
        do {
            strcpy(buf, dest.GetName());
            if (strcmp(buf, "TexDict") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(6, dest.GetStringValue());
            }
        } while (dest.GetNextSiblingChunk(dest));
    }
    if (iter.GetFirstChildChunk(dest)) {
        do {
            strcpy(buf, dest.GetName());
            if (strcmp(buf, "Mesh") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(1, dest.GetStringValue());
            } else if (strcmp(buf, "ANM") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(3, dest.GetStringValue());
            } else if (strcmp(buf, "DMA") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(4, dest.GetStringValue());
            } else if (strcmp(buf, "TAN") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(5, dest.GetStringValue());
            } else if (strcmp(buf, "NoNPC") == 0) {
                m_unk8 |= (1 << 0);
            } else if (strcmp(buf, "ParticleEmitterFile") == 0) {
                if (dest.GetStringValue() != NULL) {
                    CResourceFactory* resource_factory = m_resource_factory;
                    resource_factory->LoadResource(9, dest.GetStringValue());
                }
            } else if (strcmp(buf, "SndDict") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(10, dest.GetStringValue());
            } else if (strcmp(buf, "StreamDict") == 0) {
                CResourceFactory* resource_factory = m_resource_factory;
                resource_factory->LoadResource(12, dest.GetStringValue());
            } else if (strcmp(buf, "GUI") == 0) {
                m_gui_manager->LoadGui(&dest, m_resource_factory->m_unkC);
            } else if (strcmp(buf, "RTCCamFight") == 0) {
                ParseRTCCamFight(dest);
            } else if (strcmp(buf, "SCRFX") == 0) {
                if (m_screen_effect != NULL) {
                    continue;
                }

                m_screen_effect = new CScreenEffect(this);
                if (m_screen_effect != NULL) {
                    m_screen_effect->Parse(dest);
                    m_screen_effect->SetSequenceByIndex(0);
                }
            }
        } while (dest.GetNextSiblingChunk(dest));
    }

    delete config_xmd_data;
    return TRUE;
}

// Inlined (and incomplete), see CGame::LoadConfigFile
void CGame::ParseRTCCamFight(DkXmd::CChunkIterator iter) {
    char buf[256];
    DkXmd::CChunkIterator dest;
    size_t anim = NULL;

    if (m_unk5060 == NULL) {
        m_resource_factory->LoadResource(1, "MISSIONS\\RTCCamera.dff");
        if ((m_unk5060 = m_scene->CloneClump("MISSIONS\\RTCCamera.dff", NULL)) == NULL) {
            return;
        }
    }
    if (m_unk505C == NULL && m_unk5060 != NULL) {
        m_unk505C = m_anim_dictionary->CreateController("RTCCamCtrl", m_unk5060, 6);
        m_unk5060->SetController(m_unk505C);
    }

    strcpy(buf, iter.GetStringValue());
    m_resource_factory->LoadResource(3, buf);
    anim = reinterpret_cast<size_t>(m_anim_dictionary->FindAnimation(buf));
    AS_ULONG_VECTOR_HACK(m_unk5064).push_back(anim);
}

// Equivalent: std::string operator!=
void CGame::PlayNarratorLine(std::string narrator_line) {
    if (m_unk5004 != narrator_line) {
        StopNarratorLine();
        m_unk5004.assign(narrator_line, 0);
        DKSND::CSound2D* narrator_sound = m_sound_engine->PlayStreamedSound(&narrator_line, 0);
        if (narrator_sound != NULL) {
            narrator_sound->SetVolume(1.0f);
            narrator_sound->SetLayer(1);
            narrator_sound->SetLoopMode(0);
        }
        m_unk5008 = narrator_sound;
    } else if (m_unk5008 != NULL && m_unk5008->IsFinished()) {
        StopNarratorLine();
        m_unk5004.assign(narrator_line, 0);
        DKSND::CSound2D* narrator_sound = m_sound_engine->PlayStreamedSound(&narrator_line, 0);
        if (narrator_sound != NULL) {
            narrator_sound->SetVolume(1.0f);
            narrator_sound->SetLayer(1);
            narrator_sound->SetLoopMode(0);
        }
        m_unk5008 = narrator_sound;
    }
}

void CGame::StopNarratorLine() {
    m_unk5004 = "";
    m_unk5008 = NULL;
    m_sound_engine->StopStreamedSound();
}

void CGame::FadeInit(F32 duration, ERommFadeType fade_type, U8 red, U8 green, U8 blue, F32 a6) {
    m_unk502C = 0.0f;
    m_fade_duration = duration;
    m_fade_type = fade_type;
    m_unk5038 = 0;
    m_unk5040 = 0.0f;
    m_fade_color = (red << 0) | (green << 8) | (blue << 16);
    m_unk5044 = a6;
    m_unk503C = (1 << 2);
    m_unk5048 = 0.1f;
    m_unk503C |= (1 << 3);
}

int CGame::FadeUpdate(F32 unk) {
    m_unk5040 = unk;
    if (m_unk5038 == 0) {
        if (FadeIn(-1.0f) == 0) {
            if (m_unk503C & (1 << 1)) {
                if (m_unk5038 == 3) {
                    m_unk5038 = 3;
                } else {
                    m_unk5038 = 2;
                }
                m_unk502C = 0.0f;
            } else {
                m_unk5038 = 1;
                m_unk502C = 0.0f;
                m_unk5044 = 0.0f;
                FadeOut(-1.0f);
            }
        }
    } else if (m_unk5038 == 1 && FadeOut(-1.0f) == 0) {
        if (m_unk5038 == 3) {
            m_unk5038 = 3;
        } else {
            m_unk5038 = 2;
        }
        m_unk502C = 0.0f;
    }

    return m_unk5038;
}

void CGame::RenderFade() {
    if (m_batch5050 == NULL) {
        return;
    }

    m_display_engine->SetRenderState((RwRenderState)12, (void*)1);
    m_display_engine->SetRenderState((RwRenderState)10, (void*)5);
    m_display_engine->SetRenderState((RwRenderState)11, (void*)6);
    m_display_engine->SetRenderState((RwRenderState)1, (void*)0);

    m_display_engine->SetRenderState((RwRenderState)8, (void*)0);
    m_display_engine->AlphaEnable();
    m_batch5050->Render(rwPRIMTYPETRIFAN);
    m_display_engine->SetRenderState((RwRenderState)8, (void*)1);
}

BOOL CGame::IsGUIDisplayNotAdvised() {
    if (m_game_part == NULL) {
        return TRUE;
    }

    return (((CGamePartIngame*)m_game_part)->m_game_room_manager->GetState() < 2) ? TRUE : FALSE;
}

void CGame::RegisterVideo(int id, std::string filename) {
    SVideoDesc desc;
    std::string filename_;

    desc.id = id;

    filename_.assign(filename, 0);
    filename_ += ".THP";
    desc.filename.assign(filename_, 0);

    m_video_descs.push_back(desc);
}

// Incomplete
void CGame::PlayVideo(int id) {
    CVideoLoadingCallback* callback = NULL;
    if (m_current_loading_callback != m_video_loading_callback) {
        callback = (CVideoLoadingCallback*)m_current_loading_callback;
    }
    if (callback != NULL) {
        callback->Destroy();
    }
    m_current_loading_callback = m_video_loading_callback;
    CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);

    for (U32 i = 0; i < m_video_descs.size(); i++) {
        SVideoDesc& desc = m_video_descs[i];
        if (id != m_video_descs[i].id) {
            continue;
        }
            if (RWFileInterface.rwfexist(m_video_descs[i].filename.c_str())) {
                m_video_engine->SetCallBack(ReplayVideoCallback);
                m_video_engine->SetVolume(m_sound_engine->GetGlobalVolume());
                m_video_engine->Play((char*)m_video_descs[i].filename.c_str());
            }

            m_gui_manager->Reset();
            m_gui_manager->Update(1.0f/30.0f);

            if (callback != NULL) {
                m_current_loading_callback = callback;
                callback->Create();
                CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);
            }

            return;
    }

    m_gui_manager->Reset();
    m_gui_manager->Update(1.0f/30.0f);

    if (callback != NULL) {
        m_current_loading_callback = callback;
        callback->Create();
        CDkFileSys::SetCallBackOnLoad(m_current_loading_callback, 1);
    }
}

void CGame::ReplayVideoCallback() {
    DKI::IInputEngine::Update(1.0f/30.0f);
    ManageReset();

    if (gs_CurrentGuiManager != NULL && gs_CurrentGuiManager->m_unk1C->GetState().m_unk0 == TRUE) {
        gs_CurrentVideoManager->Stop();
    }
}

CDKW_RGBA CGame::ComputeGameFadeColor() {
    CDKW_RGBA color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;
    color.alpha = 0xFF;

    if (m_game_part != NULL && m_game_part->m_unk0 == 7) {
        CEntityHero* hero = m_entity_manager->GetHero();
        if (hero != NULL && hero->GetType() == ENTITY_PIGLET) {
            switch (hero->GetState()) {
                case 1:
                    color = CDKW_RGBA(26, 26, 67, 0xFF);
                    break;
                case 2:
                    color = CDKW_RGBA(0, 0, 0, 0xFF);
                    break;
                default:
                    color = CDKW_RGBA(199, 226, 222, 0xFF);
                    break;
            }

            if (m_unk4F5C == 3) {
                color = CDKW_RGBA(78, 83, 149, 0xFF);
            }
        } else if (hero != NULL && hero->GetType() == ENTITY_TIGGER) {
            color = CDKW_RGBA(229, 120, 15, 0xFF);
        } else if (hero != NULL && hero->GetType() == ENTITY_WINNIE) {
            color = CDKW_RGBA(229, 120, 15, 0xFF);
        }
    }

    return CDKW_RGBA(color);
}

static inline void UpdateResetButton() {
    if (OSGetResetButtonState() != FALSE) {
        s_bResetButtonPushed = TRUE;
        return;
    } else if (!s_bResetButtonPushed) {
        return;
    }

    if (OSGetResetButtonState() != FALSE) {
        s_bResetButtonPushed = TRUE;
        return;
    }
}

// Equivalent: stack length, reset button stuff may be inlined?
void CGame::ManageReset() {
    if (OSGetResetButtonState() != FALSE) {
        s_bResetButtonPushed = TRUE;
        return;
    } else if (!s_bResetButtonPushed) {
        return;
    }

    if (OSGetResetButtonState() != FALSE) {
        s_bResetButtonPushed = TRUE;
        return;
    }

    DKI::IInputEngine::GetDevice(0)->StopVibration();
    DKI::IInputEngine::Update(1.0f/30.0f);

    while (!PADRecalibrate(PAD_CHAN0_BIT)) {
        PADStatus status;
        do {
            PADReset(PAD_CHAN0_BIT);
            PADRead(&status);
        } while (status.err == PAD_ERR_NO_CONTROLLER);
    }

    if (DkSoundGetEngine() != NULL) {
        DkSoundGetEngine()->StopStreamedSound();
    }

    if (DkDisplayGetEngine() != NULL) {
        DKDSP::CScene* scene = DkDisplayGetEngine()->GetScene(0);
        if (scene != NULL) {
            scene->Clear(0, 0.0f, 0.0f, 0.0f);
            scene->BeginRender();
            scene->EndRender();
            scene->Flip(1);

            scene->Clear(0, 0.0f, 0.0f, 0.0f);
            scene->BeginRender();
            scene->EndRender();
            scene->Flip(1);
        }
    }

    GXDrawDone();
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();

    OSResetSystem(FALSE, 0, FALSE);
}

void CMemoryCardSaveEventCallback::OnSave() {
    UpdateResetButton();
}
