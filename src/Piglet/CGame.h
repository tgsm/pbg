#ifndef PIGLET_CGAME_H
#define PIGLET_CGAME_H

#include <string>
#include "engine/backup/CGCNBAKEngine.h"
#include "engine/display/CAnimDictionary.h"
#include "engine/display/CCamera.h"
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
    u8 m_unk0[8];
    u32 m_unk8;
    int m_opcode_buffer[128];
    u32 m_opcode_buffer_size;
    CMission m_unk210[8];
    CMission m_unk28B0[8];
    f32 m_delta_time;
    u32 m_unk4F54;
    u32 m_unk4F58;
    u32 m_unk4F5C;
    u8 m_unk4F60[0x4F7C - 0x4F60];
    DKGUI::CGUIEngine* m_gui_engine;
    DKSND::CSoundEngine* m_sound_engine;
    u8 m_unk4F84[4];
    DKDSP::CEngine* m_display_engine;
    DKDSP::CTextureDictionary* m_texture_dictionary;
    u8 m_unk4F90[4];
    DKDSP::CAnimDictionary* m_anim_dictionary;
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    CEntityManager* m_entity_manager;
    CMiniMap* m_minimap;
    CResourceFactory* m_resource_factory;
    DKDSP::CTimer* m_timer;
    CMailBox* m_mailbox;
    u8 m_unk4FB4[0x4FDC - 0x4FB4];
    CGuiManager* m_gui_manager;
    CFxManager* m_fx_manager;
    CShadowZone* m_shadow_zone;
    CGameBackup* m_game_backup;
    DKBAK::CGCNBAKEngine* m_backup_engine;
    u8 m_unk4FF0[0x5000 - 0x4FF0];
    CScreenEffect* m_screen_effect;
    u8 m_unk5004[0x500C - 0x5004];
    CGamePart* m_game_part;
    u8 m_unk5010[0x5028 - 0x5010];
    u32 m_fade_color;
    f32 m_unk502C;
    f32 m_fade_duration;
    u32 m_fade_type;
    int m_unk5038;
    u8 m_unk503C;
    u8 m_unk503D[3];
    f32 m_unk5040;
    f32 m_unk5044;
    f32 m_unk5048;
    f32 m_unk504C;
    u8 m_unk5050[0x508C - 0x5050];
    DKI::IInput* m_unk508C;
    u8 m_unk5090[4];

    static CGuiManager* gs_CurrentGuiManager;

public:
    CGame(void*, u32);
    virtual ~CGame();

    virtual BOOL NextFrame();

    DKDSP::CCamera* GetCamera() { return m_camera; }
    CMailBox* GetMailbox() { return m_mailbox; }

    u32& GetFlags() { return m_unk8; }

    BOOL IsUnk5038Not2() { return m_unk5038 == 2 ? 0 : 1; }

    f32 GetDeltaTime();
    void ComputeDeltaTime();
    CGamePart* GetGamePartPointer();
    void SetCurrentRoomReturnType(ERoomReturnType return_type, int a2);
    void ResetOpcodeBuffer();
    void PushOpcodeValue(int opcode);
    BOOL LoadConfigFile(char* filename);
    void PlayNarratorLine(std::string line_id);
    void FadeInit(f32 duration, ERommFadeType fade_type, u8 red, u8 green, u8 blue, f32 a6);
    int FadeUpdate(f32);
    int FadeIn(f32 a1);
    int FadeOut(f32 a1);
    void RenderFade();
    void RegisterVideo(int, std::string filename);
    CDKW_RGBA ComputeGameFadeColor();
};
REQUIRE_SIZE(CGame, 0x5098);

#endif
