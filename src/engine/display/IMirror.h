#ifndef ENGINE_DISPLAY_IMIRROR_H
#define ENGINE_DISPLAY_IMIRROR_H

#include "engine/display/CClump.h"

namespace DKDSP {

class IMirror {
public:
    virtual void Release() = 0;
    virtual int AddClump(IClump* clump) = 0; // don't know the return type yet
    virtual void RemoveClump(IClump* clump) = 0;
    virtual void RemoveClump(int clump_id) = 0;
    virtual void RemoveAllClumps() = 0;
    virtual int GetNumberOfClumps() = 0;
    virtual CClump* GetClump(int id) = 0;
    virtual int GetClumpIndex(IClump* clump) = 0;
    virtual void SetSize(F32, F32) = 0;
    virtual BOOL IsEnabled() = 0;
    virtual void* GetCamera() = 0; // which camera?
    virtual void* GetTexture() = 0; // which texture?
    virtual void* GetRaster() = 0; // which raster?
};

}

#endif
