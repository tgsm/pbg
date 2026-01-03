#ifndef ENGINE_DISPLAY_CANIMATIONSTARENTRY_H
#define ENGINE_DISPLAY_CANIMATIONSTARENTRY_H

#include "engine/display/IController.h"
#include "engine/display/IAnimationStarEntry.h"

namespace DKDSP {

struct SAnimationStarValues {
    F32 speed;
    EANIMATIONLOOP unk4;
    F32 target_weight;
    F32 unkC;
    F32 unk10;
    F32 unk14;
    U8 unk18[0x4];
    int unk1C;

    F32 GetUnk10() { return unk10; }
    F32 GetUnk14() { return unk14; }
};

class CAnimationStarEntry : public IAnimationStarEntry {
public:
    SAnimationStarValues* GetAnimationValues();
    int GetAnimationSyncFlag(int);
};

}

#endif
