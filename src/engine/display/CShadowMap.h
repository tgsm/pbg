#ifndef ENGINE_DISPLAY_CSHADOWMAP_H
#define ENGINE_DISPLAY_CSHADOWMAP_H

#include "engine/display/IShadowMap.h"

namespace DKDSP {

class CShadowMap : public IShadowMap {
public:
    virtual void AddShadowEmitter(IClump* shadow_emitter);
    virtual BOOL EnableDisplay(BOOL enable);
    virtual void SetShadowColor(F32 red, F32 green, F32 blue);
    virtual void SetBlurLevel(U32);
    virtual CCamera* GetCamera();
};

}

#endif
