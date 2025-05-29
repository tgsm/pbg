#ifndef PIGLET_CGUIMANAGER_H
#define PIGLET_CGUIMANAGER_H

#include "engine/gui/CGUIMenu.h"
#include "engine/xmd/CChunkIterator.h"

class CGame;

// FIXME: This doesn't exist, figure out what this actually is
struct PlaceholderGUIStruct {
    u8 filler[8];
    DKGUI::CGUIMenu* menu;
};

// TODO
class CGuiManager {
public:
    u8 m_unk0[0x5C];

public:
    CGuiManager(CGame* game);
    ~CGuiManager();

    BOOL LoadGui(DkXmd::CChunkIterator*, int);
    void UnLoadLevel(int);
    PlaceholderGUIStruct* GetGuiPtr(const std::string& name); // FIXME: Figure out what this actually returns
    void SetActive(const std::string& name, int);
    void SetVisible(const std::string& name, int);
    void* IsEventCallbackRegistered(const std::string& event_name);
    void Reset();
    void Update(f32 dt);
    void Render(f32 dt);
};
REQUIRE_SIZE(CGuiManager, 0x5C);

#endif
