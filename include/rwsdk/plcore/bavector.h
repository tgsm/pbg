#ifndef RWSDK_BAVECTOR_H
#define RWSDK_BAVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwV2d {
    float x;
    float y;
} RwV2d;

typedef struct RwV3d {
    float x;
    float y;
    float z;
} RwV3d;

void RwV3dNormalize(RwV3d*, RwV3d*); // FIXME: What return type?
float RwV3dLength(RwV3d* vec);

#ifdef __cplusplus
}
#endif

#endif
