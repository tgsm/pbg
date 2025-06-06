#include "CGame.h"
#include "CGamePartIngame.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityManager.h"
#include <cstring>
#include <iostream>

CGuiManager* CGame::gs_CurrentGuiManager;

// *Very* incomplete
CGame::CGame(void* a1, u32 a2) {
    m_unk8 = a2;
    m_game_part = NULL;
    m_display_engine = NULL;
    m_texture_dictionary = NULL;
    m_anim_dictionary = NULL;
    m_entity_manager = NULL;
    m_resource_factory = NULL;
    m_timer = NULL;
    m_scene = NULL;
    m_mailbox = NULL;
    m_sound_engine = NULL;
    m_gui_engine = NULL;
    m_gui_manager = NULL;
    m_fx_manager = NULL;
    m_game_backup = NULL;
    m_backup_engine = NULL;
    m_unk5038 = 2;
    m_unk505C = NULL;
    m_unk5060 = NULL;
    m_unk5064.reserve(40);
    // m_video_descs.clear();
    m_video_descs.reserve(16);
    m_unk5004 = "";
    m_delta_time = (1.0f/60.0f);
    m_screen_effect = NULL;
    m_display_engine->SetCharsetCreation(FALSE);

    if (m_display_engine != NULL) {
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
    }

    // ...

    m_entity_manager = new CEntityManager(this);
    m_resource_factory = new CResourceFactory(this);

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
}

// *Very* incomplete
CGame::~CGame() {
    if (m_unk508C != NULL) {
        DKI::IInputEngine::DestroyInput(m_unk508C);
        m_unk508C = NULL;
    }

    // ...

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

    // ...

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
    }
    m_screen_effect = NULL;

    // ...

    if (m_texture_dictionary != NULL) {
        m_texture_dictionary->Release();
    }
    m_texture_dictionary = NULL;

    if (m_anim_dictionary != NULL) {
        m_anim_dictionary->Release();
    }
    m_anim_dictionary = NULL;

    // ...

    m_camera = NULL;

    delete m_scene;
    m_scene = NULL;
    delete m_sound_engine;
    m_sound_engine = NULL;

    // ...

    if (m_display_engine != NULL) {
        m_display_engine->Release();
    }
    m_display_engine = NULL;
    // DkDisplayRelease();

    DKI::IInputEngine::Clear();
    DKI::IInputEngine::Close();
}

f32 CGame::GetDeltaTime() {
    return m_delta_time;
}

void CGame::ComputeDeltaTime() {
    f32 old_dt = m_delta_time;
    m_delta_time = m_timer->GetDeltaTime();
    m_delta_time += old_dt;
    m_delta_time *= 0.5f;
}

CGamePart* CGame::GetGamePartPointer() {
    return m_game_part;
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
        if (m_unk5060 = m_scene->CloneClump("MISSIONS\\RTCCamera.dff", NULL), !m_unk5060) {
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
    m_unk5064.push_back(anim);
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

void CGame::FadeInit(f32 duration, ERommFadeType fade_type, u8 red, u8 green, u8 blue, f32 a6) {
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

int CGame::FadeUpdate(f32 unk) {
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

CDKW_RGBA CGame::ComputeGameFadeColor() {
    CDKW_RGBA color;
    color.m_r = 0;
    color.m_g = 0;
    color.m_b = 0;
    color.m_a = 0xFF;

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
