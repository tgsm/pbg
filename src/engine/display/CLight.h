#ifndef ENGINE_DISPLAY_CLIGHT_H
#define ENGINE_DISPLAY_CLIGHT_H

#include "engine/display/ILight.h"

namespace DKDSP {

// TODO
class CLight : public ILight {
public:
    virtual ELIGHTTYPE GetLightType();
    virtual void SetColor(F32 red, F32 green, F32 blue);
    virtual F32 GetColorRed();
    virtual F32 GetColorGreen();
    virtual F32 GetColorBlue();
};

}

#endif
