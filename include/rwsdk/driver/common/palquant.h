#ifndef RWSDK_DRIVER_COMMON_PALQUANT_H
#define RWSDK_DRIVER_COMMON_PALQUANT_H

#include <rwsdk/baimage.h>

#ifdef __cplusplus
extern "C" {
#endif

// GoF DWARF is different for this struct.
typedef struct RwPalQuant {
    char unk0[12];
} RwPalQuant; // unknown size

int _rwPalQuantInit(RwPalQuant*);
int _rwPalQuantResolvePalette(RwRGBA*, int, RwPalQuant*);
void _rwPalQuantMatchImage(unsigned char*, int stride, int depth, int, RwPalQuant*, RwImage*);
void _rwPalQuantAddImage(float, RwPalQuant*, RwImage*);
void _rwPalQuantTerm(RwPalQuant*);

#ifdef __cplusplus
}
#endif

#endif
