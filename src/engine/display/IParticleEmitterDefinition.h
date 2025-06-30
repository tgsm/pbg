#ifndef ENGINE_DISPLAY_IPARTICLEEMITTERDEFINITION_H
#define ENGINE_DISPLAY_IPARTICLEEMITTERDEFINITION_H

#include <string>
#include "types.h"

namespace DKDSP {

class IParticleEmitterDefinition {
public:
    IParticleEmitterDefinition();

    virtual std::string* GetName() = 0;
    virtual U32 GetSize() = 0;
};

}

#endif
