#ifndef ENGINE_DISPLAY_CCAMERA_H
#define ENGINE_DISPLAY_CCAMERA_H

#include "engine/display/ICamera.h"

namespace DKDSP {

struct UnkCameraSubstruct {
    U8 unk0;
    RwCamera* rw_camera;
};

class CCamera : public ICamera {
public:
    UnkCameraSubstruct* m_unk8;
    U8 m_unkC[0x28 - 0xC];

public:
    virtual void SetProjection(RwCameraProjection projection);
    virtual RwCameraProjection GetProjection();
    virtual void SetBuffer(IRaster* buffer);
    virtual CRaster* GetBuffer();
    virtual void SetZBuffer(IRaster* z_buffer);
    virtual CRaster* GetZBuffer();
    virtual void GetSettingsFromCamera(ICamera* camera);
    virtual void SetViewWindow(F32, F32);
    virtual F32 GetViewWindowHalfWidth();
    virtual F32 GetViewWindowHalfHeight();
    virtual void SetViewOffset(F32, F32);
    virtual F32 GetViewXOffset();
    virtual F32 GetViewYOffset();
    virtual RwMatrix* GetViewMatrix();
    virtual void SetZNear(F32 near);
    virtual F32 GetZNear();
    virtual void SetZFar(F32 far);
    virtual F32 GetZFar();
    virtual void SetFogDistance(F32 distance);
    virtual F32 GetFogDistance();
    virtual void SetFOV(F32, F32);
    virtual F32 GetFOV();
    virtual F32 GetAspectRatio();
    virtual void SetFrustum(SDKW_Frustum frustum);
    virtual SDKW_Frustum GetFrustum();
    virtual void Flush();
    virtual void BuildRay(F32, F32, CDKW_V3d&, CDKW_V3d&);
};
REQUIRE_SIZE(CCamera, 0x28);

}

#endif
