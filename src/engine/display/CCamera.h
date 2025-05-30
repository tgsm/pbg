#ifndef ENGINE_DISPLAY_CCAMERA_H
#define ENGINE_DISPLAY_CCAMERA_H

#include "engine/display/ICamera.h"

namespace DKDSP {

class CCamera : public ICamera {
public:
    virtual void SetViewWindow(f32, f32);
};

}

#endif
