#ifndef ENGINE_DISPLAY_IIMBATCH_H
#define ENGINE_DISPLAY_IIMBATCH_H

#include "engine/wrap/DKW_Matrix.h"

// FIXME: Move these to a RW header.
enum RwPrimitiveType {
    rwPRIMTYPENAPRIMTYPE = 0,
    rwPRIMTYPELINELIST = 1,
    rwPRIMTYPEPOLYLINE = 2,
    rwPRIMTYPETRILIST = 3,
    rwPRIMTYPETRISTRIP = 4,
    rwPRIMTYPETRIFAN = 5,
    rwPRIMTYPEPOINTLIST = 6,
    rwPRIMITIVETYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
};
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
