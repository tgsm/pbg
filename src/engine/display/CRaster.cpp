#include <rwsdk/baimras.h>
#include "engine/display/CImage.h"
#include "engine/display/CRaster.h"

namespace DKDSP {

CRaster::CRaster() {
    m_rw_raster = NULL;
    m_unk8 = 1;
}

CRaster::~CRaster() {
    Release();
}

void CRaster::Release() {
    if (m_rw_raster != NULL && m_unk8 == 1 && m_rw_raster != NULL) {
        RwRasterDestroy(m_rw_raster);
    }
    m_rw_raster = NULL;
    m_unk8 = 1;
}

void CRaster::SetDkWrapRaster(CDKW_Raster* wrap_raster, int a2) {
    m_rw_raster = (RwRaster*)wrap_raster;
    m_unk8 = a2;
}

int CRaster::GetWidth() {
    return m_rw_raster->width;
}

int CRaster::GetHeight() {
    return m_rw_raster->height;
}

int CRaster::GetDepth() {
    return m_rw_raster->depth;
}

int CRaster::GetFormat() {
    return m_rw_raster->cFormat << 8;
}

int CRaster::GetNumberOfLevels() {
    return RwRasterGetNumLevels(m_rw_raster);
}

int CRaster::GetStride() {
    return m_rw_raster->stride;
}

BOOL CRaster::SetFromImage(IImage* image) {
    return RwRasterSetFromImage(m_rw_raster, ((CImage*)image)->m_rw_image) != NULL ? TRUE : FALSE;
}

RwRaster* CRaster::GetParent() {
    return NULL;
}

BOOL CRaster::Render(int a1, int a2) {
    return RwRasterRender(m_rw_raster, a1, a2) == NULL ? FALSE : TRUE;
}

BOOL CRaster::RenderFast(int a1, int a2) {
    return RwRasterRenderFast(m_rw_raster, a1, a2) == NULL ? FALSE : TRUE;
}

BOOL CRaster::Show(void* a1, int a2) {
    return RwRasterShowRaster(m_rw_raster, a1, a2) == NULL ? FALSE : TRUE;
}

void CRaster::SafeLock(U8 a1, int a2) {
    Lock(a1, a2);
}

void CRaster::Lock(U8 a1, int a2) {
    RwRasterLock(m_rw_raster, a1, a2);
}

BOOL CRaster::Unlock() {
    return RwRasterUnlock(m_rw_raster) == NULL ? FALSE : TRUE;
}

void CRaster::SafeLockPalette(int a1) {
    LockPalette(a1);
}

void CRaster::LockPalette(int a1) {
    RwRasterLockPalette(m_rw_raster, a1);
}

BOOL CRaster::UnlockPalette() {
    return RwRasterUnlockPalette(m_rw_raster) == NULL ? FALSE : TRUE;
}

}
