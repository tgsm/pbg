#include "CEntityBhvTagData.h"

CEntityBhvTagData::CEntityBhvTagData() {
    m_unk0 = NULL;
    m_unk4 = NULL;
}

CEntityBhvTagData::~CEntityBhvTagData() {
    if (m_unk4 != NULL) {
        delete m_unk4;
    }
    m_unk4 = NULL;

    if (m_unk0 != NULL) {
        delete m_unk0;
    }
    m_unk0 = NULL;
}

void CEntityBhvTagData::AddBehavior(CEntityBhvTagData* unk) {
    unk->m_unk0 = m_unk0;
    m_unk0 = unk;
}

void CEntityBhvTagData::AddData(CEntityBhvTagData* unk) {
    unk->m_unk0 = m_unk4;
    m_unk4 = unk;
}
