#ifndef ENGINE_DISPLAY_CSHADOWMAP_H
#define ENGINE_DISPLAY_CSHADOWMAP_H

#include <vector>
#include "engine/display/CRaster.h"
#include "engine/display/IShadowMap.h"

namespace DKDSP {

class CEngine;
class CScene;
class ILight;

class CShadowMap : public IShadowMap {
public:
    U32 m_unk4;
    U32 m_unk8;
    CRaster m_raster;
    U32 m_unk18;
    U32 m_unk1C;
    U32 m_unk20;
    U32 m_unk24;
    U32 m_unk28;
    BOOL m_enabled;
    BOOL m_display_enabled;
    F32 m_unk34;
    F32 m_unk38;
    F32 m_unk3C;
    std::vector<void*> m_unk40; // TODO: Figure out these types
    std::vector<void*> m_unk4C;
    std::vector<void*> m_unk58;
    F32 m_unk64;
    U32 m_unk68;
    U32 m_unk6C;
    U32 m_unk70;

public:
    CShadowMap() {
        m_unk4 = 0;
        m_unk8 = 0;
        m_unk1C = 0;
        m_unk20 = 0;
        m_unk24 = 0;
        m_unk28 = 0;
        m_enabled = TRUE;
        m_display_enabled = TRUE;
        m_unk18 = 0;
        m_unk34 = 0.0f;
        m_unk38 = 0.0f;
        m_unk3C = 0.0f;
        m_unk64 = 0.0f;
        m_unk68 = 0;
        m_unk6C = 0;
        m_unk70 = 0;
    }

    virtual void AddShadowEmitter(IClump* shadow_emitter);
    virtual BOOL EnableDisplay(BOOL enable);
    virtual BOOL IsDisplayEnabled();
    virtual BOOL IsEnabled();
    virtual void SetShadowColor(F32 red, F32 green, F32 blue);
    virtual F32 GetShadowColorRed();
    virtual F32 GetShadowColorGreen();
    virtual F32 GetShadowColorBlue();
    virtual void SetBlurLevel(U32);
    virtual void Prepare();
    virtual void Unprepare();
    virtual CCamera* GetCamera();

    void Init(CEngine* engine, CScene* scene, ILight* light, U32, U32, U32);
    void SetEnabled(BOOL enable);
    void Render();
};
REQUIRE_SIZE(CShadowMap, 0x74);

}

#endif
