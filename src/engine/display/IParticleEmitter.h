#ifndef ENGINE_DISPLAY_IPARTICLEEMITTER_H
#define ENGINE_DISPLAY_IPARTICLEEMITTER_H

#include <string>
#include "engine/display/CRWStream.h"
#include "engine/display/IFramableGeometry.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

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
    virtual void func4C() = 0;
    virtual void func50() = 0;
    virtual void func54() = 0;
    virtual void func58() = 0;
    virtual void func5C() = 0;
    virtual void func60() = 0;
    virtual void func64() = 0;
    virtual void func68() = 0;
    virtual void func6C() = 0;
    virtual void func70() = 0;
    virtual void func74() = 0;
    virtual void func78() = 0;
    virtual void func7C() = 0;
    virtual void func80() = 0;
    virtual void func84() = 0;
    virtual void func88() = 0;
    virtual void func8C() = 0;
    virtual void func90() = 0;
    virtual void func94() = 0;
    virtual void func98() = 0;
    virtual void func9C() = 0;
    virtual void funcA0() = 0;
    virtual void funcA4() = 0;
    virtual void funcA8() = 0;
    virtual void funcAC() = 0;
    virtual void funcB0() = 0;
    virtual void funcB4() = 0;
    virtual void funcB8() = 0;
    virtual void funcBC() = 0;
    virtual void funcC0() = 0;
    virtual void funcC4() = 0;
    virtual void funcC8() = 0;
    virtual void funcCC() = 0;
    virtual void funcD0() = 0;
    virtual void funcD4() = 0;
    virtual void funcD8() = 0;
    virtual void funcDC() = 0;
    virtual void funcE0() = 0;
    virtual void funcE4() = 0;
    virtual void funcE8() = 0;
    virtual void funcEC() = 0;
    virtual void funcF0() = 0;
    virtual void funcF4() = 0;
    virtual void funcF8() = 0;
    virtual F32 GetParticleEmissionGap() = 0;
    // TODO
};

}

#endif
