#ifndef ENGINE_DISPLAY_CCAMERA_H
#define ENGINE_DISPLAY_CCAMERA_H

#include "engine/display/ICamera.h"

namespace DKDSP {

class CCamera : public ICamera {
public:
    CDKW_Camera* m_wrap_camera;
    BOOL m_unkC;
    CRaster m_buffer;
    CRaster m_z_buffer;

public:
    CCamera() {
        Construct(NULL);
    }
    ~CCamera();

    virtual RwMatrix* GetViewMatrix() { return &m_wrap_camera->m_rw_camera->viewMatrix; }

    virtual CDKW_Frame* GetFrame() {
        CDKW_Camera* wrap_camera = m_wrap_camera;
        if (wrap_camera != NULL) {
            if (m_wrap_camera->m_wrap_frame != NULL) {
                return m_wrap_camera->m_wrap_frame;
            }

            RwFrame* rw_frame = wrap_camera->GetRwFrame();
            if (rw_frame != NULL) {
                wrap_camera->m_wrap_frame = CDKW_Frame::GetInstance(rw_frame);
                return wrap_camera->m_wrap_frame;
            }

            return NULL;
        }

        return NULL;
    }

    virtual void SetFrame(CDKW_Frame* frame);
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

    void Construct(CDKW_Camera* wrap_camera);
};
REQUIRE_SIZE(CCamera, 0x28);

}

#endif
