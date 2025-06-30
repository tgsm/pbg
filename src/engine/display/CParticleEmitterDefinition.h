#ifndef ENGINE_DISPLAY_CPARTICLEEMITTERDEFINITION_H
#define ENGINE_DISPLAY_CPARTICLEEMITTERDEFINITION_H

#include "engine/display/IParticleEmitterDefinition.h"

namespace DKDSP {

// TODO
class CParticleEmitterDefinition : public IParticleEmitterDefinition {
public:
    U8 unk4[0x14C - 0x4];

public:
    CParticleEmitterDefinition();

    virtual std::string* GetName();
    virtual U32 GetSize();
};
REQUIRE_SIZE(CParticleEmitterDefinition, 0x14C);

}

#endif
