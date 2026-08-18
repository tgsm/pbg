#ifndef PIGLET_CMINIMAP_H
#define PIGLET_CMINIMAP_H

#include <string>
#include "engine/display/CCamera.h"
#include "engine/display/CClump.h"
#include "engine/display/CIm2DBatch.h"
#include "engine/display/CScene.h"
#include "engine/input/CInput.h"
#include "engine/sound/CSound2D.h"
#include "engine/xmd/CChunkIterator.h"
#include "CMiniMapIcon.h"
#include "CMiniMapSpline.h"

class CGame;

// TODO
class CMiniMap {
public:
    CGame* m_game;
    std::string m_name;
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    int m_cam_raster_height;
    int m_cam_raster_width;
    CMiniMapSplineMgr* m_spline_manager;
    BOOL m_visible;
    DKDSP::CClump* m_clump20;
    DKI::CInput* m_select_input;
    DKI::CInput* m_axeh_input;
    DKI::CInput* m_axev_input;
    U8 m_unk30[0x38 - 0x30];
    CMiniMapIcon* m_icon38;
    CMiniMapIcon* m_hero_icon;
    CMiniMapIcon* m_icon40;
    CMiniMapIcon* m_icon44;
    DKDSP::CIm2DBatch* m_batch48;
    DKDSP::CIm2DBatch* m_batch4C;
    CDKW_V3d m_unk50;
    CDKW_V3d m_unk5C;
    F32 m_unk68;
    F32 m_unk6C;
    U8 m_unk70[0x78 - 0x70];
    U32 m_unk78;
    F32 m_unk7C;
    F32 m_unk80;
    CDKW_V3d m_unk84;
    F32 m_unk90;
    F32 m_unk94;
    BOOL m_unk98;
    U32 m_unk9C;
    U32 m_unkA0;
    U32 m_unkA4;
    U32 m_unkA8;
    DKSND::CSound2D* m_unkAC;
    F32 m_unkB0;

    static char* MINIMAP_TEX_FRIEND_NPC[];

public:
    CMiniMap(CGame* game);
    ~CMiniMap();

    BOOL Create();
    void Update(F32 dt);
    void Show();
    U32 UnkRoomNPCMaybeThing(int);
    void ShowFriendNPC(int, int, int);
    void HideFriendNPC(int);
    void Render(F32 dt);
    void RenderDarkenRooms(F32 dt);
    void RenderHeroIcon(F32 dt);
    void RenderIcons(F32 dt);
    void RenderCookiesNbIcon(F32 dt);
    BOOL IsVisible();
    void ComputeMinimapBBox();
    BOOL Load(DkXmd::CChunkIterator*);
    BOOL Unload();
};
REQUIRE_SIZE(CMiniMap, 0xB4);

#endif
