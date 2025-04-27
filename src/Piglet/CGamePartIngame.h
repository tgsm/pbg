#pragma once on

#include "CGamePart.h"
#include "CGameRoomManager.h"

// TODO: Set up this class
class CGamePartIngame : public CGamePart {
public:
    u8 m_unkC[8];
    CGameRoomManager* m_game_room_manager;
    u8 m_unk18[0x20];

public:
    BOOL StartFightMode(CEntityMesh*, int);
    BOOL UnblockFightMode();
};
REQUIRE_SIZE(CGamePartIngame, 0x38);
