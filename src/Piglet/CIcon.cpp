#include "CIcon.h"
#include <iostream>

DKDSP::CCamera* CIcon::ms_pCamera;
DKDSP::CEngine* CIcon::ms_pEngine;
DKDSP::CScene* CIcon::ms_pScene;
void* CIcon::m_backupVertexAlpha;
void* CIcon::m_backupZWrite;
void* CIcon::m_backupZTest;
void* CIcon::m_backupSrcBlend;
void* CIcon::m_backupDestBlend;
void* CIcon::m_backupCullMode;

CIcon::CIcon() {
    m_texture = NULL;

    m_unk4 = 0.0f;

    m_x = 0.0f;
    m_y = 0.0f;
    m_depth = 0.0f;
    m_width = 0.0f;
    m_height = 0.0f;

    m_red = 0xFF;
    m_green = 0xFF;
    m_blue = 0xFF;
    m_alpha = 0xFF;

    m_u1 = 0.0f;
    m_v1 = 0.0f;
    m_u2 = 1.0f;
    m_v2 = 1.0f;
}

CIcon::~CIcon() {

}

extern "C" void RwRenderStateGet(int, void*);
extern "C" void RwRenderStateSet(int, int);

void CIcon::BeginRender(DKDSP::IEngine* engine, DKDSP::ICamera* camera, DKDSP::IScene* scene) {
    ms_pCamera = (DKDSP::CCamera*)camera;
    ms_pEngine = (DKDSP::CEngine*)engine;
    ms_pScene = (DKDSP::CScene*)scene;

    RwRenderStateGet(12, &m_backupVertexAlpha);
    RwRenderStateGet(8, &m_backupZWrite);
    RwRenderStateGet(6, &m_backupZTest);
    RwRenderStateGet(20, &m_backupCullMode);
    RwRenderStateGet(10, &m_backupSrcBlend);
    RwRenderStateGet(11, &m_backupDestBlend);

    ms_pEngine->SetRenderState((RwRenderState)12, (void*)1);
    ms_pEngine->SetRenderState((RwRenderState)6, (void*)0);
    ms_pEngine->SetRenderState((RwRenderState)20, (void*)1);
    ms_pEngine->SetRenderState((RwRenderState)10, (void*)5);
    ms_pEngine->SetRenderState((RwRenderState)11, (void*)6);
}

void CIcon::EndRender() {
    ms_pEngine->SetRenderState((RwRenderState)12, m_backupVertexAlpha);
    ms_pEngine->SetRenderState((RwRenderState)8, m_backupZWrite);
    ms_pEngine->SetRenderState((RwRenderState)6, m_backupZTest);
    ms_pEngine->SetRenderState((RwRenderState)20, m_backupCullMode);
    ms_pEngine->SetRenderState((RwRenderState)10, m_backupSrcBlend);
    ms_pEngine->SetRenderState((RwRenderState)11, m_backupDestBlend);
}

void CIcon::Render(DKDSP::IIm2DBatch* batch, int a2) {
    if (m_texture != NULL) {
        ms_pEngine->SetRenderState((RwRenderState)1, m_texture->GetRaster());
    } else {
        ms_pEngine->SetRenderState((RwRenderState)1, NULL);
    }

    F32 z_near = ms_pCamera->GetZNear();
    F32 z_far = ms_pCamera->GetZFar();
    F32 raster_width = ms_pCamera->GetBuffer()->GetWidth();
    F32 raster_height = ms_pCamera->GetBuffer()->GetHeight();
    F32 x = m_x * raster_width;
    F32 y = m_y * raster_height;
    F32 z = z_near + (m_depth * (z_far - z_near));
    F32 x_offset = (m_width * raster_width) / 2;
    F32 y_offset = (m_height * raster_height) / 2;

    batch->SetVertexPosition(0, x - x_offset, y - y_offset, z, 1.0f / z);
    batch->SetVertexPosition(2, x + x_offset, y - y_offset, z, 1.0f / z);
    batch->SetVertexPosition(1, x - x_offset, y + y_offset, z, 1.0f / z);
    batch->SetVertexPosition(3, x + x_offset, y + y_offset, z, 1.0f / z);
    batch->SetVertexRGBA(0, m_red, m_green, m_blue, m_alpha);
    batch->SetVertexRGBA(2, m_red, m_green, m_blue, m_alpha);
    batch->SetVertexRGBA(1, m_red, m_green, m_blue, m_alpha);
    batch->SetVertexRGBA(3, m_red, m_green, m_blue, m_alpha);
    batch->SetVertexUV(0, m_u1, m_v1);
    batch->SetVertexUV(2, m_u2, m_v1);
    batch->SetVertexUV(1, m_u1, m_v2);
    batch->SetVertexUV(3, m_u2, m_v2);
    batch->GrabRenderstates();
    ms_pScene->RenderBatch2D(batch, rwPRIMTYPETRISTRIP, a2);
}
