#ifndef ENGINE_DISPLAY_CPARTICLEEMITTER_H
#define ENGINE_DISPLAY_CPARTICLEEMITTER_H

#include "engine/display/CTextureDictionary.h"
#include "engine/display/IParticleEmitter.h"
#include "engine/display/IParticleEmitterDefinition.h"

namespace DKDSP {

class CScene;

class CParticleEmitter : public IParticleEmitter {
public:
    CScene* m_scene;
    U8 m_unkC[0x54 - 0xC];

public:
    CParticleEmitter();

    virtual CDKW_Frame* GetFrame();
    virtual void SetFrame(CDKW_Frame* frame);
    virtual U32 GetSize();
    virtual std::string* GetName();
    virtual void Update(F32 dt);
    virtual void Render();
    virtual void Start();
    virtual void Pause();
    virtual BOOL IsPaused();
    virtual void Resume();
    virtual void Stop();
    virtual BOOL IsStopped();
    virtual U32 GetNumberOfActiveParticles();
    virtual BOOL IsInert();
    virtual int Load(IRWStream*);
    virtual int Load(DkXmd::CChunkIterator*);
    virtual BOOL LoadParameters(DkXmd::CChunkIterator*);
    virtual void func4C();
    virtual void func50();
    virtual void func54();
    virtual void func58();
    virtual void func5C();
    virtual void func60();
    virtual void func64();
    virtual void func68();
    virtual void func6C();
    virtual void func70();
    virtual void func74();
    virtual void func78();
    virtual void func7C();
    virtual void func80();
    virtual void func84();
    virtual void func88();
    virtual void func8C();
    virtual void func90();
    virtual void func94();
    virtual void func98();
    virtual void func9C();
    virtual void funcA0();
    virtual void funcA4();
    virtual void funcA8();
    virtual void funcAC();
    virtual void funcB0();
    virtual void funcB4();
    virtual void funcB8();
    virtual void funcBC();
    virtual void funcC0();
    virtual void funcC4();
    virtual void funcC8();
    virtual void funcCC();
    virtual void funcD0();
    virtual void funcD4();
    virtual void funcD8();
    virtual void funcDC();
    virtual void funcE0();
    virtual void funcE4();
    virtual void funcE8();
    virtual void funcEC();
    virtual void funcF0();
    virtual void funcF4();
    virtual void funcF8();
    virtual F32 GetParticleEmissionGap();
    // TODO

    void SetTextureDictionary(CTextureDictionary* texture_dictionary);
    void SetName(std::string name);
    void Create(U32, U32);
    void Load(IParticleEmitterDefinition* definition);
};
REQUIRE_SIZE(CParticleEmitter, 0x54);

}

#endif
