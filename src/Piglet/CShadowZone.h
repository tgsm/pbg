#pragma once on

#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "engine/wrap/DKW_V3d.h"
#include "engine/xmd/CChunkIterator.h"
#include "CGame.h"

namespace DKDSP {
class CImage;
}

class CShadowZone {
private:
    CGame* m_game;
    f32 m_topleft_x;
    f32 m_topleft_y;
    f32 m_topleft_z;
    f32 m_downright_x;
    f32 m_downright_y;
    f32 m_downright_z;
    f32 m_unk1C;
    f32 m_unk20;
    DKDSP::CImage* m_image;
    u32 m_unk28;

public:
    CShadowZone(CGame* game);
    ~CShadowZone();

    BOOL LoadShadowZone(DkXmd::CChunkIterator* iter);
    void UnloadShadowZone();
    CDKW_RGBA GetShadowZoneRGBA(const CDKW_V3d&, int, int);
    CDKW_RGBAReal RGBToHSV(CDKW_RGBAReal);
    CDKW_RGBAReal HSVToRGB(CDKW_RGBAReal);
};
REQUIRE_SIZE(CShadowZone, 0x2C);
