#pragma once on

#include <string>
#include "engine/backup/CGCNBAKEngine.h"
#include "engine/display/CEngine.h"
#include "engine/display/CTextureDictionary.h"
#include "engine/display/CTimer.h"
#include "engine/sound/CSoundEngine.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CFullScreenEffect.h"
#include "CGamePart.h"
#include "CGuiManager.h"
#include "CMailBox.h"
#include "CMiniMap.h"
#include "CMission.h"
#include "CResourceFactory.h"

class CEntityManager;
class CShadowZone;

// Very TODO, there's a lot of stuff in this class
class CGame {
public:
    enum ERommFadeType {
        FADE_TYPE_3 = 3,
        FADE_TYPE_4 = 4,
    };

    enum ERoomReturnType {
        RETURN_TYPE_0,
    };

public:
    u8 m_unk0[0x210];
    CMission m_unk210[8];
    CMission m_unk28B0[8];
    f32 m_delta_time;
    u32 m_unk4F54;
    u32 m_unk4F58;
    u32 m_unk4F5C;
    u8 m_unk4F60[0x4F80 - 0x4F60];
    DKSND::CSoundEngine* m_sound_engine;
    u8 m_unk4F84[4];
    DKDSP::CEngine* m_display_engine;
    DKDSP::CTextureDictionary* m_texture_dictionary;
    u8 m_unk4F90[0x4FA0 - 0x4F90];
    CEntityManager* m_entity_manager;
    CMiniMap* m_minimap;
    CResourceFactory* m_resource_factory;
    DKDSP::CTimer* m_timer;
    CMailBox* m_mailbox;
    u8 m_unk4FB4[0x4FDC - 0x4FB4];
    CGuiManager* m_gui_manager;
    u8 m_unk4FE0[4];
    CShadowZone* m_shadow_zone;
    u8 m_unk4FE8[4];
    DKBAK::CGCNBAKEngine* m_backup_engine;
    u8 m_unk4FF0[0x5000 - 0x4FF0];
    CScreenEffect* m_screen_effect;
    u8 m_unk5004[0x500C - 0x5004];
    CGamePart* m_game_part;
    u8 m_unk5010[0x502C - 0x5010];
    f32 m_unk502C;
    f32 m_unk5030;
    u8 m_unk5034[4];
    u32 m_unk5038;
    u8 m_unk503C;
    u8 m_unk503D[0x5048 - 0x503D];
    f32 m_unk5048;
    u8 m_unk504C[0x5094 - 0x504C];

public:
    CGame(void*, u32);
    virtual ~CGame();

    virtual BOOL NextFrame();

    CMailBox* GetMailbox() { return m_mailbox; }

    void PlayNarratorLine(std::string line_id);
    void ResetOpcodeBuffer();
    void PushOpcodeValue(int opcode);
    CDKW_RGBA ComputeGameFadeColor();
    void FadeInit(f32 a1, ERommFadeType fade_type, u8 red, u8 green, u8 blue, f32 a6);
    void FadeIn(f32 a1);
    void SetCurrentRoomReturnType(ERoomReturnType return_type, int a2);
    void ComputeDeltaTime();
    f32 GetDeltaTime();
    CGamePart* GetGamePartPointer();
};
REQUIRE_SIZE(CGame, 0x5098);
