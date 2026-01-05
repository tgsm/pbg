#include <rwsdk/baimras.h>
#include "engine/display/CImage.h"

namespace DKDSP {

CImage::CImage() {
    m_wrap_image = NULL;
}

CImage::~CImage() {
    Release();
}

void CImage::Release() {
    if (m_wrap_image != NULL && m_wrap_image != NULL) {
        RwImageDestroy(m_wrap_image);
    }
    m_wrap_image = NULL;
}

void CImage::SetDkWrapImage(CDKW_Image* wrap_image) {
    m_wrap_image = wrap_image;
}

BOOL CImage::AllocatePixels() {
    return RwImageAllocatePixels(m_wrap_image) == NULL ? FALSE : TRUE;
}

BOOL CImage::FreePixels() {
    return RwImageFreePixels(m_wrap_image) == NULL ? FALSE : TRUE;
}

BOOL CImage::SetFromRaster(IRaster* raster) {
    CRaster* raster_ = (CRaster*)raster;
    if (raster_ == NULL) {
        return FALSE;
    }

    return RwImageSetFromRaster(m_wrap_image, raster_->m_wrap_raster) == NULL ? FALSE : TRUE;
}

int CImage::GetWidth() {
    return m_wrap_image->width;
}

int CImage::GetHeight() {
    return m_wrap_image->height;
}

int CImage::GetDepth() {
    return m_wrap_image->depth;
}

int CImage::GetStride() {
    return m_wrap_image->stride;
}

BOOL CImage::SetStride(int stride) {
    CDKW_Image* image = m_wrap_image;
    image->stride = stride;
    return (image == NULL) ? FALSE : TRUE;
}

U8* CImage::GetPixels() {
    return m_wrap_image->cpPixels;
}

BOOL CImage::SetPixels(U8* pixels) {
    CDKW_Image* image = m_wrap_image;
    image->cpPixels = pixels;
    return (image == NULL) ? FALSE : TRUE;
}

// FIXME: Make CDKW_RGBA inherit RwRGBA

CDKW_RGBA* CImage::GetPalette() {
    return (CDKW_RGBA*)m_wrap_image->palette;
}

BOOL CImage::SetPalette(CDKW_RGBA* palette) {
    CDKW_Image* image = m_wrap_image;
    image->palette = (RwRGBA*)palette;
    return (image == NULL) ? FALSE : TRUE;
}

F32 CImage::GetGamma() {
    return m_wrap_image->GetGamma();
}

void CImage::SetGamma(F32 gamma) {
    m_wrap_image->SetGamma(gamma);
}

BOOL CImage::GammaCorrect() {
    return RwImageGammaCorrect(m_wrap_image) == NULL ? FALSE : TRUE;
}

}
