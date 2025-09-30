#ifndef PIGLET_CMINIMAPICON_H
#define PIGLET_CMINIMAPICON_H

#include "engine/display/CIm3DBatch.h"
#include "engine/display/CScene.h"
#include "engine/display/CTexture.h"
#include "engine/wrap/DKW_V3d.h"

class CMiniMapIcon {
public:
    CMiniMapIcon(const CDKW_V3d& position, F32 width, F32 height, F32);
    ~CMiniMapIcon();

    BOOL Create(F32);
    void SetPosition(const CDKW_V3d& position);
    void SetSize(F32 size);
    void SetRGBA(U32 r, U32 g, U32 b, U32 a);
    void SetTexture(DKDSP::ITexture* texture);
    void Render(DKDSP::IScene* scene);

public:
    DKDSP::CIm3DBatch* m_batch;
    F32 m_x;
    F32 m_y;
    F32 m_z;
    F32 m_width;
    F32 m_height;
    U32 m_red;
    U32 m_green;
    U32 m_blue;
    RwMatrix* m_rw_matrix;
    DKDSP::CTexture* m_texture;
};
REQUIRE_SIZE(CMiniMapIcon, 0x2C);

#endif
