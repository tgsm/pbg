#ifndef ENGINE_DISPLAY_ISHADOWMAPVALIDATIONCALLBACK_H
#define ENGINE_DISPLAY_ISHADOWMAPVALIDATIONCALLBACK_H

#include "engine/display/ICamera.h"

class CDKW_Atomic;
class CDKW_Sphere;

namespace DKDSP {

class IShadowMap;

class IShadowMapValidationCallback {
public:
    IShadowMapValidationCallback();

    virtual BOOL ValidateDisplay(IShadowMap*, ICamera*, CDKW_Atomic*, CDKW_Sphere*) = 0;
    virtual BOOL ValidateObjectDisplay(CDKW_Atomic*) = 0;
};

}

#endif
