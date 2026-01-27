#ifndef RWSDK_TOOL_2D_BRUSH_H
#define RWSDK_TOOL_2D_BRUSH_H

#include "rwsdk/baimage.h"
#include "rwsdk/batextur.h"
#include "rwsdk/plcore/bavector.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct Rt2dBrush {
    char unk0;
} Rt2dBrush; // unknown size

Rt2dBrush* Rt2dBrushCreate(void);
int Rt2dBrushDestroy(Rt2dBrush* brush);
Rt2dBrush* Rt2dBrushSetRGBA(Rt2dBrush* brush, RwRGBA*, RwRGBA*, RwRGBA*, RwRGBA*);
Rt2dBrush* Rt2dBrushSetUV(Rt2dBrush* brush, RwV2d*, RwV2d*, RwV2d*, RwV2d*);
Rt2dBrush* Rt2dBrushSetTexture(Rt2dBrush* brush, RwTexture* texture);

#ifdef __cplusplus
}
#endif

#endif
