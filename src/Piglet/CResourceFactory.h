#ifndef PIGLET_CRESOURCEFACTORY_H
#define PIGLET_CRESOURCEFACTORY_H

#include <string>
#include "engine/display/ITextureCallback.h"

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    u8 m_unk4[0x4];
    u32 m_unk8;
    u32 m_unkC;
    u8 m_unk10[0x28 - 0x10];

public:
    CResourceFactory(CGame* game);
    virtual ~CResourceFactory();

    virtual void AddTexture(DKDSP::ITexture*);
    virtual void LoadResource(u32, std::string&);
    virtual void LoadResource(u32, const char*);
    virtual void* LoadPureFile(std::string&, u32*);
    virtual void* LoadPureFile(const char*, u32*);

    void UnloadResources(u32);
    void UnloadSpecificResource(std::string, BOOL);
};
REQUIRE_SIZE(CResourceFactory, 0x28);

#endif
