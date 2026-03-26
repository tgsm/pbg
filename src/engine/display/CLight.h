#ifndef ENGINE_DISPLAY_CLIGHT_H
#define ENGINE_DISPLAY_CLIGHT_H

#include "engine/display/ILight.h"

class CDKW_Light;

namespace DKDSP {

// TODO
class CLight : public ILight {
public:
    CDKW_Light* m_wrap_light;
    U8 m_unkC[0x24 - 0xC];

public:
    CLight(ELIGHTTYPE type);

    virtual CDKW_Frame* GetFrame();
    virtual void SetFrame(CDKW_Frame* frame);
    virtual ELIGHTTYPE GetLightType();
    virtual void func14();
    virtual BOOL IsEnabled();
    virtual void GetSettingsFromLight(ILight* light);
    virtual void SetColor(F32 red, F32 green, F32 blue);
    virtual F32 GetColorRed();
    virtual F32 GetColorGreen();
    virtual F32 GetColorBlue();
    virtual void Affect(U32);
    virtual U32 GetAffected();
    virtual void SetRadius(F32 radius);
    virtual F32 GetRadius();
    virtual void SetConeAngle(F32 angle);
    virtual F32 GetConeAngle();

    void RenderLightDirectionnal();
    void RenderLightOmni();
    void RenderLightSpot();
    void SetEnabled(BOOL enable);
};
REQUIRE_SIZE(CLight, 0x24);

}

#endif
