#ifndef ENGINE_DISPLAY_IIMBATCH_H
#define ENGINE_DISPLAY_IIMBATCH_H

#include <rwsdk/badevice.h>
#include "engine/wrap/DKW_Matrix.h"

struct RxObjSpace3DVertex {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    float u;
    float v;
};
typedef struct RxObjSpace3DVertex RwIm3DVertex;

namespace DKDSP {

// TODO
class IImBatch {
public:
    IImBatch();

    virtual void GrabRenderstates() = 0;
    virtual void Render(RwPrimitiveType, CDKW_Matrix*) = 0;
    virtual void Render(RwPrimitiveType) = 0;
    virtual int GetSize() = 0;
};

}

#endif
