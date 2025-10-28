#ifndef PIGLET_CGAMEPARTINGAME_H
#define PIGLET_CGAMEPARTINGAME_H

#include "CGamePart.h"
#include "CCACamera.h"
#include "CGameRoomManager.h"

class CGame;

// TODO: Set up this class
class CGamePartIngame : public CGamePart {
public:
    U8 m_unkC[4];
    CCACamera* m_camera;
    CGameRoomManager* m_game_room_manager;
    U8 m_unk18[0x20];

public:
    CGamePartIngame(CGame* game);
    virtual ~CGamePartIngame();

    virtual U32 NextFrame();

    BOOL StartFightMode(CEntityMesh*, int);
    BOOL InterruptFightMode();
    BOOL UnblockFightMode();

    CGameRoomManager* GetGameRoomManager() { return m_game_room_manager; }
};
REQUIRE_SIZE(CGamePartIngame, 0x38);

#endif
