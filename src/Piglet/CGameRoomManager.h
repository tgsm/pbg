#ifndef PIGLET_CGAMEROOMMANAGER_H
#define PIGLET_CGAMEROOMMANAGER_H

#include "CFullScreenEffect.h"
#include "CPSplines.h"
#include "engine/display/CIm2DBatch.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityNPC.h"
#include "entities/CEntityPathFinder.h"
#include "entities/CEntitySeqKey.h"
#include "entities/CRTCPlayerEntity.h"

class CGamePartIngame;

class CGameRoomManager {
public:
    enum ERoomState {
        ROOM_STATE_0 = 0,
        ROOM_STATE_1 = 1,
        ROOM_STATE_2 = 2,
        ROOM_STATE_3 = 3,
        ROOM_STATE_4 = 4,
        ROOM_STATE_5 = 5,
        ROOM_STATE_6 = 6,
        ROOM_STATE_9 = 9,
        ROOM_STATE_10 = 10,
        ROOM_STATE_12 = 12,
        ROOM_STATE_13 = 13,
        ROOM_STATE_14 = 14,
        ROOM_STATE_15 = 15,
    };

    U32 m_flags;
    CGamePartIngame* m_game_part;
    U32 m_unk8;
    U32 m_unkC;
    ERoomState m_room_state10;
    U32 m_unk14;
    CGame* m_game;
    F32 m_delta_time;
    CEntityHero* m_hero;
    CDKW_V3d m_camera_position;
    CDKW_V3d m_camera_target;
    U8 m_unk3C[0x40 - 0x3C];
    F32 m_unk40;
    DKDSP::CIm2DBatch* m_batch44;
    DKDSP::CIm2DBatch* m_batch48;
    CEntityNPC* m_unk4C;
    CEntitySeqKey* m_key_sequence_entity;
    CDKW_V3d m_unk54;
    CDKW_V3d m_unk60;
    CDKW_V3d m_unk6C;
    CDKW_V3d m_unk78;
    CDKW_V3d m_unk84;
    CDKW_V3d m_unk90;
    F32 m_unk9C;
    F32 m_unkA0;
    F32 m_camera_roll_angle;
    F32 m_unkA8;
    U8 m_unkAC;
    U32 m_unkB0;
    U8 m_unkB4[4];
    F32 m_fov;
    F32 m_unkBC;
    F32 m_camera_rollC0;
    CEntityPathFinder* m_pathfinder_entity;
    F32 m_unkC8;
    F32 m_unkCC;
    F32 m_unkD0;
    F32 m_unkD4;
    F32 m_unkD8;
    F32 m_unkDC;
    F32 m_unkE0;
    U8 m_unkE4[0xF8 - 0xE4];
    DKSND::ISound* m_soundF8; // which sound type?
    DKSND::ISound* m_soundFC; // which sound type?
    DKSND::ISound* m_sound100; // which sound type?
    DKSND::ISound* m_sound104; // which sound type?
    CPigSpline* m_pigspline108;
    F32 m_unk10C;
    DKDSP::CLight* m_light;
    U8 m_unk114[4];
    F32 m_unk118;
    F32 m_unk11C;
    int m_unk120;
    DKDSP::CIm2DBatch* m_batch124;
    F32 m_unk128;
    F32 m_unk12C;
    DKDSP::CIm2DBatch* m_batch130;
    DKDSP::CIm2DBatch* m_batch134;
    DKDSP::CIm2DBatch* m_batch138;
    DKDSP::CIm2DBatch* m_batch13C;
    F32 m_unk140;
    S8 m_unk144;
    // 3 padding bytes
    DKDSP::CIm2DBatch* m_batch148;
    U8 m_unk14C[0x154 - 0x14C];
    U32 m_unk154; // color?
    F32 m_timer;
    F32 m_unk15C;
    DKDSP::CClump* m_clump160;
    DKDSP::CClump* m_clump164;
    U8 m_unk168[4];
    CRTCPlayerEntity* m_player_entity;
    F32 m_unk170;
    CScreenEffect* m_screen_effect;

public:
    CGameRoomManager(CGamePartIngame* game_part, CGame* game);
    ~CGameRoomManager();

    void Init();
    void Update(F32 dt);
    void Render();
    void RenderRoomNormalFade();
    void RenderFadeWarpForFight(DKDSP::IEngine* display_engine);
    void RenderVictoryElement(DKDSP::IEngine* display_engine);
    U32 AddFlag(U32 flag);
    U32 RemFlag(U32 flag);
    U32 GetFlag();
    void SetState(ERoomState state, int);
    BOOL IsPlayingRTC();
    BOOL IsOnFight();
    void FadeInRoom();
    void OnPlayNormal();
    void OnPlayRTCInit();
    void OnPlayRTCUpdate();
    void OnPlayFightInit();
    void OnPlayFightUpdate();
    void OnPlayFightClose();
    void SecondPass();
    void Victory();
    void DisplayCatchThemAll();
    void UpdateAdventureCam();
    void UpdateFightCam(int);
    void LaunchCurrentGrimace();
    void UpdateRTCCamFight();
    void InitTimer(F32 duration);
    void UpdateTimer(F32 dt);
    void StopTimer();
    void DisplayTimer();
    void Create2ndPassVictoryCamParam(int);
    BOOL Update2ndPassVictoryCam(int);

    CEntityHero* GetCurrentHero();
    void SetEntitiesOnFight(CEntityMesh*);
    F32 GetCamRollAngle();
    void SetCamRollAngle(F32 angle);

    U32 GetState();
    BOOL CheckIfHeroIsPushing();
    BOOL CheckIfFightPossible();
    void DisplayActionKey();
    void DisplayExclamation();
    void CreateScreenEffect(DkXmd::CChunkIterator iter);
    void DisplayTicTac();
    void DisplayCookiesNbOnCatch();

    BOOL UnkInlineFor4C() {
        return (m_unk4C != NULL && m_unk4C->GetGenericBehaviour() != GENERIC_BEHAVIOUR_6) ? TRUE : FALSE;
    }

    void CountEnemies() {
        m_unk120 = 0;
        for (int i = 0; i < (int)m_game->m_entity_manager->GetEntityCount(); i++) {
            U32 type = m_game->m_entity_manager->GetEntity(i)->GetType();
            if (type > ENTITY_NPC23 && type < ENTITY_UNK36) {
                m_unk120++;
            }
        }
    }

    void dothething(DKDSP::CClump* clump, const CDKW_V3d& vec) {
        RwMatrix* model = &clump->GetFrame()->m_rwframe->modelling;
        model->pos.x = vec.m_x;
        model->pos.y = vec.m_y;
        model->pos.z = vec.m_z;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(clump->GetFrame()->m_rwframe);
    }

    void dothething(DKDSP::CClump* clump, F32 x, F32 y, F32 z) {
        RwMatrix* model = &clump->GetFrame()->m_rwframe->modelling;
        model->pos.x = x;
        model->pos.y = y;
        model->pos.z = z;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(clump->GetFrame()->m_rwframe);
    }

    void dothescalething(DKDSP::CClump* clump) {
        CDKW_V3d vec(m_unkA8, m_unkA8, m_unkA8);
        RwFrameScale(clump->GetFrame()->m_rwframe, (RwV3d*)&vec, 2);
    }
};
REQUIRE_SIZE(CGameRoomManager, 0x178);

#endif
