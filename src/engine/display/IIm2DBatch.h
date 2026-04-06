#ifndef ENGINE_DISPLAY_IIM2DBATCH_H
#define ENGINE_DISPLAY_IIM2DBATCH_H

#include "engine/display/IImBatch.h"

// FIXME
typedef struct rwGameCube2DVertex {
    float x;
    float y;
    float z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    float u;
    float v;
} RwIm2DVertex;

namespace DKDSP {

class IIm2DBatch : public IImBatch {
public:
    IIm2DBatch();

    virtual void GrabRenderstates() = 0;
    virtual void Render(RwPrimitiveType, CDKW_Matrix*) = 0;
    virtual void Render(RwPrimitiveType type) { Render(type, NULL); }
    virtual int GetSize() = 0;
    virtual RwIm2DVertex* GetVerticeArray() = 0;
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
