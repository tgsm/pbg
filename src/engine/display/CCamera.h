#ifndef ENGINE_DISPLAY_CCAMERA_H
#define ENGINE_DISPLAY_CCAMERA_H

#include "engine/display/ICamera.h"

struct RwCamera;

namespace DKDSP {

struct UnkCameraSubstruct {
    u8 unk0;
    RwCamera* rw_camera;
};

class CCamera : public ICamera {
public:
    UnkCameraSubstruct* m_unk8;
    u8 m_unkC[0x28 - 0xC];

public:
    virtual void SetBuffer(IRaster*);
    virtual IRaster* GetBuffer();
    virtual void SetViewWindow(f32, f32);
};
REQUIRE_SIZE(CCamera, 0x28);

}

#endif
