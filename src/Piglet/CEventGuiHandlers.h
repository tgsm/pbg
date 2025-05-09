#pragma once on

#include <string>
#include "engine/gui/IEventGUIMenuCallBack.h"

class CGame;

class CGuiBaseEventHandler : public DKGUI::IEventGUIMenuCallBack {
protected:
    std::string m_unk4;
    CGame* m_game;

public:
    CGuiBaseEventHandler(const std::string& unk);
    virtual ~CGuiBaseEventHandler();

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiNullEventHandler : public CGuiBaseEventHandler {
public:
    CGuiNullEventHandler();
    virtual ~CGuiNullEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiPauseEventHandler : public CGuiBaseEventHandler {
public:
    CGuiPauseEventHandler();
    virtual ~CGuiPauseEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadCheckingMemoryCardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadCheckingMemoryCardEventHandler();
    virtual ~CGuiLoadCheckingMemoryCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};
