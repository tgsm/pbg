#include <cstring>
#include <new>
#include <rwsdk/badevice.h>
#include "engine/display/CCamera.h"
#include "engine/wrap/DKW_Matrix.h"
#include "engine/wrap/DKW_V2d.h"

namespace DKDSP {

CCamera::~CCamera() {
    if (m_wrap_camera != NULL && m_unkC == TRUE) {
        delete m_wrap_camera;
    }
    m_wrap_camera = NULL;
    m_unkC = TRUE;
}

void CCamera::Construct(CDKW_Camera* wrap_camera) {
    m_wrap_camera = wrap_camera;
    if (m_wrap_camera == NULL) {
        CDKW_Camera::ms_NbObject++;
        CDKW_Camera* camera_ptr = (CDKW_Camera*)RwEngineInstance->unk140(CDKW_Camera::ms_pMemEntry);
        m_wrap_camera = new (camera_ptr) CDKW_Camera();
    }
    m_buffer.SetDkWrapRaster((CDKW_Raster*)m_wrap_camera->m_rw_camera->frameBuffer, 0);
    if (m_wrap_camera->m_rw_camera->zBuffer != NULL) {
        m_z_buffer.SetDkWrapRaster((CDKW_Raster*)m_wrap_camera->m_rw_camera->zBuffer, 0);
    }

    m_unkC = TRUE;
}

void CCamera::SetProjection(RwCameraProjection projection) {
    if (m_wrap_camera != NULL) {
        RwCameraSetProjection(m_wrap_camera->m_rw_camera, projection);
    }
}

RwCameraProjection CCamera::GetProjection() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->projectionType;
    }

    return rwPERSPECTIVE;
}

void CCamera::SetBuffer(IRaster* buffer_) {
    CRaster* buffer = (CRaster*)buffer_;
    if (m_wrap_camera != NULL) {
        if (buffer == NULL) {
            m_wrap_camera->m_rw_camera->frameBuffer = NULL;
            m_buffer.SetDkWrapRaster(NULL, 1);
        } else {
            m_wrap_camera->m_rw_camera->frameBuffer = buffer->m_wrap_raster;
            m_buffer.SetDkWrapRaster(buffer->m_wrap_raster, 1);
        }
    }
}

CRaster* CCamera::GetBuffer() {
    return &m_buffer;
}

void CCamera::SetZBuffer(IRaster* z_buffer_) {
    CRaster* z_buffer = (CRaster*)z_buffer_;
    if (m_wrap_camera != NULL) {
        if (z_buffer == NULL) {
            m_wrap_camera->m_rw_camera->zBuffer = NULL;
            m_z_buffer.SetDkWrapRaster(NULL, 1);
        } else {
            m_wrap_camera->m_rw_camera->zBuffer = z_buffer->m_wrap_raster;
            m_z_buffer.SetDkWrapRaster(z_buffer->m_wrap_raster, 1);
        }
    }
}

CRaster* CCamera::GetZBuffer() {
    return &m_z_buffer;
}

void CCamera::GetSettingsFromCamera(ICamera* camera) {
    SetViewWindow(camera->GetViewWindowHalfWidth(), camera->GetViewWindowHalfHeight());
    SetViewOffset(camera->GetViewXOffset(), camera->GetViewYOffset());
    SetZNear(camera->GetZNear());
    SetZFar(camera->GetZFar());
    SetProjection(camera->GetProjection());
    RwMatrix* camera_model = &camera->GetFrame()->m_rwframe->modelling;
    RwFrameTransform(GetFrame()->m_rwframe, camera_model, 0);
}

void CCamera::SetViewWindow(F32 a1, F32 a2) {
    CDKW_V2d view_window;
    view_window.x = a1;
    view_window.y = a2;
    if (m_wrap_camera != NULL) {
        // FIXME: Make CDKW_V2d inherit RwV2d
        RwCameraSetViewWindow(m_wrap_camera->m_rw_camera, (RwV2d*)&view_window);
    }
}

F32 CCamera::GetViewWindowHalfWidth() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->viewWindow.x;
    }

    return 0.0f;
}

F32 CCamera::GetViewWindowHalfHeight() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->viewWindow.y;
    }

    return 0.0f;
}

void CCamera::SetViewOffset(F32 a1, F32 a2) {
    CDKW_V2d view_offset;
    view_offset.x = a1;
    view_offset.y = a2;
    if (m_wrap_camera != NULL) {
        // FIXME: Make CDKW_V2d inherit RwV2d
        RwCameraSetViewOffset(m_wrap_camera->m_rw_camera, (RwV2d*)&view_offset);
    }
}

F32 CCamera::GetViewXOffset() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->viewOffset.x;
    }

    return 0.0f;
}

F32 CCamera::GetViewYOffset() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->viewOffset.y;
    }

    return 0.0f;
}

void CCamera::SetZNear(F32 near) {
    if (m_wrap_camera != NULL && near > 0.0f) {
        RwCameraSetNearClipPlane(near, m_wrap_camera->m_rw_camera);
    }
}

F32 CCamera::GetZNear() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->nearPlane;
    }

    return 0.0f;
}

void CCamera::SetZFar(F32 far) {
    if (m_wrap_camera != NULL && far > 0.0f) {
        RwCameraSetFarClipPlane(far, m_wrap_camera->m_rw_camera);
    }
}

F32 CCamera::GetZFar() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->farPlane;
    }

    return 0.0f;
}

void CCamera::SetFogDistance(F32 fog_distance) {
    if (m_wrap_camera != NULL) {
        m_wrap_camera->m_rw_camera->fogPlane = fog_distance;
    }
}

F32 CCamera::GetFogDistance() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->m_rw_camera->fogPlane;
    }

    return 0.0f;
}

void CCamera::SetFOV(F32 a1, F32 a2) {
    if (m_wrap_camera != NULL) {
        m_wrap_camera->SetFOV(a1, a2);
    }
}

F32 CCamera::GetFOV() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->GetFOV();
    }

    return 0.0f;
}

F32 CCamera::GetAspectRatio() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->GetAspectRatio();
    }

    return 0.0f;
}

void CCamera::SetFrustum(SDKW_Frustum frustum) {
    if (m_wrap_camera != NULL) {
        m_wrap_camera->SetFrustum(frustum);
    }
}

SDKW_Frustum CCamera::GetFrustum() {
    if (m_wrap_camera != NULL) {
        return m_wrap_camera->GetFrustum();
    }

    SDKW_Frustum frustum;
    memset(&frustum, 0, sizeof(SDKW_Frustum));
    return frustum;
}

void CCamera::Flush() {
    if (m_wrap_camera != NULL) {
        m_wrap_camera->FlushAlphaAtomic();
    }
}

void CCamera::BuildRay(F32 a1, F32 a2, CDKW_V3d& a3, CDKW_V3d& a4) {
    if (GetProjection() == rwPARALLEL) {
        a3 = CDKW_V3d(GetViewXOffset() - (a1 * GetViewWindowHalfWidth()), GetViewYOffset() + (a2 * GetViewWindowHalfHeight()), 0.0f);
        a4 = CDKW_V3d(0.0f, 0.0f, 1.0f);
    } else {
        a3 = CDKW_V3d(0.0f, 0.0f, 0.0f);
        a4 = CDKW_V3d(GetViewXOffset() - (a1 * GetViewWindowHalfWidth()), GetViewYOffset() + (a2 * GetViewWindowHalfHeight()), 1.0f);
        a4.Normalize();
    }

    CDKW_Frame* frame = GetFrame();
    CDKW_Matrix matrix((CDKW_Matrix&)(frame->m_rwframe->modelling));
    a3 *= matrix;
    matrix.pos = CDKW_V3d(0.0f, 0.0f, 0.0f);
    RwMatrixUpdate(&matrix);
    a4 *= matrix;
}

void CCamera::SetFrame(CDKW_Frame* frame) {

}

}
