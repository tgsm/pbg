#ifndef PIGLET_CGUISCRAPBOOKEVENTHANDLER_H
#define PIGLET_CGUISCRAPBOOKEVENTHANDLER_H

#include "CEventGuiHandlers.h"

class CGuiScrapBookEventHandler : public CGuiBaseEventHandler {
public:
    int m_unk10;
    int m_unk14;
    int m_unk18;

public:
    CGuiScrapBookEventHandler();
    virtual ~CGuiScrapBookEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void*);

    int GetUnk10() { return m_unk10; }
};
REQUIRE_SIZE(CGuiScrapBookEventHandler, 0x1C);

#endif
