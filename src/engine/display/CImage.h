#ifndef ENGINE_DISPLAY_CIMAGE_H
#define ENGINE_DISPLAY_CIMAGE_H

#include "engine/display/IImage.h"

namespace DKDSP {

// TODO
class CImage : public IImage {
public:
    virtual U8* AllocatePixels();
    virtual U32 FreePixels();
    virtual BOOL SetFromRaster(IRaster*);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual U32 GetDepth();
    virtual U32 GetStride();
    virtual BOOL SetStride(int);
    virtual U8* GetPixels();
    virtual BOOL SetPixels(U8*);
    virtual CDKW_RGBA* GetPalette();
    virtual BOOL SetPalette(CDKW_RGBA*);
    virtual F32 GetGamma();
    virtual BOOL SetGamma(F32);
    virtual void* GammaCorrect();
};

}

#endif
