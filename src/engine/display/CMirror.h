#ifndef ENGINE_DISPLAY_CMIRROR_H
#define ENGINE_DISPLAY_CMIRROR_H

#include <vector>
#include "engine/display/CRaster.h"
#include "engine/display/IMirror.h"

namespace DKDSP {

class CEngine;
class CScene;

class CMirror : public IMirror {
public:
    U32 m_unk4;
    U32 m_unk8;
    CRaster m_unkC;
    U32 m_unk18;
    CRaster m_unk1C;
    U32 m_unk28;
    U32 m_unk2C;
    U32 m_unk30;
    U32 m_unk34;
    std::vector<CClump*> m_unk38;
    U32 m_unk44;
    F32 m_unk48;
    F32 m_unk4C;

public:
    CMirror() {
        m_unk4 = 0;
        m_unk8 = 0;
        m_unk2C = 0;
        m_unk30 = 0;
        m_unk44 = 0;
        m_unk18 = 0;
        m_unk28 = 0;
        m_unk34 = 1;
        m_unk48 = 1.0f;
        m_unk4C = 1.0f;
    }

    virtual void Release();
    virtual int AddClump(IClump* clump); // don't know the return type yet
    virtual void RemoveClump(IClump* clump);
    virtual void RemoveClump(int clump_id);
    virtual void RemoveAllClumps();
    virtual int GetNumberOfClumps();
    virtual CClump* GetClump(int id);
    virtual int GetClumpIndex(IClump* clump);
    virtual void SetSize(F32, F32);
    virtual BOOL IsEnabled();
    virtual void* GetCamera(); // which camera?
    virtual void* GetTexture(); // which texture?
    virtual void* GetRaster(); // which raster?

    void Init(CEngine* engine, CScene* scene, U32 width, U32 height, U32);
    void SetEnabled(BOOL enable);
    void Render();
    void AssociateClump(CClump* clump);
};
REQUIRE_SIZE(CMirror, 0x50);

}

#endif
