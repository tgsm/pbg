#include "CEntity.h"

CEntity::CEntity(CEntityManager* entity_manager, std::string name) {
    m_unk0.assign(name, 0);
    m_flags = 3;
    m_unk10 = 0;
    m_unk14 = 0;
    m_unkC = 0;
    m_unk8 = 0;
    m_unk18 = NULL;
    m_entity_manager = entity_manager;
}

CEntity::~CEntity() {
    if (m_unk18 != NULL) {
        // FIXME: need vtable for m_unk18
        // delete m_unk18;
        m_unk18 = NULL;
    }
}

void CEntity::Init() {

}

void CEntity::Reset() {

}

u32 CEntity::GetSaveSize() {
    return 12; // FIXME: This is probably calculated
}

void CEntity::UpdateBehavior(f32 dt_maybe) {

}
