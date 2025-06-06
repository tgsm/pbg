#ifndef PIGLET_CRESOURCEFACTORY_H
#define PIGLET_CRESOURCEFACTORY_H

#include <string>
#include "engine/display/ITextureCallback.h"

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    U8 m_unk4[0x4];
    U32 m_unk8;
    U32 m_unkC;
    U8 m_unk10[0x28 - 0x10];

public:
    CResourceFactory(CGame* game);
    virtual ~CResourceFactory();

    virtual void AddTexture(DKDSP::ITexture*);
    virtual void LoadResource(U32, std::string&);
    virtual void LoadResource(U32, const char*);
    virtual void* LoadPureFile(std::string&, U32*);
    virtual void* LoadPureFile(const char*, U32*);

    void UnloadResources(U32);
    void UnloadSpecificResource(std::string, BOOL);
};
REQUIRE_SIZE(CResourceFactory, 0x28);

#endif
