#ifndef PIGLET_CRESOURCEFACTORY_H
#define PIGLET_CRESOURCEFACTORY_H

#include <string>
#include "engine/display/ITextureCallback.h"

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    u8 m_unk0[0x8];
    u32 m_unk8;
    u8 m_unkC[0x24 - 0xC];

public:
    virtual ~CResourceFactory();

    virtual void AddTexture(DKDSP::ITexture*);
    virtual void LoadResource(u32, std::string);
    virtual void LoadResource(u32, const char*);
    virtual void* LoadPureFile(std::string&, u32*);
    virtual void* LoadPureFile(const char*, u32*);

    void UnloadResources(u32);
};
REQUIRE_SIZE(CResourceFactory, 0x28);

#endif
