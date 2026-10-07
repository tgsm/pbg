#ifndef RWA_OBJECTS_RWAVIRTUALVOICEOBJ_H
#define RWA_OBJECTS_RWAVIRTUALVOICEOBJ_H

#include <rwa/core/rwaintf.h>
#include <rwa/core/rwaobj.h>
#include <rwa/core/rwawave.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct RwaVirtualVoice {
    RwaObj object;
    char unk28[0x3C - 0x28];
    RwInt32 unk3C;
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
