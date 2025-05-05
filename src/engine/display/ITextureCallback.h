#pragma once

#include "types.h"

namespace DKDSP {

class ITexture;

class ITextureCallback {
public:
    virtual void AddTexture(ITexture*) = 0;
};

}
