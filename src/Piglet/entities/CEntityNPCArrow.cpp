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

void CEntityNPCArrow::Parse(DkXmd::CChunkIterator iter) {

}

void CEntityNPCArrow::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {

}
