#pragma once on

#include <string>
#include "engine/DKW_RGBA.h"
#include "CGamePart.h"
#include "CMailBox.h"

// Very TODO, there's a lot of stuff in this class
// Size: 0x5098
class CGame {
public:
    enum ERommFadeType {
        FADE_TYPE_4 = 4,
    };

    enum ERoomReturnType {
        RETURN_TYPE_0,
    };

public:
    u8 m_unk0[0x4F5C];
    u32 m_unk4F5C;
    u8 m_unk4F60[0x4FB0 - 0x4F60];
    CMailBox* m_mailbox;
    u8 m_unk4FB4[0x502C - 0x4FB4];
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
    CGamePart* GetGamePartPointer();
};
