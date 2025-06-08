#ifndef PIGLET_CGAME_H
#define PIGLET_CGAME_H

#include <string>
#include <vector>
#include "engine/backup/CGCNBAKEngine.h"
#include "engine/display/CAnimDictionary.h"
#include "engine/display/CCamera.h"
#include "engine/display/CController.h"
#include "engine/display/CEngine.h"
#include "engine/display/CScene.h"
#include "engine/display/CTextureDictionary.h"
#include "engine/display/CTimer.h"
#include "engine/gui/CGUIEngine.h"
#include "engine/input/IInputEngine.h"
#include "engine/sound/CSoundEngine.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CFullScreenEffect.h"
#include "CFxManager.h"
#include "CGameBackup.h"
#include "CGamePart.h"
#include "CGuiManager.h"
#include "CMailBox.h"
#include "CMiniMap.h"
#include "CMission.h"
#include "CResourceFactory.h"
#include "CShadowZone.h"

class CEntityManager;

struct SVideoDesc {
    int id;
    std::string filename;
};

// Very TODO, there's a lot of stuff in this class
class CGame {
public:
    enum ERommFadeType {
        FADE_TYPE_0,
        FADE_TYPE_3 = 3,
        FADE_TYPE_4 = 4,
    };

    enum ERoomReturnType {
        RETURN_TYPE_0,
    };

public:
    U8 m_unk0[8];
    U32 m_unk8;
    int m_opcode_buffer[128];
    U32 m_opcode_buffer_size;
    CMission m_unk210[8];
    CMission m_unk28B0[8];
    F32 m_delta_time;
    U32 m_unk4F54;
    U32 m_unk4F58;
    U32 m_unk4F5C;
    U8 m_unk4F60[0x4F7C - 0x4F60];
    DKGUI::CGUIEngine* m_gui_engine;
    DKSND::CSoundEngine* m_sound_engine;
    U8 m_unk4F84[4];
    DKDSP::CEngine* m_display_engine;
    DKDSP::CTextureDictionary* m_texture_dictionary;
    U8 m_unk4F90[4];
    DKDSP::CAnimDictionary* m_anim_dictionary;
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    CEntityManager* m_entity_manager;
    CMiniMap* m_minimap;
    CResourceFactory* m_resource_factory;
    DKDSP::CTimer* m_timer;
    CMailBox* m_mailbox;
    U8 m_unk4FB4[0x4FDC - 0x4FB4];
    CGuiManager* m_gui_manager;
    CFxManager* m_fx_manager;
    CShadowZone* m_shadow_zone;
    CGameBackup* m_game_backup;
    DKBAK::CGCNBAKEngine* m_backup_engine;
    U8 m_unk4FF0[4];
    std::vector<SVideoDesc> m_video_descs;
    CScreenEffect* m_screen_effect;
    std::string m_unk5004;
    DKSND::CSound2D* m_unk5008;
    CGamePart* m_game_part;
    U8 m_unk5010[0x5028 - 0x5010];
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
    U8 m_unk5050[0x505C - 0x5050];
    DKDSP::CController* m_unk505C;
    DKDSP::CClump* m_unk5060;
    std::vector<size_t> m_unk5064;
    U8 m_unk5070[0x508C - 0x5070];
    DKI::IInput* m_unk508C;
    U8 m_unk5090[4];

    static CGuiManager* gs_CurrentGuiManager;

public:
    CGame(void*, U32);
    virtual ~CGame();

    virtual BOOL NextFrame();

    DKDSP::CCamera* GetCamera() { return m_camera; }
    CResourceFactory* GetResourceFactory() { return m_resource_factory; }
    CMailBox* GetMailbox() { return m_mailbox; }

    U32& GetFlags() { return m_unk8; }

    BOOL IsUnk5038Not2() { return m_unk5038 == 2 ? 0 : 1; }

    F32 GetDeltaTime();
    void ComputeDeltaTime();
    CGamePart* GetGamePartPointer();
    void SetCurrentRoomReturnType(ERoomReturnType return_type, int a2);
    void ResetOpcodeBuffer();
    void PushOpcodeValue(int opcode);
    BOOL LoadConfigFile(char* filename);
    void ParseRTCCamFight(DkXmd::CChunkIterator iter);
    void PlayNarratorLine(std::string line_id);
    void StopNarratorLine();
    void FadeInit(F32 duration, ERommFadeType fade_type, U8 red, U8 green, U8 blue, F32 a6);
    int FadeUpdate(F32);
    int FadeIn(F32 a1);
    int FadeOut(F32 a1);
    void RenderFade();
    BOOL IsGUIDisplayNotAdvised();
    void RegisterVideo(int id, std::string filename);
    CDKW_RGBA ComputeGameFadeColor();
};
REQUIRE_SIZE(CGame, 0x5098);

#endif
