#ifndef ENGINE_DISPLAY_IWARP_H
#define ENGINE_DISPLAY_IWARP_H

#include <string>
#include "engine/display/CClump.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

class IRWStream;

enum EWARPRENDERSTATE {

};

class IWarp {
public:
    IWarp();

    virtual void Release() = 0;
    virtual void Load(IRWStream*) = 0;
    virtual void Load(DkXmd::CChunkIterator*) = 0;
    virtual int CloneClumpGetIndex(std::string) = 0;
    virtual IClump* CloneClump(std::string) = 0;
    virtual IClump* FindClump(std::string) = 0;
    virtual int FindClumpID(IClump*) = 0;
    virtual void RemoveClump(IClump*) = 0;
    virtual void RemoveClump(int) = 0;
    virtual void RemoveAllClumps() = 0;
    virtual int GetNumberOfClumps() = 0;
    virtual IClump* GetClump(int) = 0;
    virtual int GetClumpIndex(IClump*) = 0;
    virtual void SetRenderState(int, EWARPRENDERSTATE, void*) = 0;
    virtual void LinkToCamera(int, int) = 0;
    virtual void EnableRenderState(BOOL) = 0;
    virtual BOOL IsRenderStateEnable() = 0;
    virtual BOOL IsEnabled() = 0;
    virtual void Render() = 0;
};

}

#endif
