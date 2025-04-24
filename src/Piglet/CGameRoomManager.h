#pragma once on

#include "types.h"

class CGameRoomManager {
public:
    u8 m_unk0[0x44];
    u32 m_unk44;
    u8 m_unk48[0x178 - 0x48];

public:
    BOOL IsPlayingRTC();
    void InitTimer(f32 duration);
    void UpdateTimer(f32 dt);
    void StopTimer();
    void DisplayTimer();
};
REQUIRE_SIZE(CGameRoomManager, 0x178);
