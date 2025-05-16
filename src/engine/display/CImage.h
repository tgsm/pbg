#ifndef ENGINE_DISPLAY_CIMAGE_H
#define ENGINE_DISPLAY_CIMAGE_H

#include "engine/display/IImage.h"

namespace DKDSP {

// TODO
class CImage : public IImage {
public:
    virtual u8* AllocatePixels();
    virtual u32 FreePixels();
    virtual BOOL SetFromRaster(IRaster*);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual u32 GetDepth();
    virtual u32 GetStride();
    virtual BOOL SetStride(int);
    virtual u8* GetPixels();
    virtual BOOL SetPixels(u8*);
    virtual CDKW_RGBA* GetPalette();
    virtual BOOL SetPalette(CDKW_RGBA*);
    virtual f32 GetGamma();
    virtual BOOL SetGamma(f32);
    virtual void* GammaCorrect();
};

}

#endif
