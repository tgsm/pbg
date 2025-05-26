#ifndef PIGLET_CGAMEROOMMANAGER_H
#define PIGLET_CGAMEROOMMANAGER_H

#include "types.h"
#include "entities/CEntityHero.h"
#include "entities/CEntitySeqKey.h"

class CGameRoomManager {
public:
    u32 m_unk0;
    u8 m_unk4[0x40];
    u32 m_unk44;
    u8 m_unk48[0x50 - 0x48];
    CEntitySeqKey* m_key_sequence_entity;
    u8 m_unk54[0x178 - 0x54];

public:
    BOOL IsPlayingRTC();
    void InitTimer(f32 duration);
    void UpdateTimer(f32 dt);
    void StopTimer();
    void DisplayTimer();

    CEntityHero* GetCurrentHero();
    f32 GetCamRollAngle();
    void SetCamRollAngle(f32 angle);
};
REQUIRE_SIZE(CGameRoomManager, 0x178);

#endif
