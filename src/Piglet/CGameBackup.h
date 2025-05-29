#ifndef PIGLET_CGAMEBACKUP_H
#define PIGLET_CGAMEBACKUP_H

#include "types.h"

extern "C" {
extern void* OSAllocFromHeapAligned(int heap, u32 size, u32 align);
extern void OSFreeToHeap(int heap, void* ptr);
}

class CGame;

class CSlotContainer {
private:
    u32 m_unk0;
    void* m_unk4;

public:
    CSlotContainer() {
        m_unk4 = NULL;
        m_unk4 = OSAllocFromHeapAligned(0, 0x18000, 32);
        *((u32*)m_unk4) = 0; // FIXME: what is this?
        m_unk0 = NULL;
    }

    ~CSlotContainer() {
        m_unk0 = 0;
        if (m_unk4 != NULL) {
            OSFreeToHeap(0, m_unk4);
            m_unk4 = NULL;
        }
    }

    void StoreS8(s8 value, s8* dest);
    void StoreS32(s32 value, s8* dest);
    s8 GetS8(s8* src);
    s32 GetS32(s8* src);
};
REQUIRE_SIZE(CSlotContainer, 0x8);

class CGameBackup {
private:
    CGame* m_game;
    CSlotContainer* m_unk4;
    CSlotContainer* m_current_container;

public:
    CGameBackup(CGame* game);
    ~CGameBackup();

    void CreateNewGame();
    void Backup();
    void ApplyToGameData();
    CSlotContainer* GetCurrentContainer();
};
REQUIRE_SIZE(CGameBackup, 0xC);

#endif
