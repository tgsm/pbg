#ifndef ENGINE_DISPLAY_CRASTER_H
#define ENGINE_DISPLAY_CRASTER_H

#include <rwsdk/baraster.h>
#include "engine/display/IRaster.h"

// Not sure what to do with this, it's basically just an alias for RwRaster.
class CDKW_Raster;

namespace DKDSP {

class CRaster : public IRaster {
public:
    RwRaster* m_rw_raster;
    int m_unk8;

public:
    CRaster();
    ~CRaster();

    virtual int GetWidth();
    virtual int GetHeight();
    virtual int GetDepth();
    virtual int GetFormat();
    virtual int GetNumberOfLevels();
    virtual int GetStride();
    virtual BOOL SetFromImage(IImage* image);
    virtual RwRaster* GetParent(); // return type is a guess, this just returns NULL
    virtual BOOL Render(int, int);
    virtual BOOL RenderFast(int, int);
    virtual BOOL Show(void*, int);
    virtual void SafeLock(U8, int);
    virtual void Lock(U8, int);
    virtual BOOL Unlock();
    virtual void SafeLockPalette(int);
    virtual void LockPalette(int);
    virtual BOOL UnlockPalette();

    void Release();
    void SetDkWrapRaster(CDKW_Raster* wrap_raster, int);
};
REQUIRE_SIZE(CRaster, 0xC);

}

#endif
