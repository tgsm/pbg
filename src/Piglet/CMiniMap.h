#ifndef PIGLET_CMINIMAP_H
#define PIGLET_CMINIMAP_H

#include <string>
#include "engine/display/CCamera.h"
#include "engine/display/CClump.h"
#include "engine/display/CIm2DBatch.h"
#include "engine/display/CScene.h"
#include "engine/input/CInput.h"
#include "engine/xmd/CChunkIterator.h"
#include "CMiniMapIcon.h"
#include "CMiniMapSpline.h"

class CGame;

// TODO
class CMiniMap {
public:
    CGame* m_game;
    std::string m_unk4;
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    int m_cam_raster_height;
    int m_cam_raster_width;
    CMiniMapSplineMgr* m_spline_manager;
    BOOL m_visible;
    DKDSP::CClump* m_clump20;
    DKI::CInput* m_select_input;
    DKI::CInput* m_axeh_input;
    DKI::CInput* m_axevinput;
    U8 m_unk30[0x38 - 0x30];
    CMiniMapIcon* m_icon38;
    CMiniMapIcon* m_icon3C;
    CMiniMapIcon* m_icon40;
    CMiniMapIcon* m_icon44;
    DKDSP::CIm2DBatch* m_batch48;
    DKDSP::CIm2DBatch* m_batch4C;
    F32 m_unk50;
    F32 m_unk54;
    F32 m_unk58;
    F32 m_unk5C;
    F32 m_unk60;
    F32 m_unk64;
    U8 m_unk68[0x78 - 0x68];
    U32 m_unk78;
    F32 m_unk7C;
    F32 m_unk80;
    U8 m_unk84[0x90 - 0x84];
    F32 m_unk90;
    F32 m_unk94;
    U8 m_unk98[0x9C - 0x98];
    U32 m_unk9C;
    U32 m_unkA0;
    U32 m_unkA4;
    U32 m_unkA8;
    U32 m_unkAC;
    F32 m_unkB0;

public:
    CMiniMap(CGame* game);
    ~CMiniMap();

    BOOL Create();
    void ShowFriendNPC(int, int, int);
    void HideFriendNPC(int);
    void RenderDarkenRooms(F32 dt);
    void RenderHeroIcon(F32 dt);
    void RenderCookiesNbIcon(F32 dt);
    BOOL IsVisible();
    BOOL Load(DkXmd::CChunkIterator*);
    BOOL Unload();
};
REQUIRE_SIZE(CMiniMap, 0xB4);

#endif
