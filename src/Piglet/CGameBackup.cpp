#include "CGameBackup.h"
#include "CGame.h"
#include <iostream>

void CSlotContainer::StoreS8(S8 value, S8* dest) {
    *dest = value;
}

void CSlotContainer::StoreS32(S32 value, S8* dest) {
    *((S32*)dest) = value;
}

S8 CSlotContainer::GetS8(S8* src) {
    return *src;
}

S32 CSlotContainer::GetS32(S8* src) {
    return *((S32*)src);
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
