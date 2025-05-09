#pragma once on

#include "engine/gui/CGUIMenu.h"
#include "engine/xmd/CChunkIterator.h"

// FIXME: This doesn't exist, figure out what this actually is
struct PlaceholderGUIStruct {
    u8 filler[8];
    DKGUI::CGUIMenu* menu;
};

// TODO
class CGuiManager {
public:
    BOOL LoadGui(DkXmd::CChunkIterator*, int);
    void UnLoadLevel(int);
    PlaceholderGUIStruct* GetGuiPtr(const std::string& name); // FIXME: Figure out what this actually returns
    void SetActive(const std::string& name, int);
    void SetVisible(const std::string& name, int);
};
