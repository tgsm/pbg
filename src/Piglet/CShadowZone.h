#ifndef PIGLET_CSHADOWZONE_H
#define PIGLET_CSHADOWZONE_H

#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "engine/wrap/DKW_V3d.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {
class CImage;
}

class CGame;

class CShadowZone {
private:
    CGame* m_game;
    F32 m_topleft_x;
    F32 m_topleft_y;
    F32 m_topleft_z;
    F32 m_downright_x;
    F32 m_downright_y;
    F32 m_downright_z;
    F32 m_unk1C;
    F32 m_unk20;
    DKDSP::CImage* m_image;
    U32 m_unk28;

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

#endif
