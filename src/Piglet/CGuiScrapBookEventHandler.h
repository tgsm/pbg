#ifndef PIGLET_CGUISCRAPBOOKEVENTHANDLER_H
#define PIGLET_CGUISCRAPBOOKEVENTHANDLER_H

#include "CEventGuiHandlers.h"

class CGuiScrapBookEventHandler : public CGuiBaseEventHandler {
private:
    int m_unk10;
    int m_unk14;
    int m_unk18;

public:
    CGuiScrapBookEventHandler();
    virtual ~CGuiScrapBookEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void*);
};
REQUIRE_SIZE(CGuiScrapBookEventHandler, 0x1C);

#endif
