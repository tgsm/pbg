#ifndef PIGLET_CGAMEPARTINGAME_H
#define PIGLET_CGAMEPARTINGAME_H

#include "CGamePart.h"
#include "CCACamera.h"
#include "CGameRoomManager.h"
#include "CPSplines.h"
#include "entities/CRTCPlayerEntity.h"

class CGame;

// TODO: Set up this class
class CGamePartIngame : public CGamePart {
public:
    CGame* m_game;
    CCACamera* m_camera;
    CGameRoomManager* m_game_room_manager;
    U32 m_unk18;
    CPigSplineBank* m_pig_spline_bank;
    F32 m_second_pass_timer;
    CDKW_V3d m_unk24;
    U8 m_unk30[8];

public:
    CGamePartIngame(CGame* game);
    virtual ~CGamePartIngame();

    virtual U32 NextFrame();

    U32 UnknownRoomCheck();
    BOOL LoadCollisionBSP(DkXmd::CChunkIterator iter);
    BOOL LoadCameraSystem(DkXmd::CChunkIterator iter);
    BOOL LoadSplines(DkXmd::CChunkIterator iter);
    BOOL LoadPathNodes(DkXmd::CXmdFile* xmd, DkXmd::CChunkIterator iter);
    BOOL Load();
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
