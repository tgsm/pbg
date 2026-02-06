#ifndef PIGLET_CGAME_H
#define PIGLET_CGAME_H

#include <string>
#include <vector>
#include "engine/backup/CGCNBAKEngine.h"
#include "engine/backup/IBAKEngine.h"
#include "engine/display/CAnimDictionary.h"
#include "engine/display/CCamera.h"
#include "engine/display/CController.h"
#include "engine/display/CEngine.h"
#include "engine/display/CIm2DBatch.h"
#include "engine/display/CObjectDictionary.h"
#include "engine/display/CScene.h"
#include "engine/display/CTextureDictionary.h"
#include "engine/display/CTimer.h"
#include "engine/gui/CGUIEngine.h"
#include "engine/input/IInputEngine.h"
#include "engine/sound/CSampleDictionary.h"
#include "engine/sound/CSoundEngine.h"
#include "engine/video/CVideoEngineGCN.h"
#include "engine/wrap/DKW_RGBA.h"
#include "entities/CEntity.h"
#include "entities/CEntityManager.h"
#include "CEventAnimationHandlers.h"
#include "CFullScreenEffect.h"
#include "CFxManager.h"
#include "CGameBackup.h"
#include "CGamePart.h"
#include "CGuiManager.h"
#include "CLoading.h"
#include "CLoadingAdventure.h"
#include "CLoadingCatchThemAll.h"
#include "CLoadingTigger.h"
#include "CLoadingWinnie.h"
#include "CMailBox.h"
#include "CMiniMap.h"
#include "CMission.h"
#include "CResourceFactory.h"
#include "CShadowZone.h"

class CGamePartIngame;

struct SVideoDesc {
    int id;
    std::string filename;
};

class CMemoryCardSaveEventCallback : public IEventBackupCB {
public:
    virtual ~CMemoryCardSaveEventCallback() {}
    virtual void OnSave();
};

// Very TODO, there's a lot of stuff in this class
class CGame {
public:
    enum ERommFadeType {
        FADE_TYPE_0,
        FADE_TYPE_2 = 2,
        FADE_TYPE_3 = 3,
        FADE_TYPE_4 = 4,
    };

    enum ERoomReturnType {
        RETURN_TYPE_0 = 0,
        RETURN_TYPE_1 = 1,
        RETURN_TYPE_2 = 2,
    };

public:
    DKBAK::DKBAK_DATE m_backup_date;
    U32 m_unk8;
    int m_opcode_buffer[128];
    U32 m_opcode_buffer_size;
    CMission m_unk210[8];
    CMission m_unk28B0[8];
    F32 m_delta_time;
    U32 m_unk4F54;
    U32 m_unk4F58;
    U32 m_unk4F5C;
    F32 m_unk4F60;
    F32 m_unk4F64;
    F32 m_unk4F68;
    F32 m_unk4F6C;
    F32 m_unk4F70;
    F32 m_unk4F74;
    int m_room_return_type;
    DKGUI::CGUIEngine* m_gui_engine;
    DKSND::CSoundEngine* m_sound_engine;
    DKSND::CSampleDictionary* m_sample_dictionary;
    DKDSP::CEngine* m_display_engine;
    DKDSP::CTextureDictionary* m_texture_dictionary;
    DKDSP::CObjectDictionary* m_object_dictionary;
    DKDSP::CAnimDictionary* m_anim_dictionary;
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    CEntityManager* m_entity_manager;
    CMiniMap* m_minimap;
    CResourceFactory* m_resource_factory;
    DKDSP::CTimer* m_timer;
    CMailBox* m_mailbox;
    CBaseLoadingCallback* m_current_loading_callback;
    CInGameLoadingCallback* m_ingame_loading_callback;
    CLoadingAdventure* m_loading_adventure;
    CLoadingCatchThemAll* m_loading_catch_them_all;
    CLoadingTigger* m_loading_tigger;
    CLoadingWinnie* m_loading_winnie;
    CBootUpLoadingCallback* m_bootup_loading_callback;
    CPreBootUpLoadingCallback* m_prebootup_loading_callback;
    CVideoLoadingCallback* m_video_loading_callback;
    CErrorCallback* m_error_callback;
    CGuiManager* m_gui_manager;
    CFxManager* m_fx_manager;
    CShadowZone* m_shadow_zone;
    CGameBackup* m_game_backup;
    DKBAK::CGCNBAKEngine* m_backup_engine;
    DKVIDEO::CVideoEngineGCN* m_video_engine;
    std::vector<SVideoDesc> m_video_descs;
    CScreenEffect* m_screen_effect;
    std::string m_unk5004;
    DKSND::CSound2D* m_unk5008;
    CGamePart* m_game_part;
    CFXEventCallback m_fx_event_callback;
    CSNDEventCallback m_snd_event_callback;
    CVIBEventCallback m_vib_event_callback;
    CVIB2DEventCallback m_vib2d_event_callback;
    CShakeEventCallback m_shake_event_callback;
    CMemoryCardSaveEventCallback m_memory_card_save_event_callback;
    U32 m_fade_color;
    F32 m_unk502C;
    F32 m_fade_duration;
    U32 m_fade_type;
    int m_unk5038;
    U8 m_unk503C;
    U8 m_unk503D[3];
    F32 m_unk5040;
    F32 m_unk5044;
    F32 m_unk5048;
    F32 m_unk504C;
    DKDSP::CIm2DBatch* m_batch5050;
    size_t m_unk5054;
    U8 m_unk5058[0x505C - 0x5058];
    DKDSP::CController* m_unk505C;
    DKDSP::CClump* m_unk5060;
    std::vector<DKDSP::CAnimation*> m_unk5064;
    U8 m_unk5070[0x508C - 0x5070];
    DKI::IInput* m_unk508C;
    U32 m_unk5090;

    static CGuiManager* gs_CurrentGuiManager;
    static DKVIDEO::CVideoEngineGCN* gs_CurrentVideoManager;

public:
    CGame(void*, U32);
    virtual ~CGame();

    virtual BOOL NextFrame();

    static void ManageReset();
    static void ReplayVideoCallback();

    DKBAK::DKBAK_DATE& GetBackupDate() { return m_backup_date; }
    void SetBackupDate(DKBAK::DKBAK_DATE date) { m_backup_date = date; }
    DKGUI::CGUIEngine* GetGuiEngine() { return m_gui_engine; }
    DKDSP::CObjectDictionary* GetObjectDictionary() { return m_object_dictionary; }
    DKDSP::CScene* GetScene() { return m_scene; }
    DKDSP::CCamera* GetCamera() { return m_camera; }
    CResourceFactory* GetResourceFactory() { return m_resource_factory; }
    CMailBox* GetMailbox() { return m_mailbox; }
    CGuiManager* GetGuiManager() { return m_gui_manager; }
    CGameBackup* GetGameBackup() { return m_game_backup; }
    DKBAK::CGCNBAKEngine* GetBackupEngine() { return m_backup_engine; }

    U32& GetFlags() { return m_unk8; }

    BOOL IsUnk5038Not2() { return m_unk5038 == 2 ? 0 : 1; }

    CMission& GetMission(int index) { return m_unk210[index]; }
    CMission* GetCurrentMission() { return &m_unk210[m_unk4F54 - 1]; }
    U32 GetUnk4F58() { return m_unk4F58; }

    CGamePartIngame* GetIngameGamePart() { return (CGamePartIngame*)GetGamePartPointer(); }

    BOOL HasEntityOfType(U32 type) {
        BOOL result = FALSE;
        for (U32 i = 0; i < m_entity_manager->GetEntityCount(); i++) {
            if (m_entity_manager->GetEntity(i)->GetType() == type) {
                result = TRUE;
                break;
            }
        }
        return result;
    }

    F32 FadeRelatedInline() {
        F32 fVar1;
        if (m_unk5038 != 0) {
            fVar1 = 0.0f;
        } else {
            fVar1 = 0.0f;
            if (m_fade_duration != 0.0f) {
                fVar1 = m_unk502C / m_fade_duration;
            }
        }

        return fVar1;
    }

    BOOL MissionUnk30Inline(int no) {
        BOOL ret = GetMission(no).m_unk30 == 0;
        if (ret != 0) {
            ret = GetMission(no).m_unk2C;
            if (ret != 0) {
                ret = GetMission(no + 1).m_unk30 == 0;
                if (ret != 0) {
                    ret = GetMission(no + 1).m_unk2C;
                    if (ret == 0) {
                        return ret;
                    }
                }
            }
        }
        return ret;
    }

    BOOL MissionUnk30Inline2(int no) {
        BOOL ret = GetMission(no).m_unk30;
        if (ret != 0) {
            ret = GetMission(no).m_unk2C;
            if (ret != 0) {
                ret = GetMission(no + 1).m_unk30;
                if (ret != 0) {
                    ret = GetMission(no + 1).m_unk2C;
                    if (ret == 0) {
                        return ret;
                    }
                }
            }
        }
        return ret;
    }

    F32 GetUnk502C() { return m_unk502C; }
    F32 GetFadeDuration() { return m_fade_duration; }

    F32 GetDeltaTime();
    void ComputeDeltaTime();
    CGamePart* GetGamePartPointer();
    void SetCurrentRoomReturnType(ERoomReturnType return_type, int a2);
    void ResetOpcodeBuffer();
    void PushOpcodeValue(int opcode);
    BOOL LoadConfigFile(char* filename);
    CDKW_V2d GetScreenOffset();
    void SetScreenOffset(CDKW_V2d offset);
    void ParseRTCCamFight(DkXmd::CChunkIterator iter);
    void PlayNarratorLine(std::string line_id);
    void StopNarratorLine();
    void FadeInit(F32 duration, ERommFadeType fade_type, U8 red, U8 green, U8 blue, F32 a6);
    int FadeUpdate(F32);
    int FadeIn(F32 a1);
    int FadeOut(F32 a1);
    void RenderFade();
    BOOL IsGUIDisplayNotAdvised();
    void UpdateRTCCamera(F32 dt);
    void RegisterVideo(int id, std::string filename);
    void PlayVideo(int id);
    CDKW_RGBA ComputeGameFadeColor();
};
REQUIRE_SIZE(CGame, 0x5098);

#endif
