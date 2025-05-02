#pragma once on

#include <string>
#include "engine/display/CTimer.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CGamePart.h"
#include "CMailBox.h"

class CEntityManager;

// Very TODO, there's a lot of stuff in this class
class CGame {
public:
    enum ERommFadeType {
        FADE_TYPE_4 = 4,
    };

    enum ERoomReturnType {
        RETURN_TYPE_0,
    };

public:
    u8 m_unk0[0x4F50];
    f32 m_delta_time;
    u8 m_unk4F54[0x4F5C - 0x4F54];
    u32 m_unk4F5C;
    u8 m_unk4F60[0x4FA0 - 0x4F60];
    CEntityManager* m_entity_manager;
    u8 m_unk4FA4[0x4FAC - 0x4FA4];
    DKDSP::CTimer* m_timer;
    CMailBox* m_mailbox;
    u8 m_unk4FB4[0x500C - 0x4FB4];
    CGamePart* m_game_part;
    u8 m_unk5010[0x502C - 0x5010];
    f32 m_unk502C;
    f32 m_unk5030;
    u8 m_unk5034[0x503C - 0x5034];
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
