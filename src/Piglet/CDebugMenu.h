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
    U8 m_unk28[0x6C - 0x28];

public:
    CDebugMenu(CGame* game, char* xmd_filename);
    ~CDebugMenu();

    CMenuControl* GetControl(char* name);
    void RemoveControl(char* name);
    U32 UpdateAndDisplay();
};
REQUIRE_SIZE(CDebugMenu, 0x6C);

#endif
