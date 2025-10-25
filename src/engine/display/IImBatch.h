#ifndef ENGINE_DISPLAY_IIMBATCH_H
#define ENGINE_DISPLAY_IIMBATCH_H

#include "engine/wrap/DKW_Matrix.h"

// FIXME: Move this to a RW header.
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

namespace DKDSP {

// TODO
class IImBatch {
public:
    virtual void GrabRenderstates() = 0;
    virtual void Render(RwPrimitiveType, CDKW_Matrix*) = 0;
    virtual void Render(RwPrimitiveType) = 0;
    virtual int GetSize() = 0;
};

}

#endif
