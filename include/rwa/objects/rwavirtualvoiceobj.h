#ifndef RWA_OBJECTS_RWAVIRTUALVOICEOBJ_H
#define RWA_OBJECTS_RWAVIRTUALVOICEOBJ_H

#include <rwa/core/rwaintf.h>
#include <rwa/core/rwaobj.h>
#include <rwa/core/rwawave.h>
#include <rwsdk/plcore/bavector.h>

#ifdef __cplusplus
extern "C" {
#endif

// Different from Ghost Rider
typedef struct RwaVirtualVoiceState {
    RwLLLink link;
    RwaObjHandle* RealVoice;
    RwBool unkC;
    RwInt32 NumRealVoices;
    RwBool Trigger;
    RwReal Frequency;
    RwReal Gain;
    RwReal Pan;
    RwBool Loop;
    RwUInt32 WavePosition; // might be / might not be start position instead?
    RwaWave* Wave;
    RwUInt32 LastPosTime;
    RwV3d Position;
    RwV3d Velocity;
    RwReal MinDistance;
    RwReal MaxDistance;
    RwReal Priority;
    RwReal Bias;
    RwReal Weight;
    RwInt32 unk60; // frequencySet? hard to prove
} RwaVirtualVoiceState;

// TODO
typedef struct RwaVirtualVoice {
    RwaObj object;
    RwaVirtualVoiceState state;
} RwaVirtualVoice;

void _rwaVirtualVoiceSetTrigger(RwaVirtualVoice* virtualVoice, RwBool trigger);
void _rwaVirtualVoiceSetWave(RwaVirtualVoice* virtualVoice, RwaWave* wave);

#ifdef __cplusplus
}
#endif

#endif
