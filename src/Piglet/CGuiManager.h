#ifndef PIGLET_CGUIMANAGER_H
#define PIGLET_CGUIMANAGER_H

#include <vector>
#include "engine/gui/CGUIMenu.h"
#include "engine/input/CCombo.h"
#include "engine/input/CInput.h"
#include "engine/xmd/CChunkIterator.h"

class CGame;

// FIXME: This doesn't exist, figure out what this actually is
struct PlaceholderGUIStruct {
    U8 filler[8];
    DKGUI::CGUIMenu* menu;
};

// TODO
class CGuiManager {
public:
    std::vector<std::string> m_unk0; // may be wrong
    std::vector<size_t> m_unkC;
    CGame* m_game;
    DKI::CInput* m_unk1C;
    DKI::CInput* m_unk20;
    DKI::CInput* m_unk24;
    DKI::CInput* m_unk28;
    DKI::CInput* m_unk2C;
    DKI::CInput* m_unk30;
    DKI::CInput* m_unk34;
    DKI::CInput* m_unk38;
    DKI::CInput* m_unk3C;
    DKI::CCombo* m_unk40;
    DKI::CCombo* m_unk44;
    DKI::CCombo* m_unk48;
    DKI::CCombo* m_unk4C;
    DKI::CInput* m_unk50;
    DKI::CInput* m_unk54;
    U32 m_unk58;

public:
    CGuiManager(CGame* game);
    ~CGuiManager();

    BOOL LoadGui(DkXmd::CChunkIterator*, int);
    void UnLoadLevel(int);
    PlaceholderGUIStruct* GetGuiPtr(const std::string& name); // FIXME: Figure out what this actually returns
    BOOL IsActive(const std::string& name);
    void SetActive(const std::string& name, int);
    void SetVisible(const std::string& name, int);
    void* IsEventCallbackRegistered(const std::string& event_name);
    void Reset();
    void Update(F32 dt);
    void Render(F32 dt);

    template <typename T>
    inline void PushEvent(size_t& event_as_size) {
        event_as_size = reinterpret_cast<size_t>(new T);
        if (event_as_size != 0) {
            reinterpret_cast<T*>(event_as_size)->SetGame(m_game);
            m_unkC.push_back(event_as_size);
        }
    }

    void PushEnterNameEvent(size_t& event_as_size);
};
REQUIRE_SIZE(CGuiManager, 0x5C);

#endif
