#include "engine/display/DkDisplay.h"
#include "engine/display/IIm3DBatch.h"
#include "engine/wrap/DKW_Matrix.h"
#include "CMiniMapIcon.h"
#include <iostream>

CMiniMapIcon::CMiniMapIcon(const CDKW_V3d& position, F32 width, F32 height, F32 a4) {
    m_rw_matrix = RwMatrixCreate();
    m_batch = NULL;
    m_texture = NULL;

    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
    m_red = 0;
    m_green = 0;
    m_blue = 0;

    if (m_rw_matrix != NULL) {
        *m_rw_matrix = CDKW_Matrix::IDENTITY.m_rw_matrix;
    }

    m_x = position.m_x;
    m_y = position.m_y;
    m_z = position.m_z;
    m_width = width;
    m_height = height;
    *m_rw_matrix = CDKW_Matrix::IDENTITY.m_rw_matrix;

    RwV3d rwv3d;
    CDKW_V3d v3d(width, height, 1.0f);
    rwv3d.x = v3d.m_x;
    rwv3d.y = v3d.m_y;
    rwv3d.z = v3d.m_z;
    RwMatrixScale(m_rw_matrix, &rwv3d, 2);

    RwMatrix* mtx = m_rw_matrix;
    mtx->pos.x = position.m_x;
    mtx->pos.y = position.m_y;
    mtx->pos.z = position.m_z;
    RwMatrixUpdate(mtx);

    Create(a4);
}

CMiniMapIcon::~CMiniMapIcon() {
    DKDSP::CImmediate* imm = DkDisplayGetEngine()->GetImmediate();
    if (m_batch != NULL && imm != NULL) {
        imm->RemoveBatch3D(m_batch);
    }

    if (m_rw_matrix != NULL) {
        RwMatrixDestroy(m_rw_matrix);
        m_rw_matrix = NULL;
    }

    m_batch = NULL;
    m_texture = NULL;

    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;
    m_red = 0;
    m_green = 0;
    m_blue = 0;

    if (m_rw_matrix != NULL) {
        *m_rw_matrix = CDKW_Matrix::IDENTITY.m_rw_matrix;
    }
}

BOOL CMiniMapIcon::Create(F32 a1) {
    DKDSP::CEngine* display_engine = DkDisplayGetEngine();
    if (display_engine == NULL) {
        return FALSE;
    }

    DKDSP::CImmediate* imm = display_engine->GetImmediate();
    if (imm == NULL) {
        return FALSE;
    }

    DKDSP::CIm3DBatch* batch = imm->CreateBatch3D(4, 6);
    m_batch = batch;
    if (m_batch == NULL) {
        return FALSE;
    }

    m_batch->SetVertexPosition(0, -1.0f, 1.0f, 0.01f);
    m_batch->SetVertexRGBA(0, 0xFF, 0xFF, 0xFF, 0xFF);
    m_batch->SetVertexUV(0, 0.0f, 0.0f);
    m_batch->SetVertexNormal(0, 0.0f, 0.0f, 1.0f);

    m_batch->SetVertexPosition(1, 1.0f, 1.0f, 0.01f);
    m_batch->SetVertexRGBA(1, 0xFF, 0xFF, 0xFF, 0xFF);
    m_batch->SetVertexUV(1, a1, 0.0f);
    m_batch->SetVertexNormal(1, 0.0f, 0.0f, 1.0f);

    m_batch->SetVertexPosition(2, -1.0f, -1.0f, 0.01f);
    m_batch->SetVertexRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    m_batch->SetVertexUV(2, 0.0f, a1);
    m_batch->SetVertexNormal(2, 0.0f, 0.0f, 1.0f);

    m_batch->SetVertexPosition(3, 1.0f, -1.0f, 0.01f);
    m_batch->SetVertexRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
    m_batch->SetVertexUV(3, a1, a1);
    m_batch->SetVertexNormal(3, 0.0f, 0.0f, 1.0f);

    m_batch->SetIndex(0, 0);
    m_batch->SetIndex(1, 2);
    m_batch->SetIndex(2, 1);
    m_batch->SetIndex(3, 1);
    m_batch->SetIndex(4, 2);
    m_batch->SetIndex(5, 3);

    return TRUE;
}

void CMiniMapIcon::SetPosition(const CDKW_V3d& position) {
    m_x = position.m_x;
    m_y = position.m_y;
    m_z = position.m_z;

    *m_rw_matrix = CDKW_Matrix::IDENTITY.m_rw_matrix;

    RwV3d rwv3d;
    CDKW_V3d v3d(m_width, m_height, 1.0f);
    rwv3d.x = v3d.m_x;
    rwv3d.y = v3d.m_y;
    rwv3d.z = v3d.m_z;
    RwMatrixScale(m_rw_matrix, &rwv3d, 2);

    RwMatrix* mtx = m_rw_matrix;
    mtx->pos.x = m_x;
    mtx->pos.y = m_y;
    mtx->pos.z = m_z;
    RwMatrixUpdate(mtx);
}

void CMiniMapIcon::SetSize(F32 size) {
    m_width = size;
    m_height = size;

    *m_rw_matrix = CDKW_Matrix::IDENTITY.m_rw_matrix;

    CDKW_V3d v3d(size, size, 1.0f);
    RwV3d rwv3d;
    rwv3d.x = v3d.m_x;
    rwv3d.y = v3d.m_y;
    rwv3d.z = v3d.m_z;
    RwMatrixScale(m_rw_matrix, &rwv3d, 2);

    RwMatrix* mtx = m_rw_matrix;
    mtx->pos.x = m_x;
    mtx->pos.y = m_y;
    mtx->pos.z = m_z;
    RwMatrixUpdate(mtx);
}

void CMiniMapIcon::SetRGBA(U32 r, U32 g, U32 b, U32 a) {
    if (m_batch == NULL) {
        return;
    }

    for (U32 i = 0; i < 4; i++) {
        m_batch->SetVertexRGBA(i, r, g, b, a);
    }

    m_red = r;
    m_green = g;
    m_blue = b;
}

void CMiniMapIcon::SetTexture(DKDSP::ITexture* texture) {
    m_texture = (DKDSP::CTexture*)texture;
}

void CMiniMapIcon::Render(DKDSP::IScene* scene) {
    DKDSP::CEngine* display_engine = DkDisplayGetEngine();
    if (display_engine != NULL) {
        if (m_texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, m_texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, NULL);
        }
    }

    if (m_batch != NULL) {
        m_batch->GrabRenderstates();
        scene->RenderBatch3D(m_batch, rwPRIMTYPETRILIST, (CDKW_Matrix*)m_rw_matrix, 1);
        scene->Flush();
    }
}
