#include "engine/display/CTexture.h"

namespace DKDSP {

CTexture::~CTexture() {
    Release();
}

void CTexture::Release() {
    if (m_wrap_texture != NULL && m_wrap_texture != NULL) {
        RwTextureDestroy(m_wrap_texture);
    }
    m_wrap_texture = NULL;
}

void CTexture::SetDkWrapTexture(CDKW_Texture* wrap_texture) {
    m_wrap_texture = wrap_texture;
    if (m_wrap_texture != NULL) {
        m_raster.SetDkWrapRaster((CDKW_Raster*)wrap_texture->raster, 0);
    }
}

U32 CTexture::GetSize() {
    return 0;
}

RwTextureAddressMode CTexture::GetAddressing() {
    if ((U32)CTexture::GetAddressingU() == (U32)CTexture::GetAddressingV()) {
        return CTexture::GetAddressingV();
    }
    return rwTEXTUREADDRESSNATEXTUREADDRESS;
}

RwTextureAddressMode CTexture::GetAddressingU() {
    return (RwTextureAddressMode)((m_wrap_texture->filterAddressing & 0x0F00) >> 8);
}

RwTextureAddressMode CTexture::GetAddressingV() {
    return (RwTextureAddressMode)((m_wrap_texture->filterAddressing & 0xF000) >> 12);
}

void CTexture::SetAddressing(RwTextureAddressMode mode) {
    m_wrap_texture->filterAddressing = ((mode << 8) & 0x0F00) | ((mode << 12) & 0xF000) | (m_wrap_texture->filterAddressing & ~0xFF00);
}

void CTexture::SetAddressingU(RwTextureAddressMode mode) {
    m_wrap_texture->filterAddressing = (m_wrap_texture->filterAddressing & ~0x0F00) | ((mode << 8) & 0x0F00);
}

void CTexture::SetAddressingV(RwTextureAddressMode mode) {
    m_wrap_texture->filterAddressing = (m_wrap_texture->filterAddressing & ~0xF000) | ((mode << 12) & 0xF000);
}

RwTextureFilterMode CTexture::GetFilterMode() {
    return (RwTextureFilterMode)(m_wrap_texture->filterAddressing & 0x00FF);
}

void CTexture::SetFilterMode(RwTextureFilterMode mode) {
    m_wrap_texture->filterAddressing = (m_wrap_texture->filterAddressing & ~0x00FF) | (mode & 0xFF);
}

CRaster* CTexture::GetRaster() {
    return &m_raster;
}

void CTexture::SetRaster(IRaster* raster) {
    if (raster != NULL) {
        m_raster.SetDkWrapRaster(((CRaster*)raster)->m_wrap_raster, 0);
        RwTextureSetRaster(m_wrap_texture, m_raster.m_wrap_raster);
    } else {
        m_raster.Release();
        RwTextureSetRaster(m_wrap_texture, NULL);
    }
}

void CTexture::GenerateMipMap() {
    if (m_raster.m_wrap_raster != NULL) {
        RwTextureRasterGenerateMipmaps(m_raster.m_wrap_raster, 0);
    }
}

char* CTexture::InternalGetName() {
    return m_wrap_texture->name;
}

char* CTexture::GetName() {
    return InternalGetName();
}

void CTexture::SetName(char* name) {
    RwTextureSetName(m_wrap_texture, name);
}

char* CTexture::GetMaskName() {
    return m_wrap_texture->mask;
}

void CTexture::SetMaskName(char* mask) {
    RwTextureSetMaskName(m_wrap_texture, mask);
}

int CTexture::GetReferenceCount() {
    if (m_wrap_texture != NULL) {
        return m_wrap_texture->refCount;
    }

    return 0;
}

}
