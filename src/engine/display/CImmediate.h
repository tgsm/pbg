#ifndef ENGINE_DISPLAY_CIMMEDIATE_H
#define ENGINE_DISPLAY_CIMMEDIATE_H

#include "engine/display/IImmediate.h"

namespace DKDSP {

class CImmediate : public IImmediate {
public:
    virtual CIm3DBatch* CreateBatch3D(int, int);
    virtual void RemoveBatch3D(IIm3DBatch*);
    virtual CIm2DBatch* CreateBatch2D(int, int);
    virtual void RemoveBatch2D(IIm2DBatch*);
};

}

#endif
