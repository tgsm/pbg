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
    IAnimationStar();

    virtual BOOL Load(IRWStream* stream) = 0;
    virtual BOOL Load(DkXmd::CChunkIterator* iter) = 0;
    virtual BOOL DumpDot(char*) = 0;
    virtual BOOL DumpDot(char*, int) = 0;
    virtual BOOL DumpDot(char*, int, int) = 0;
    virtual int FindNextAnimationInPath(std::string, std::string, int, SAnimationPath*) = 0; // FIXME: Need actual return type
    virtual int FindNextAnimationInPath(CAnimationStarEntry*, CAnimationStarEntry*, int, SAnimationPath*) = 0; // FIXME: Need actual return type
    virtual void SetDefaultAnimation(IGenericAnimation* animation) = 0;
    virtual CAnimationStarEntry* GetAnimationEntry(std::string) = 0;
    virtual CAnimationStarEntry* GetAnimationEntry(char*) = 0;
    virtual CAnimationStarEntry* GetAnimationEntry(int) = 0;
    virtual int GetAnimationEntryIndex(IAnimationStarEntry*) = 0;
    virtual IGenericAnimation* GetDefaultAnimation() = 0;
    virtual int GetNumberOfAnimations() = 0;
    virtual std::string* GetAnimationName(int) = 0;
    virtual U32 GetSize() = 0;
    virtual std::string* GetName() = 0;
};

}

#endif
