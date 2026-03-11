#ifndef ENGINE_DISPLAY_ITEXTURECALLBACK_H
#define ENGINE_DISPLAY_ITEXTURECALLBACK_H

#include "types.h"

namespace DKDSP {

class ITexture;

class ITextureCallback {
public:
    ITextureCallback();

    virtual void AddTexture(ITexture* texture) = 0;
};
REQUIRE_SIZE(ITextureCallback, 0x4);

}

#endif
