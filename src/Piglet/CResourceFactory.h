#ifndef PIGLET_CRESOURCEFACTORY_H
#define PIGLET_CRESOURCEFACTORY_H

#include <string>
#include "engine/display/ITextureCallback.h"
#include "engine/filesys/DkFileSys.h"

// TODO
class CErrorCallback : public CDkFileSysErrorCallBack {
public:
    CErrorCallback() {
        m_unk8 = 0;
    }
    virtual U32 Error(U32);
};

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    int m_unk4;
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
