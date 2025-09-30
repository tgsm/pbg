#ifndef ENGINE_DISPLAY_CTEXTURE_H
#define ENGINE_DISPLAY_CTEXTURE_H

#include "engine/display/ITexture.h"

namespace DKDSP {

class CTexture : public ITexture {
public:
    virtual U32 GetSize();
    virtual RwTextureAddressMode GetAddressing();
    virtual RwTextureAddressMode GetAddressingU();
    virtual RwTextureAddressMode GetAddressingV();
    virtual void SetAddressing(RwTextureAddressMode);
    virtual void SetAddressingU(RwTextureAddressMode);
    virtual void SetAddressingV(RwTextureAddressMode);
    virtual RwTextureFilterMode GetFilterMode();
    virtual void SetFilterMode(RwTextureFilterMode);
    virtual CRaster* GetRaster();
    virtual void SetRaster(IRaster* raster);
    virtual void GenerateMipMap();
    virtual char* GetName();
    virtual void SetName(char* name);
    virtual char* GetMaskName();
    virtual void SetMaskName(char* mask);
    virtual int GetReferenceCount();
};

}

#endif
