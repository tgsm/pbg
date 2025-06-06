#include "CMailBox.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

CMailBox::CMailBox(CGame* game, U32 max_items) {
    m_game = game;
    m_max_items = max_items;
    m_items = NULL;

    m_items = new MailBoxItem[m_max_items];
    m_unkC.clear();
    m_unkC.reserve(128);

    ResetMessageList();

    m_unk18 = 0;
}

CMailBox::~CMailBox() {
    delete[] m_items;
    m_unkC.clear();
}

// Equivalent: regalloc
// Inlined inside the constructor ...and matches
void CMailBox::ResetMessageList() {
    for (U32 i = 0; i < m_max_items; i++) {
        m_items[i].flags = 0;
        sprintf(m_items[i].unk10, "");
        sprintf(m_items[i].unk0, "");
        sprintf(m_items[i].unk20, "");
        m_items[i].unk30 = 0;
    }
}

void CMailBox::UpdateMessageList() {
    for (int i = 0, item_no_maybe = 0; i < m_max_items && item_no_maybe < m_unk18; i++) {
        if (m_items[i].flags & (1 << 0)) {
            DelFlag(i, (1 << 1));
            DelFlag(i, (1 << 3));
            item_no_maybe++;
        }
    }
}

U32 CMailBox::AddToDictionnary(std::string unk) {
    U32 index = GetDictionnaryIndexFromString(unk);
    if (index == -1) {
        m_unkC.push_back(unk);
        index = m_unkC.size() - 1;
    }
    return index;
}
