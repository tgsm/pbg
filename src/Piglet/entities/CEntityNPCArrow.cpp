#include "entities/CEntityNPCArrow.h"
#include <iostream>

CEntityNPCArrow::CEntityNPCArrow(CEntityManager* entity_manager, std::string name) : CEntityMesh(entity_manager, name) {
    m_unk100 = CDKW_V3d(0.0f, 6.0f, 0.0f);
    m_unk10C = CDKW_V3d(0.0f, 0.0f, 0.0f);
}

CEntityNPCArrow::~CEntityNPCArrow() {

}

void CEntityNPCArrow::Reset() {
    CEntityMesh::Reset();
}

void CEntityNPCArrow::Update(F32 dt_maybe) {
    BOOL active = IsFlagged(ENTITY_FLAG_ACTIVE);
}

// Incomplete
void CEntityNPCArrow::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_UNK8) == FALSE && IsFlagged(ENTITY_FLAG_VISIBLE) == TRUE) {
        DKDSP::IIm3DBatch* batch = m_entity_manager->GetGame()->GetDisplayEngine()->GetImmediate()->CreateBatch3D(18, 0);
        CDKW_Im3DVertex* vertices = batch->GetVerticeArray();
        CDKW_V3d camera_dir = m_entity_manager->GetGame()->GetCamera()->GetAt();
        camera_dir.y = 0.0f;
        camera_dir.Normalize();
        CDKW_V3d yeah = CDKW_V3d::YAXIS;
        CDKW_V3d cross = camera_dir ^ yeah;

        m_unk100.y -= 10.0f * dt;
        m_unk10C.y += m_unk100.y * dt;
        if (m_unk10C.y <= 0.0f) {
            m_unk10C.y = 0.0f;
            m_unk100.y = 6.0f;
        }

        const CDKW_V3d& local_2c = m_unkF4 + m_unk10C;

        vertices[0].SetPosition(local_2c + (yeah * 2.0f));
        vertices[0].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[1].SetPosition(local_2c + (yeah * 1.75f) + (cross * 0.25f));
        vertices[1].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[2].SetPosition(local_2c + (yeah * 1.75f) - (cross * 0.25f));
        vertices[2].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[3].SetPosition(local_2c + (yeah * 1.75f) - (cross * 0.25f));
        vertices[3].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[4].SetPosition(local_2c + (yeah * 1.75f) + (cross * 0.25f));
        vertices[4].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[5].SetPosition(local_2c + (yeah * 1.0f) - (cross * 0.25f));
        vertices[5].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[6].SetPosition(local_2c + (yeah * 1.75f) + (cross * 0.25f));
        vertices[6].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[7].SetPosition(local_2c + (yeah * 1.0f) + (cross * 0.25f));
        vertices[7].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[8].SetPosition(local_2c + (yeah * 1.0f) - (cross * 0.25f));
        vertices[8].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[9].SetPosition(local_2c + (yeah * 1.0f) - (cross * 0.25f));
        vertices[9].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[10].SetPosition(local_2c);
        vertices[10].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[11].SetPosition(local_2c + (yeah * 0.75f) - (cross * 0.5f));
        vertices[11].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[12].SetPosition(local_2c + (yeah * 1.0f) - (cross * 0.25f));
        vertices[12].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[13].SetPosition(local_2c + (yeah * 1.0f) + (cross * 0.25f));
        vertices[13].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[14].SetPosition(local_2c);
        vertices[14].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[15].SetPosition(local_2c + (yeah * 1.0f) + (cross * 0.25f));
        vertices[15].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[16].SetPosition(local_2c);
        vertices[16].SetRGBA(0xFF, 0x00, 0x00, 0xFF);
        vertices[17].SetPosition(local_2c + (yeah * 0.75f) + (cross * 0.5f));
        vertices[17].SetRGBA(0xFF, 0x00, 0x00, 0xFF);

        m_entity_manager->GetGame()->GetDisplayEngine()->SetRenderState((RwRenderState)20, (void*)1);
        m_entity_manager->GetGame()->GetDisplayEngine()->SetRenderState((RwRenderState)1, (void*)0);
        batch->Render(rwPRIMTYPETRILIST);
        m_entity_manager->GetGame()->GetDisplayEngine()->GetImmediate()->RemoveBatch3D(batch);
    }
}

void CEntityNPCArrow::Parse(DkXmd::CChunkIterator iter) {

}

void CEntityNPCArrow::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {

}
