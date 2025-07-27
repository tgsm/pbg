#ifndef ENGINE_DISPLAY_COBJECTDICTIONARY_H
#define ENGINE_DISPLAY_COBJECTDICTIONARY_H

#include "engine/display/IObjectDictionary.h"

namespace DKDSP {

class CObjectDictionary : public IObjectDictionary {
public:
    virtual void Release();
    virtual CParticleEmitterDefinition* FindParticleEmitterDefinition(std::string);
};

}

#endif
