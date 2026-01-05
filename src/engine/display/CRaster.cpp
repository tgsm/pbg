#include <rwsdk/baimras.h>
#include "engine/display/CImage.h"
#include "engine/display/CRaster.h"

namespace DKDSP {

CRaster::CRaster() {
    m_wrap_raster = NULL;
    m_unk8 = 1;
}

CRaster::~CRaster() {
    Release();
}

void CRaster::Release() {
    if (m_wrap_raster != NULL && m_unk8 == 1 && m_wrap_raster != NULL) {
        RwRasterDestroy(m_wrap_raster);
    }
    m_wrap_raster = NULL;
    m_unk8 = 1;
}

void CRaster::SetDkWrapRaster(CDKW_Raster* wrap_raster, int a2) {
    m_wrap_raster = wrap_raster;
    m_unk8 = a2;
}

int CRaster::GetWidth() {
    return m_wrap_raster->width;
}

int CRaster::GetHeight() {
    return m_wrap_raster->height;
}

int CRaster::GetDepth() {
    return m_wrap_raster->depth;
}

int CRaster::GetFormat() {
    return m_wrap_raster->cFormat << 8;
}

int CRaster::GetNumberOfLevels() {
    return RwRasterGetNumLevels(m_wrap_raster);
}

int CRaster::GetStride() {
    return m_wrap_raster->stride;
}

BOOL CRaster::SetFromImage(IImage* image) {
    return RwRasterSetFromImage(m_wrap_raster, ((CImage*)image)->m_rw_image) != NULL ? TRUE : FALSE;
}

RwRaster* CRaster::GetParent() {
    return NULL;
}

BOOL CRaster::Render(int a1, int a2) {
    return RwRasterRender(m_wrap_raster, a1, a2) == NULL ? FALSE : TRUE;
}

BOOL CRaster::RenderFast(int a1, int a2) {
    return RwRasterRenderFast(m_wrap_raster, a1, a2) == NULL ? FALSE : TRUE;
}

BOOL CRaster::Show(void* a1, int a2) {
    return RwRasterShowRaster(m_wrap_raster, a1, a2) == NULL ? FALSE : TRUE;
}

void CRaster::SafeLock(U8 a1, int a2) {
    Lock(a1, a2);
}

void CRaster::Lock(U8 a1, int a2) {
    RwRasterLock(m_wrap_raster, a1, a2);
}

BOOL CRaster::Unlock() {
    return RwRasterUnlock(m_wrap_raster) == NULL ? FALSE : TRUE;
}

void CRaster::SafeLockPalette(int a1) {
    LockPalette(a1);
}

void CRaster::LockPalette(int a1) {
    RwRasterLockPalette(m_wrap_raster, a1);
}

BOOL CRaster::UnlockPalette() {
    return RwRasterUnlockPalette(m_wrap_raster) == NULL ? FALSE : TRUE;
}

}
