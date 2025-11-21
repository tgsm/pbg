#ifndef PIGLET_CGAMEPARTINGAME_H
#define PIGLET_CGAMEPARTINGAME_H

#include "CGamePart.h"
#include "CCACamera.h"
#include "CGameRoomManager.h"
#include "entities/CRTCPlayerEntity.h"

class CGame;

// TODO: Set up this class
class CGamePartIngame : public CGamePart {
public:
    CGame* m_game;
    CCACamera* m_camera;
    CGameRoomManager* m_game_room_manager;
    U32 m_unk18;
    U8 m_unk1C[0x1C];

public:
    CGamePartIngame(CGame* game);
    virtual ~CGamePartIngame();

    virtual U32 NextFrame();

    BOOL StartFightMode(CEntityMesh*, int);
    BOOL InterruptFightMode();
    BOOL UnblockFightMode();
    BOOL HeroAskForFightModeLaunch();
    void StartRTCMode(CRTCPlayerEntity* player, int);
    BOOL IsAllCookiesCollected();
    void SetupSoundsLayer(U32, U32);

    CGameRoomManager* GetGameRoomManager() { return m_game_room_manager; }
};
REQUIRE_SIZE(CGamePartIngame, 0x38);

#endif
