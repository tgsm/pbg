#ifndef RWA_RWAVIRTUALVOICE_H
#define RWA_RWAVIRTUALVOICE_H

#include <rwa/rwaobject.h>
#include <rwa/rwawave.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaVirtualVoice {
    RwaObj object;
    char unk40[0x7C - 0x40];
    float unk7C;
    char unk80[0x8C - 0x80];
} RwaVirtualVoice;

void _rwaVirtualVoiceSetTrigger(RwaVirtualVoice* virtualVoice, int trigger);
void _rwaVirtualVoiceSetWave(RwaVirtualVoice* virtualVoice, RwaWave* wave);

#ifdef __cplusplus
}
#endif

#endif
