#ifndef ENGINE_DISPLAY_IIMAGE_H
#define ENGINE_DISPLAY_IIMAGE_H

#include "engine/wrap/DKW_RGBA.h"

namespace DKDSP {

class IRaster;

// TODO
class IImage {
public:
    IImage();

    virtual U8* AllocatePixels() = 0;
    virtual U32 FreePixels() = 0;
    virtual BOOL SetFromRaster(IRaster*) = 0;
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual U32 GetDepth() = 0;
    virtual U32 GetStride() = 0;
    virtual BOOL SetStride(int) = 0;
    virtual U8* GetPixels() = 0;
    virtual BOOL SetPixels(U8*) = 0;
    virtual CDKW_RGBA* GetPalette() = 0;
    virtual BOOL SetPalette(CDKW_RGBA*) = 0;
    virtual F32 GetGamma() = 0;
    virtual BOOL SetGamma(F32) = 0;
    virtual void* GammaCorrect() = 0;
};

}

#endif
