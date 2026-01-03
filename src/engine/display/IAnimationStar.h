#ifndef ENGINE_DISPLAY_IANIMATIONSTAR_H
#define ENGINE_DISPLAY_IANIMATIONSTAR_H

#include <string>
#include "engine/display/IGenericAnimation.h"
#include "engine/display/CAnimationStarEntry.h"
#include "engine/display/CRWStream.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

struct STargetAnimationValues {
    F32 unk0;
    F32 unk4;
}; // Unknown size

struct SAnimationPath {
    F32 unk0;
    int unk4;
    int unk8;
    CAnimationStarEntry* unkC;
    STargetAnimationValues* unk10;
};

class IAnimationStar {
public:
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
};

}

#endif
