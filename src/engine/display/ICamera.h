#ifndef ENGINE_DISPLAY_ICAMERA_H
#define ENGINE_DISPLAY_ICAMERA_H

#include <rwsdk/bacamera.h>
#include "engine/display/IFramable.h"
#include "engine/display/CRaster.h"
#include "engine/wrap/DKW_V3d.h"

namespace DKDSP {

// TODO
struct SDKW_Frustum {

};

class ICamera : public IFramable {
public:
    ICamera();

    virtual void SetProjection(RwCameraProjection projection) = 0;
    virtual RwCameraProjection GetProjection() = 0;
    virtual void SetBuffer(IRaster* buffer) = 0;
    virtual CRaster* GetBuffer() = 0;
    virtual void SetZBuffer(IRaster* z_buffer) = 0;
    virtual CRaster* GetZBuffer() = 0;
    virtual void GetSettingsFromCamera(ICamera* camera) = 0;
    virtual void SetViewWindow(F32, F32) = 0;
    virtual F32 GetViewWindowHalfWidth() = 0;
    virtual F32 GetViewWindowHalfHeight() = 0;
    virtual void SetViewOffset(F32, F32) = 0;
    virtual F32 GetViewXOffset() = 0;
    virtual F32 GetViewYOffset() = 0;
    virtual RwMatrix* GetViewMatrix() = 0;
    virtual void SetZNear(F32 near) = 0;
    virtual F32 GetZNear() = 0;
    virtual void SetZFar(F32 far) = 0;
    virtual F32 GetZFar() = 0;
    virtual void SetFogDistance(F32 distance) = 0;
    virtual F32 GetFogDistance() = 0;
    virtual void SetFOV(F32, F32) = 0;
    virtual F32 GetFOV() = 0;
    virtual F32 GetAspectRatio() = 0;
    virtual void SetFrustum(SDKW_Frustum frustum) = 0;
    virtual SDKW_Frustum GetFrustum() = 0;
    virtual void Flush() = 0;
    virtual void BuildRay(F32, F32, CDKW_V3d&, CDKW_V3d&) = 0;
};

}

#endif
