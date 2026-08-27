#ifndef PIGLET_CGAMEBACKUP_H
#define PIGLET_CGAMEBACKUP_H

#include "engine/Allocator.h"
#include "types.h"

class CGame;

// FIXME: These were very sloppily put together. Something is probably wrong here.
// Unofficial name
struct SSubslot {
    char name[0x10];
    int offset;
    void* unk14;
    int unk18;
    int unk1C;
};
// Unofficial name
struct SSlot {
    U32 unk0;
    SSubslot sslot[];
};

class CSlotContainer {
public:
    int m_unk0;
    SSlot* m_unk4;

public:
    CSlotContainer() {
        m_unk4 = NULL;
        m_unk4 = (SSlot*)OSAllocFromHeapAligned(0, 0x18000, 32);
        m_unk4->unk0 = 0;
        m_unk0 = NULL;
    }

    ~CSlotContainer() {
        m_unk0 = 0;
        if (m_unk4 != NULL) {
            OSFreeToHeap(0, m_unk4);
            m_unk4 = NULL;
        }
    }

    void StoreS8(S8 value, S8* dest);
    void StoreS32(S32 value, S8* dest);
    S8 GetS8(S8* src);
    S32 GetS32(S8* src);
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
    void GetFromGameData(U32);
    void ApplyToGameData();
    CSlotContainer* GetCurrentContainer();
};
REQUIRE_SIZE(CGameBackup, 0xC);

#endif
