#ifndef ENGINE_DISPLAY_CWARP_H
#define ENGINE_DISPLAY_CWARP_H

#include "engine/display/IWarp.h"

namespace DKDSP {

class CWarp : public IWarp {
public:
    CWarp();

    virtual void Release();
    virtual void Load(IRWStream*);
    virtual void Load(DkXmd::CChunkIterator*);
    virtual int CloneClumpGetIndex(std::string);
    virtual IClump* CloneClump(std::string);
    virtual IClump* FindClump(std::string);
    virtual int FindClumpID(IClump*);
    virtual void RemoveClump(IClump*);
    virtual void RemoveClump(int);
    virtual void RemoveAllClumps();
    virtual int GetNumberOfClumps();
    virtual IClump* GetClump(int);
    virtual int GetClumpIndex(IClump*);
    virtual void SetRenderState(int, EWARPRENDERSTATE, void*);
    virtual void LinkToCamera(int, int);
    virtual void EnableRenderState(BOOL);
    virtual BOOL IsRenderStateEnable();
    virtual BOOL IsEnabled();
    virtual void Render();
};

}

#endif
