#include "rwsdk/babbox.h"
#include "rwsdk/plcore/bavector.h"

RwBBox* RwBBoxAddPoint(RwBBox* bbox, RwV3d* point) {
    // Equivalent: the instructions for loading these two floats are swapped
    if (bbox->inf.x > point->x) {
        bbox->inf.x = point->x;
    }
    if (bbox->inf.y > point->y) {
        bbox->inf.y = point->y;
    }
    if (bbox->inf.z > point->z) {
        bbox->inf.z = point->z;
    }
    if (bbox->sup.x < point->x) {
        bbox->sup.x = point->x;
    }
    if (bbox->sup.y < point->y) {
        bbox->sup.y = point->y;
    }
    if (bbox->sup.z < point->z) {
        bbox->sup.z = point->z;
    }

    return bbox;
}

RwBBox* RwBBoxInitialize(RwBBox* bbox, RwV3d* vec) {
    bbox->inf = *vec;
    bbox->sup = *vec;
    return bbox;
}

RwBBox* RwBBoxCalculate(RwBBox* bbox, RwV3d* points, int num_points) {
    int i;

    RwBBoxInitialize(bbox, points++);
    for (i = num_points - 1; i != 0; i--, points++) {
        RwBBoxAddPoint(bbox, points);
    }

    return bbox;
}
