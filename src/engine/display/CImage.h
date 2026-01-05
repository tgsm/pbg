#ifndef ENGINE_DISPLAY_CIMAGE_H
#define ENGINE_DISPLAY_CIMAGE_H

#include <rwsdk/baimage.h>
#include "engine/display/CRaster.h"
#include "engine/display/IImage.h"

class CDKW_Image : public RwImage {
public:
    static F32 GetGamma() { return RwImageGetGamma(); }
    static void SetGamma(F32 gamma) { RwImageSetGamma(gamma); }
};

namespace DKDSP {

// TODO
class CImage : public IImage {
public:
    CDKW_Image* m_wrap_image;

public:
    CImage();
    ~CImage();

    virtual BOOL AllocatePixels();
    virtual BOOL FreePixels();
    virtual BOOL SetFromRaster(IRaster*);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual int GetDepth();
    virtual int GetStride();
    virtual BOOL SetStride(int);
    virtual U8* GetPixels();
    virtual BOOL SetPixels(U8*);
    virtual CDKW_RGBA* GetPalette();
    virtual BOOL SetPalette(CDKW_RGBA*);
    virtual F32 GetGamma();
    virtual void SetGamma(F32);
    virtual BOOL GammaCorrect();

    void Release();
    void SetDkWrapImage(CDKW_Image* wrap_image);
};
REQUIRE_SIZE(CImage, 0x8);

}

#endif
