#include <string.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plugin/dmorph/dmphanim.h>
#include "dont_inline_hack.h"

RpDMorphAnimation* RpDMorphAnimationCreate(RwUInt32 numTargets) {
    RwInt32 i;
    rpDMorphFrameSequence* sequence;
    RwInt32 size = numTargets * sizeof(rpDMorphFrameSequence) + 8;
    RpDMorphAnimation* animation = RwMalloc(size);
    DONT_INLINE_HACK();
    if (animation == NULL) {
        RwThrowErrorParams(0x122, E_RW_NOMEM, size);
        return NULL;
    }

    animation->numDMorphTargets = numTargets;
    animation->frameSequences = (rpDMorphFrameSequence*)(animation + 1);
    sequence = animation->frameSequences;
    for (i = 0; i < numTargets; i++) {
        sequence->numFrames = 0;
        sequence->frames = NULL;
        sequence++;
    }

    return animation;
}

RwBool RpDMorphAnimationDestroy(RpDMorphAnimation* animation) {
    RwInt32 i;
    rpDMorphFrameSequence* sequence = animation->frameSequences;
    for (i = 0; i < animation->numDMorphTargets; i++) {
        if (sequence->frames != NULL) {
            RwFree(sequence->frames);
        }
        sequence++;
    }

    RwFree(animation);
    return FALSE;
}

RpDMorphAnimation* RpDMorphAnimationCreateFrames(RpDMorphAnimation* animation, RwUInt32 sequenceNum, RwUInt32 numFrames) {
    RwInt32 i;
    RwInt32 size = numFrames * sizeof(rpDMorphFrame);
    rpDMorphFrameSequence* sequence = &animation->frameSequences[sequenceNum];
    DONT_INLINE_HACK();
    sequence->frames = RwMalloc(size);
    if (sequence->frames == NULL) {
        RwThrowErrorParams(0x122, E_RW_NOMEM, size);
        return NULL;
    }

    memset(sequence->frames, 0, size);
    for (i = 0; i < numFrames; i++) {
        sequence->frames[i].nextFrame = -1;
    }

    sequence->numFrames = numFrames;
    return animation;
}

RwUInt32 RpDMorphAnimationGetNumDMorphTargets(RpDMorphAnimation* animation) {
    return animation->numDMorphTargets;
}

RwUInt32 RpDMorphAnimationGetNumFrames(RpDMorphAnimation* animation, RwUInt32 sequenceNum) {
    return animation->frameSequences[sequenceNum].numFrames;
}

extern RwBool RwStreamFindChunk(RwStream*, RwInt32, RwUInt32*, RwUInt32*);
extern RwStream* RwStreamReadInt32(RwStream*, RwUInt32*, RwUInt32);
extern RwStream* RwStreamReadReal(RwStream*, RwReal*, RwUInt32);

// Equivalent: regalloc
RpDMorphAnimation* RpDMorphAnimationStreamRead(RwStream* stream) {
    rpDMorphFrameSequence* sequence;
    RpDMorphAnimation* animation;
    RwInt32 i;
    rpDMorphFrame* frame;
    RwInt32 j;
    RwUInt32 unused_30;
    RwUInt32 version;
    RwUInt32 local_38;
    RwUInt32 unused_3c;
    RwUInt32 numTargets;
    RwUInt32 unused_44;

    if (!RwStreamFindChunk(stream, 1, &unused_30, &version)) {
        return NULL;
    }

    if (!RwStreamReadInt32(stream, &local_38, sizeof(RwUInt32)) ||
        !RwStreamReadInt32(stream, &unused_3c, sizeof(RwUInt32)) ||
        !RwStreamReadInt32(stream, &numTargets, sizeof(RwUInt32)) ||
        !RwStreamReadInt32(stream, &unused_44, sizeof(RwUInt32))) {
        return NULL;
    }

    if (local_38 > 0x100) {
        RwThrowError(0x122, E_RW_BADVERSION);
        return NULL;
    }

    animation = RpDMorphAnimationCreate(numTargets);
    if (animation == NULL) {
        return NULL;
    }

    for (i = 0; i < numTargets; i++) {
        RwUInt32 numFrames;
        if (!RwStreamReadInt32(stream, &numFrames, sizeof(RwInt32))) {
            return NULL;
        }

        if (numFrames != 0) {
            if (!RpDMorphAnimationCreateFrames(animation, i, numFrames)) {
                return NULL;
            }
            frame = animation->frameSequences[i].frames;
            for (j = 0; j < numFrames; j++) {
                // The frame structure contains four floats and one u32.
                if (!RwStreamReadReal(stream, &frame->startValue, sizeof(RwReal) * 4) ||
                    !RwStreamReadInt32(stream, &frame->nextFrame, sizeof(RwUInt32))) {
                    return NULL;
                }
                frame++;
            }
        }
    }

    return animation;
}

RwUInt32 RpDMorphAnimationStreamGetSize(RpDMorphAnimation* animation) {
    RwInt32 i;
    RwUInt32 size = animation->numDMorphTargets * 4 + 0x1C;
    RwUInt32 numFrames = 0;
    for (i = 0; i < animation->numDMorphTargets; i++) {
        numFrames += animation->frameSequences[i].numFrames;
    }
    size += numFrames * sizeof(rpDMorphFrame);
    return size;
}

RwReal RpDMorphAnimationFrameGetDuration(RpDMorphAnimation* animation, RwUInt32 targetNum, RwUInt32 frameNum) {
    rpDMorphFrameSequence* target = &animation->frameSequences[targetNum];
    return target->frames[frameNum].duration;
}
