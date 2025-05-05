#pragma once on

#include <string>
#include "engine/display/ITextureCallback.h"

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    virtual ~CResourceFactory();

    virtual void AddTexture(DKDSP::ITexture*);
    virtual void LoadResource(u32, std::string);
    virtual void LoadResource(u32, const char*);
    virtual void* LoadPureFile(std::string&, u32*);
    virtual void* LoadPureFile(const char*, u32*);
};
