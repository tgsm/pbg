#ifndef PIGLET_CGAMEPARTINGAME_H
#define PIGLET_CGAMEPARTINGAME_H

#include "CGamePart.h"
#include "CGameRoomManager.h"

// TODO: Set up this class
class CGamePartIngame : public CGamePart {
public:
    U8 m_unkC[8];
    CGameRoomManager* m_game_room_manager;
    U8 m_unk18[0x20];

public:
    BOOL StartFightMode(CEntityMesh*, int);
    BOOL UnblockFightMode();
};
REQUIRE_SIZE(CGamePartIngame, 0x38);

#endif
