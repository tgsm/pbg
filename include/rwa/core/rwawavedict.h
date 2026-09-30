#ifndef RWA_CORE_RWAWAVEDICT_H
#define RWA_CORE_RWAWAVEDICT_H

#include <rwa/core/rwallist.h>
#include <rwa/core/rwauuid.h>
#include <rwa/core/rwawave.h>
#include <rwa/core/rwawavedef.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

// Fabricated names
#define rwaWAVEDICTFLAGUSEFREELIST  (1 << 0)
#define rwaWAVEDICTFLAGNOTOWNED     (1 << 2)

typedef struct RwaWaveDict {
    RwaUniqueID uniqueID;
    RwaLLNode waveListHead;
    RwUInt8 pad[3];
    RwUInt8 flags;
    RwLLLink link;
} RwaWaveDict; // size: 0x24

RwBool _rwaWaveDictOpenModule(void);
void _rwaWaveDictCloseModule(void);
RwaWaveDict* _rwaWaveDictCreate(RwaWaveDict* dict);
void RwaWaveDictDestroy(RwaWaveDict* dict);
RwaWaveDict* RwaWaveDictAssignID(RwaWaveDict* dict, const RwaUUID* uuid, const RwChar* name);
RwaWaveDict* RwaWaveDictContainsWave(RwaWaveDict* dict, RwaWave* wave);
RwaWaveDef* RwaWaveDictUsingAllWaveDef(RwaWaveDef* def, RwBool);
RwaWaveDict* RwaWaveDictAddWave(RwaWaveDict* dict, RwaWave* wave);
RwaWaveDict* RwaWaveDictGetCurrent(void);
void _rwaWaveDictRemoveAllWave(RwaWaveDict* dict);

#ifdef __cplusplus
}
#endif

#endif
