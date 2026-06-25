#ifndef RWSDK_PLUGIN_DMORPH_DMPHANIM_H
#define RWSDK_PLUGIN_DMORPH_DMPHANIM_H

#include <rwsdk/plcore/bastream.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rpDMorphFrame {
    RwReal startValue;
    RwReal endValue;
    RwReal duration;
    RwReal recipDuration;
    RwUInt32 nextFrame;
} rpDMorphFrame; // size: 0x14

typedef struct rpDMorphFrameSequence {
    RwUInt32 numFrames;
    rpDMorphFrame* frames;
} rpDMorphFrameSequence; // size: 0x8

typedef struct RpDMorphAnimation {
    RwUInt32 numDMorphTargets;
    rpDMorphFrameSequence* frameSequences;
} RpDMorphAnimation; // size: 0x8

RpDMorphAnimation* RpDMorphAnimationCreate(RwUInt32 numTargets);
RwBool RpDMorphAnimationDestroy(RpDMorphAnimation* animation);
RpDMorphAnimation* RpDMorphAnimationCreateFrames(RpDMorphAnimation* animation, RwUInt32 sequenceNum, RwUInt32 numFrames);
RwUInt32 RpDMorphAnimationGetNumDMorphTargets(RpDMorphAnimation* animation);
RwUInt32 RpDMorphAnimationGetNumFrames(RpDMorphAnimation* animation, RwUInt32 sequenceNum);
RpDMorphAnimation* RpDMorphAnimationStreamRead(RwStream* stream);
RwUInt32 RpDMorphAnimationStreamGetSize(RpDMorphAnimation* animation);
RwReal RpDMorphAnimationFrameGetDuration(RpDMorphAnimation* animation, RwUInt32 targetNum, RwUInt32 frameNum);

#ifdef __cplusplus
}
#endif

#endif
