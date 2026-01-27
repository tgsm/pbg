#ifndef PIGLET_CDEBUGMENU_H
#define PIGLET_CDEBUGMENU_H

#include "types.h"
#include "CMenuControl.h"

class CGame;

// TODO
class CDebugMenu {
public:
    U8 m_unk0[0x24];
    U32 m_unk24;
    int m_unk28;
    int m_unk2C;
    U8 m_unk30[0x6C - 0x30];

public:
    CDebugMenu(CGame* game, char* xmd_filename);
    ~CDebugMenu();

    CMenuControl* GetControl(char* name);
    void RemoveControl(char* name);
    U32 UpdateAndDisplay();

    int GetUnk28() { return m_unk28; }
    int GetUnk2C() { return m_unk2C; }
};
REQUIRE_SIZE(CDebugMenu, 0x6C);

#endif
