#ifndef ENGINE_DISPLAY_CANIMATIONSTAR_H
#define ENGINE_DISPLAY_CANIMATIONSTAR_H

#include "engine/display/IAnimationStar.h"

namespace DKDSP {

class CAnimDictionary;

class CAnimationStar : public IAnimationStar {
public:
    U8 m_unk4[0x40 - 0x4];

public:
    CAnimationStar();
    ~CAnimationStar();

    virtual BOOL Load(IRWStream* stream);
    virtual BOOL Load(DkXmd::CChunkIterator* iter);
    virtual BOOL DumpDot(char*);
    virtual BOOL DumpDot(char*, int);
    virtual BOOL DumpDot(char*, int, int);
    virtual int FindNextAnimationInPath(std::string, std::string, int, SAnimationPath*); // FIXME: Need actual return type
    virtual int FindNextAnimationInPath(CAnimationStarEntry*, CAnimationStarEntry*, int, SAnimationPath*); // FIXME: Need actual return type
    virtual void SetDefaultAnimation(IGenericAnimation* animation);
    virtual CAnimationStarEntry* GetAnimationEntry(std::string);
    virtual CAnimationStarEntry* GetAnimationEntry(char*);
    virtual CAnimationStarEntry* GetAnimationEntry(int);
    virtual int GetAnimationEntryIndex(IAnimationStarEntry*);
    virtual IGenericAnimation* GetDefaultAnimation();
    virtual int GetNumberOfAnimations();
    virtual std::string* GetAnimationName(int);
    virtual U32 GetSize();
    virtual std::string* GetName();

    void Release();
    void SetName(std::string name);
    SAnimationStarValues* GetDefaultAnimationValues();
    void SetAnimDictionary(CAnimDictionary* anim_dictionary);
};
REQUIRE_SIZE(CAnimationStar, 0x40);

}

#endif
