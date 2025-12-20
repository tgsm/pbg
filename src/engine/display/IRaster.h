#ifndef ENGINE_DISPLAY_IRASTER_H
#define ENGINE_DISPLAY_IRASTER_H

#include <rwsdk/batextur.h>
#include "engine/display/IImage.h"

namespace DKDSP {

class IRaster {
public:
    IRaster();

    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual int GetDepth() = 0;
    virtual int GetFormat() = 0;
    virtual int GetNumberOfLevels() = 0;
    virtual int GetStride() = 0;
    virtual BOOL SetFromImage(IImage* image) = 0;
    virtual RwRaster* GetParent() = 0;
    virtual BOOL Render(int, int) = 0;
    virtual BOOL RenderFast(int, int) = 0;
    virtual BOOL Show(void*, int) = 0;
    virtual void SafeLock(U8, int) = 0;
    virtual void Lock(U8, int) = 0;
    virtual BOOL Unlock() = 0;
    virtual void SafeLockPalette(int) = 0;
    virtual void LockPalette(int) = 0;
    virtual BOOL UnlockPalette() = 0;
};

}

#endif
