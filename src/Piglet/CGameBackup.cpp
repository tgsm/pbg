#include "CGameBackup.h"
#include <iostream>

void CSlotContainer::StoreS8(s8 value, s8* dest) {
    *dest = value;
}

void CSlotContainer::StoreS32(s32 value, s8* dest) {
    *((s32*)dest) = value;
}

s8 CSlotContainer::GetS8(s8* src) {
    return *src;
}

s32 CSlotContainer::GetS32(s8* src) {
    return *((s32*)src);
}

CGameBackup::CGameBackup(CGame* game) : m_game(game), m_unk4(0), m_current_container(NULL) {
    m_current_container = new CSlotContainer();
}

CGameBackup::~CGameBackup() {
    if (m_unk4 != NULL) {
        delete m_unk4;
        m_unk4 = NULL;
    }
    if (m_current_container != NULL) {
        delete m_current_container;
        m_current_container = NULL;
    }
}

void CGameBackup::Backup() {

}

CSlotContainer* CGameBackup::GetCurrentContainer() {
    return m_current_container;
}
