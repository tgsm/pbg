#ifndef ENGINE_DISPLAY_ILIGHT_H
#define ENGINE_DISPLAY_ILIGHT_H

#include "engine/display/IFramable.h"

namespace DKDSP {

enum ELIGHTTYPE {
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT,
    LIGHT_TYPE_SOFTSPOT,
};

// TODO
class ILight : public IFramable {
public:
    virtual ELIGHTTYPE GetLightType() = 0;
    virtual void func14() = 0;
    virtual void func18() = 0;
    virtual void func1C() = 0;
    virtual void SetColor(F32 red, F32 green, F32 blue) = 0;
    virtual F32 GetColorRed() = 0;
    virtual F32 GetColorGreen() = 0;
    virtual F32 GetColorBlue() = 0;
    virtual void Affect(U32) = 0;
    virtual U32 GetAffected() = 0;
    virtual void SetRadius(F32 radius) = 0;
    virtual F32 GetRadius() = 0;
    virtual void SetConeAngle(F32 angle) = 0;
    virtual F32 GetConeAngle() = 0;
};

}

#endif
