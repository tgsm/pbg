#ifndef RWSDK_DRIVER_COMMON_PALQUANT_H
#define RWSDK_DRIVER_COMMON_PALQUANT_H

#include <rwsdk/baimage.h>
#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// GoF DWARF is different for this struct.
typedef struct RwPalQuant {
    RwUInt8 unk0[12];
} RwPalQuant; // unknown size

RwBool _rwPalQuantInit(RwPalQuant*);
RwInt32 _rwPalQuantResolvePalette(RwRGBA*, RwInt32, RwPalQuant*);
void _rwPalQuantMatchImage(RwUInt8*, RwInt32 stride, RwInt32 depth, RwInt32, RwPalQuant*, RwImage*);
void _rwPalQuantAddImage(RwReal, RwPalQuant*, RwImage*);
void _rwPalQuantTerm(RwPalQuant*);

#ifdef __cplusplus
}
#endif

#endif
