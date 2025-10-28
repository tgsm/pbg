#ifndef PIGLET_CGAMEROOMMANAGER_H
#define PIGLET_CGAMEROOMMANAGER_H

#include "entities/CEntityHero.h"
#include "entities/CEntityNPC.h"
#include "entities/CEntitySeqKey.h"
#include "entities/CRTCPlayerEntity.h"

class CGameRoomManager {
public:
    U32 m_unk0;
    U8 m_unk4[0x8];
    U32 m_unkC;
    U8 m_unk10[0x34];
    U32 m_unk44;
    U8 m_unk48[4];
    CEntityNPC* m_unk4C;
    CEntitySeqKey* m_key_sequence_entity;
    U8 m_unk54[0x118 - 0x54];
    F32 m_unk118;
    F32 m_unk11C;
    U8 m_unk120[0x154 - 0x120];
    U32 m_unk154; // color?
    U8 m_unk158[0x16C - 0x158];
    CRTCPlayerEntity* m_player_entity;
    U8 m_unk170[0x178 - 0x170];

public:
    void AddFlag(U32 flag);
    void RemFlag(U32 flag);
    U32 GetFlag();
    BOOL IsPlayingRTC();
    BOOL IsOnFight();
    void InitTimer(F32 duration);
    void UpdateTimer(F32 dt);
    void StopTimer();
    void DisplayTimer();

    CEntityHero* GetCurrentHero();
    void SetEntitiesOnFight(CEntityMesh*);
    F32 GetCamRollAngle();
    void SetCamRollAngle(F32 angle);

    U32 GetState();

    BOOL UnkInlineFor4C() {
        return (m_unk4C != NULL && m_unk4C->GetGenericBehaviour() != GENERIC_BEHAVIOUR_6) ? TRUE : FALSE;
    }
};
REQUIRE_SIZE(CGameRoomManager, 0x178);

#endif
