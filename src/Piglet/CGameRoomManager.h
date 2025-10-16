#ifndef PIGLET_CGAMEROOMMANAGER_H
#define PIGLET_CGAMEROOMMANAGER_H

#include "types.h"
#include "entities/CEntityHero.h"
#include "entities/CEntitySeqKey.h"

class CGameRoomManager {
public:
    U32 m_unk0;
    U8 m_unk4[0x40];
    U32 m_unk44;
    U8 m_unk48[0x50 - 0x48];
    CEntitySeqKey* m_key_sequence_entity;
    U8 m_unk54[0x118 - 0x54];
    F32 m_unk118;
    F32 m_unk11C;
    U8 m_unk120[0x178 - 0x120];

public:
    BOOL IsPlayingRTC();
    BOOL IsOnFight();
    void InitTimer(F32 duration);
    void UpdateTimer(F32 dt);
    void StopTimer();
    void DisplayTimer();

    CEntityHero* GetCurrentHero();
    F32 GetCamRollAngle();
    void SetCamRollAngle(F32 angle);

    U32 GetState();
};
REQUIRE_SIZE(CGameRoomManager, 0x178);

#endif
