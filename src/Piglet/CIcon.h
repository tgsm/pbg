#ifndef PIGLET_CICON_H
#define PIGLET_CICON_H

#include "engine/display/CCamera.h"
#include "engine/display/CEngine.h"
#include "engine/display/CImmediate.h"
#include "engine/display/CScene.h"
#include "engine/display/CTexture.h"

// TODO
class CIcon {
public:
    DKDSP::CTexture* m_texture;
    F32 m_unk4;
    F32 m_x;
    F32 m_y;
    F32 m_depth;
    F32 m_width;
    F32 m_height;
    F32 m_u1;
    F32 m_v1;
    F32 m_u2;
    F32 m_v2;
    U8 m_red;
    U8 m_green;
    U8 m_blue;
    U8 m_alpha;

    static DKDSP::CCamera* ms_pCamera;
    static DKDSP::CEngine* ms_pEngine;
    static DKDSP::CScene* ms_pScene;
    // Unknown types
    static void* m_backupVertexAlpha;
    static void* m_backupZWrite;
    static void* m_backupZTest;
    static void* m_backupSrcBlend;
    static void* m_backupDestBlend;
    static void* m_backupCullMode;

public:
    CIcon();
    virtual ~CIcon();

    static void BeginRender(DKDSP::IEngine* engine, DKDSP::ICamera* camera, DKDSP::IScene* scene);
    void Render(DKDSP::IIm2DBatch* batch, int);
    static void EndRender();
};
REQUIRE_SIZE(CIcon, 0x34);

#endif
