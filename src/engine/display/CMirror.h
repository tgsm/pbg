#ifndef ENGINE_DISPLAY_CMIRROR_H
#define ENGINE_DISPLAY_CMIRROR_H

#include "engine/display/IMirror.h"

namespace DKDSP {

class CMirror : public IMirror {
public:
    virtual void Release();
    virtual int AddClump(IClump* clump); // don't know the return type yet
    virtual void RemoveClump(IClump* clump);
    virtual void RemoveClump(int clump_id);
    virtual void RemoveAllClumps();
    virtual int GetNumberOfClumps();
    virtual CClump* GetClump(int id);
    virtual int GetClumpIndex(IClump* clump);
    virtual void SetSize(F32, F32);
    virtual BOOL IsEnabled();
    virtual void* GetCamera(); // which camera?
    virtual void* GetTexture(); // which texture?
    virtual void* GetRaster(); // which raster?
};

}

#endif
