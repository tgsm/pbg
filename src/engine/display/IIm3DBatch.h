#ifndef ENGINE_DISPLAY_IIM3DBATCH_H
#define ENGINE_DISPLAY_IIM3DBATCH_H

#include "engine/display/IImBatch.h"

namespace DKDSP {

class IIm3DBatch : public IImBatch {
public:
    virtual void GrabRenderstates() = 0;
    virtual void Render(RwPrimitiveType, CDKW_Matrix*) = 0;
    virtual void Render(RwPrimitiveType);
    virtual int GetSize() = 0;
    virtual void* GetVerticeArray() = 0; // FIXME: what type is this?
    virtual int GetNumberOfVertices() = 0;
    virtual void SetVertexPosition(int, F32, F32, F32) = 0;
    virtual void SetVertexNormal(int, F32, F32, F32) = 0;
    virtual void SetVertexRGBA(int, int, int, int, int) = 0;
    virtual void SetVertexUV(int, F32, F32) = 0;
    virtual void* GetIndiceArray() = 0; // FIXME: what type is this?
    virtual int GetNumberOfIndices() = 0;
    virtual void SetIndex(int, U16) = 0;
};

}

#endif
