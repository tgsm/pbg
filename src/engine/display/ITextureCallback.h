#ifndef ENGINE_DISPLAY_ITEXTURECALLBACK_H
#define ENGINE_DISPLAY_ITEXTURECALLBACK_H

#include "types.h"

namespace DKDSP {

class ITexture;

class ITextureCallback {
public:
    virtual void AddTexture(ITexture*) = 0;
};

}

#endif
