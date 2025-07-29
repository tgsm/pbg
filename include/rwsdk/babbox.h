#ifndef RWSDK_BABBOX_H
#define RWSDK_BABBOX_H

#include "rwsdk/plcore/bavector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwBBox {
    RwV3d sup;
    RwV3d inf;
} RwBBox;

void RwBBoxCalculate(RwBBox* bbox, RwV3d*, int a2);
void RwBBoxInitialize(RwBBox* bbox, RwV3d*);
void RwBBoxAddPoint(RwBBox* bbox, RwV3d* point);

#ifdef __cplusplus
}
#endif

#endif
