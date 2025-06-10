#ifndef ENGINE_DISPLAY_IPARTICLEEMITTER_H
#define ENGINE_DISPLAY_IPARTICLEEMITTER_H

#include <string>
#include "engine/display/IFramableGeometry.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

class IRWStream;

class IParticleEmitter : public IFramableGeometry {
public:
    virtual U32 GetSize() = 0;
    virtual std::string* GetName() = 0;
    virtual void Update(F32 dt) = 0;
    virtual void Render(F32 dt) = 0;
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual BOOL IsPaused() = 0;
    virtual void Resume() = 0;
    virtual void Stop() = 0;
    virtual BOOL IsStopped() = 0;
    virtual int GetNumberOfActiveParticles() = 0;
    virtual BOOL IsInert() = 0;
    virtual int Load(IRWStream*) = 0;
    virtual int Load(DkXmd::CChunkIterator*) = 0;
    virtual BOOL LoadParameters(DkXmd::CChunkIterator*) = 0;
    // TODO
};

}

#endif
