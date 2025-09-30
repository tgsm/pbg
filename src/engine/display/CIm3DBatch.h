#ifndef ENGINE_DISPLAY_CIM3DBATCH_H
#define ENGINE_DISPLAY_CIM3DBATCH_H

#include "engine/display/IIm3DBatch.h"

namespace DKDSP {

class CIm3DBatch : public IIm3DBatch {
public:
    virtual void GrabRenderstates();
    virtual void Render(RwPrimitiveType, CDKW_Matrix*);
    virtual int GetSize();
    virtual void* GetVerticeArray(); // FIXME: what type is this?
    virtual int GetNumberOfVertices();
    virtual void SetVertexPosition(int, F32, F32, F32);
    virtual void SetVertexNormal(int, F32, F32, F32);
    virtual void SetVertexRGBA(int, int, int, int, int);
    virtual void SetVertexUV(int, F32, F32);
    virtual void* GetIndiceArray(); // FIXME: what type is this?
    virtual int GetNumberOfIndices();
    virtual void SetIndex(int, U16);
};

}

#endif
