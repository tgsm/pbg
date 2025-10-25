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
    U8 m_unk4[4];
    F32 m_unk8;
    F32 m_unkC;
    F32 m_unk10;
    F32 m_unk14;
    F32 m_unk18;
    U8 m_unk1C[0x13];
    U8 m_alpha;

public:
    CIcon();
    virtual ~CIcon();

    static void BeginRender(DKDSP::IEngine* engine, DKDSP::ICamera* camera, DKDSP::IScene* scene);
    void Render(DKDSP::IIm2DBatch* batch, int);
    static void EndRender();
};
REQUIRE_SIZE(CIcon, 0x34);

#endif
