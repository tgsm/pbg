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

void RwBBoxAddPoint(RwBBox* bbox, RwV3d* point);
void RwBBoxInitialize(RwBBox* bbox, RwV3d* point);
void RwBBoxCalculate(RwBBox* bbox, RwV3d* points, int num_points);

#ifdef __cplusplus
}
#endif

#endif
