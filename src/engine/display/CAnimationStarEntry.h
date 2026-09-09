#ifndef ENGINE_DISPLAY_CANIMATIONSTARENTRY_H
#define ENGINE_DISPLAY_CANIMATIONSTARENTRY_H

#include <vector>
#include "engine/display/IAnimationStar.h"
#include "engine/display/IAnimationStarEntry.h"
#include "engine/display/IController.h"
#include "engine/display/IRWStream.h"
#include "engine/xmd/CChunkIterator.h"

namespace DKDSP {

class CAnimationStar;

struct SAnimationEntry {
    int node;
    int unk4;
    IGenericAnimation* animation;
    int sync_flag;
}; // size: 0x10

class CAnimationStarEntry;

struct STargetAnimationValues {
    F32 delay_in;
    F32 delay_out;
    BOOL cut;
    CAnimationStarEntry* entry;
};

struct SAnimationStarValues {
    F32 speed;
    EANIMATIONLOOP loop_mode;
    F32 target_weight;
    F32 unkC;
    F32 delay_in;
    F32 delay_out;
    U32 unk18;
    int cut;

    F32 GetDelayIn() { return delay_in; }
    F32 GetDelayOut() { return delay_out; }
    BOOL GetCut() { return cut; }
};

struct SAnimationPath {
    F32 unk0;
    int unk4;
    int unk8;
    CAnimationStarEntry* unkC;
    STargetAnimationValues* unk10;
};

class CAnimationStarEntry : public IAnimationStarEntry {
public:
    SAnimationStarValues m_values;
    std::string m_name;
    CAnimationStar* m_star;
    std::vector<STargetAnimationValues> m_target_animations;
    std::vector<SAnimationEntry> m_animation_entries;

public:
    CAnimationStarEntry();
    ~CAnimationStarEntry();

    virtual int GetNumberOfTargetAnimations();
    virtual std::string* GetTargetAnimationName(int id);
    virtual int GetSize();
    virtual std::string* GetName();
    virtual void SetName(std::string name);
    virtual int GetNumberOfAnimations();
    virtual IGenericAnimation* GetAnimation(int);
    virtual int GetAnimationNode(int);
    virtual F32 GetAnimationDuration(EANIMATIONTYPE type);

    void Release();
    void SetAnimationStar(DKDSP::CAnimationStar* star);
    void LoadTargetAnimationValues(DkXmd::CChunkIterator* iter, STargetAnimationValues* dest);
    BOOL GetTargetAnimationEntryChunkDoWhile(DkXmd::CChunkIterator* iter, DkXmd::CChunkIterator* dest); // ?
    BOOL GetTargetAnimationEntryChunk(DkXmd::CChunkIterator* iter, DkXmd::CChunkIterator* dest); // ?
    void LoadTargetAnimationList(DkXmd::CChunkIterator* iter);
    int GetAnimationNodeFromChunk(DkXmd::CChunkIterator* iter); // ?
    int GetSyncFlagFromChunk(DkXmd::CChunkIterator* iter); // ?
    void LoadAnimationList(DkXmd::CChunkIterator* iter);
    BOOL DumpDot(IRWStream* stream, BOOL, int);
    BOOL DumpDotNode(IRWStream* stream, int id);
    BOOL FindNextAnimationInPath(IAnimationStarEntry*, IAnimationStarEntry*, IAnimationStarEntry*, int, SAnimationPath* path);
    SAnimationStarValues* GetAnimationValues();

    int GetAnimationSyncFlag(int);
};
REQUIRE_SIZE(CAnimationStarEntry, 0x44);

}

#endif
