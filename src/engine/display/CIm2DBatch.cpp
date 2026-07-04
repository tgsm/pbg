#include "engine/display/CIm2DBatch.h"
#include "display/CEngine.h"
#include "rwsdk/badevice.h"
#include "wrap/DKW_Matrix.h"
#include <cstdlib>

namespace DKDSP {

CIm2DBatch::CIm2DBatch() {
    m_vertices = NULL;
    m_nb_vertices = 0;
    m_indices = NULL;
    m_nb_indices = 0;

    m_prim_type = rwPRIMTYPETRILIST;

    m_renderstates[21] = 0;
    ResetRenderstates();
}

CIm2DBatch::~CIm2DBatch() {
    Release();
}

void CIm2DBatch::GrabRenderstates() {
    for (int i = 1; i < 22 - 1; i++) {
        m_renderstates[i] = GetRenderstate((RwRenderState)i);
    }
    m_renderstates[21] = 1;
}

void CIm2DBatch::Setup(int nb_vertices, int nb_indices) {
    Release();

    m_nb_indices = nb_indices;
    m_nb_vertices = nb_vertices;

    m_vertices = (RwIm2DVertex*)malloc(m_nb_vertices * sizeof(RwIm2DVertex));
    if (m_vertices == NULL) {
        Release();
        return;
    }

    if (m_nb_indices > 0) {
        m_indices = (U16*)malloc(m_nb_indices * sizeof(U16));
        if (m_indices == NULL) {
            Release();
        }
    }
}

void CIm2DBatch::Release() {
    m_nb_indices = 0;
    m_nb_vertices = 0;

    if (m_vertices != NULL) {
        free(m_vertices);
    }
    m_vertices = NULL;

    if (m_indices != NULL) {
        free(m_indices);
    }
    m_indices = NULL;

    ResetRenderstates();
}

RwIm2DVertex* CIm2DBatch::GetVerticeArray() {
    return m_vertices;
}

int CIm2DBatch::GetNumberOfVertices() {
    return m_nb_vertices;
}

void CIm2DBatch::SetVertexPosition(int index, F32 x, F32 y, F32 z, F32 w_maybe) {
    if (m_vertices == NULL) {
        return;
    }

    if (index < 0 || index >= m_nb_vertices) {
        return;
    }

    m_vertices[index].x = x;
    m_vertices[index].y = y;
    m_vertices[index].z = z;
}

void CIm2DBatch::SetVertexRGBA(int index, int r, int g, int b, int a) {
    if (m_vertices == NULL) {
        return;
    }

    if (index < 0 || index >= m_nb_vertices) {
        return;
    }

    m_vertices[index].r = r;
    m_vertices[index].g = g;
    m_vertices[index].b = b;
    m_vertices[index].a = a;
}

void CIm2DBatch::SetVertexUV(int index, F32 u, F32 v) {
    if (m_vertices == NULL) {
        return;
    }

    if (index < 0 || index >= m_nb_vertices) {
        return;
    }

    m_vertices[index].u = u;
    m_vertices[index].v = v;
}

U16* CIm2DBatch::GetIndiceArray() {
    return m_indices;
}

int CIm2DBatch::GetNumberOfIndices() {
    return m_nb_indices;
}

void CIm2DBatch::SetIndex(int index_index, U16 index) {
    if (m_indices == NULL) {
        return;
    }

    if (index_index < 0 || index_index >= m_nb_indices) {
        return;
    }

    if (index >= m_nb_vertices) {
        return;
    }

    m_indices[index_index] = index;
}

extern "C" void RwIm2DRenderIndexedPrimitive(RwPrimitiveType, RwIm2DVertex*, int, RwUInt16*, int);
extern "C" void RwIm2DRenderPrimitive(RwPrimitiveType, RwIm2DVertex*, int);
extern "C" void _rwDlTextureSet(RwTexture*, int);

// Equivalent: regalloc, branching weirdness
// FIXME: Make this prettier
void CIm2DBatch::Render(RwPrimitiveType prim_type, CDKW_Matrix*) {
    if (m_vertices == NULL) {
        return;
    }

    if (m_renderstates[21] != 0) {
        for (int i = 1; i < 22 - 1; i++) {
            int state = m_renderstates[i];
            int local_20 = state;
            if (i == 17) {
                if (i != 1) {
                    if ((void*)GetRenderstate((RwRenderState)i) == &local_20) {
                        continue;
                    }
                } else {
                    if (&local_20 == 0) {
                        _rwDlTextureSet(NULL, 0);
                    }
                }
                RwRenderStateSet((RwRenderState)i, &local_20);
            } else {
                if (i != 1) {
                    if ((void*)GetRenderstate((RwRenderState)i) == (void*)state) {
                        continue;
                    }
                } else {
                    if ((void*)state == 0) {
                        _rwDlTextureSet(NULL, 0);
                    }
                }
                RwRenderStateSet((RwRenderState)i, (void*)state);
            }
        }
    }

    CEngine::ms_pEngine->AlphaEnable();

    if (prim_type == rwPRIMTYPELINELIST || prim_type == rwPRIMTYPEPOLYLINE) {
        if (m_nb_vertices < 2) {
            return;
        }
    } else if (prim_type == rwPRIMTYPETRILIST || prim_type == rwPRIMTYPETRISTRIP || prim_type == rwPRIMTYPETRIFAN) {
        if (m_nb_vertices < 3) {
            return;
        }
    }

    if (m_nb_indices > 0) {
        if (prim_type == rwPRIMTYPELINELIST || prim_type == rwPRIMTYPEPOLYLINE) {
            if (m_nb_indices < 2) {
                return;
            }
        } else if (prim_type == rwPRIMTYPETRILIST || prim_type == rwPRIMTYPETRISTRIP || prim_type == rwPRIMTYPETRIFAN) {
            if (m_nb_indices < 3) {
                return;
            }
        }
    }

    if (m_nb_indices > 0) {
        RwIm2DRenderIndexedPrimitive(prim_type, m_vertices, m_nb_vertices, m_indices, m_nb_indices);
    } else {
        RwIm2DRenderPrimitive(prim_type, m_vertices, m_nb_vertices);
    }
}

int CIm2DBatch::GetSize() {
    return m_nb_vertices * sizeof(RwIm2DVertex) + m_nb_indices * sizeof(U16);
}

}
