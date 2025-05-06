#pragma once on

#include "types.h"
#include "entities/CEntityHero.h"

class CGameRoomManager {
public:
    u32 m_unk0;
    u8 m_unk4[0x40];
    u32 m_unk44;
    u8 m_unk48[0x178 - 0x48];

public:
    BOOL IsPlayingRTC();
    void InitTimer(f32 duration);
    void UpdateTimer(f32 dt);
    void StopTimer();
    void DisplayTimer();

    CEntityHero* GetCurrentHero();
};
REQUIRE_SIZE(CGameRoomManager, 0x178);
