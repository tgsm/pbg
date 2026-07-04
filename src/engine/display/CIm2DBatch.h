#ifndef ENGINE_DISPLAY_CIM2DBATCH_H
#define ENGINE_DISPLAY_CIM2DBATCH_H

#include "engine/display/IIm2DBatch.h"

namespace DKDSP {

// TODO
class CIm2DBatch : public IIm2DBatch {
public:
    CIm2DBatch();
    ~CIm2DBatch();

    virtual void GrabRenderstates();
    virtual RwIm2DVertex* GetVerticeArray();
    virtual int GetNumberOfVertices();
    virtual void SetVertexPosition(int index, F32 x, F32 y, F32 z, F32 w_maybe);
    virtual void SetVertexRGBA(int index, int r, int g, int b, int a);
    virtual void SetVertexUV(int index, F32 u, F32 v);
    virtual U16* GetIndiceArray(); // FIXME: is there a RW type for this?
    virtual int GetNumberOfIndices();
    virtual void SetIndex(int, U16);
    virtual void Render(RwPrimitiveType, CDKW_Matrix*);
    virtual int GetSize();

    void Setup(int nb_vertices, int nb_indices);
    void Release();

public:
    RwIm2DVertex* m_vertices;
    int m_nb_vertices;
    U16* m_indices;
    int m_nb_indices;
    RwPrimitiveType m_prim_type;
    int m_renderstates[22];

private:
    void ResetRenderstates() {
        // FIXME: Use RW constants for this
        m_renderstates[1] = 0;
        m_renderstates[2] = 1;
        m_renderstates[3] = 1;
        m_renderstates[4] = 1;
        m_renderstates[5] = 1;
        m_renderstates[6] = 1;
        m_renderstates[7] = 2;
        m_renderstates[8] = 1;
        m_renderstates[9] = 2;
        m_renderstates[10] = 5;
        m_renderstates[11] = 6;
        m_renderstates[12] = 1;
        m_renderstates[13] = 0;
        m_renderstates[14] = 0;
        m_renderstates[15] = 0;
        m_renderstates[16] = 1;
        m_renderstates[20] = 2;
    }

    int GetRenderstate(RwRenderState state) {
        int value;
        RwRenderStateGet(state, &value);
        return value;
    }
};
REQUIRE_SIZE(CIm2DBatch, 0x70);

}

#endif
