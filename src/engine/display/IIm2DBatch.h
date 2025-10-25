#ifndef ENGINE_DISPLAY_IIM2DBATCH_H
#define ENGINE_DISPLAY_IIM2DBATCH_H

#include "engine/display/IImBatch.h"

namespace DKDSP {

// TODO
class IIm2DBatch : public IImBatch {
public:
    virtual void GrabRenderstates() = 0;
    virtual void Render(RwPrimitiveType, CDKW_Matrix*) = 0;
    virtual void Render(RwPrimitiveType);
    virtual int GetSize() = 0;
    virtual void* GetVerticeArray() = 0; // FIXME: what type is this?
    virtual int GetNumberOfVertices() = 0;
    virtual void SetVertexPosition(int index, F32 x, F32 y, F32 z, F32 w_maybe) = 0;
    virtual void SetVertexRGBA(int index, int r, int g, int b, int a) = 0;
    virtual void SetVertexUV(int index, F32 u, F32 v) = 0;
    virtual void* GetIndiceArray() = 0; // FIXME: what type is this?
    virtual int GetNumberOfIndices() = 0;
    virtual void SetIndex(int, U16) = 0;
};

}

#endif
