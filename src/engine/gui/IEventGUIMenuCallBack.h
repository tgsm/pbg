#ifndef ENGINE_GUI_IEVENTGUIMENUCALLBACK_H
#define ENGINE_GUI_IEVENTGUIMENUCALLBACK_H

#include "types.h"

namespace DKGUI {

class IGUIMenu;

enum EMENU_EVENT {
    EVENT_0 = 0,
    EVENT_1 = 1,
    EVENT_2 = 2,
    EVENT_3 = 3,
    EVENT_8 = 8,
    EVENT_9 = 9,
    EVENT_10 = 10,
    EVENT_11 = 11,
    EVENT_12 = 12,
    EVENT_13 = 13,
};

class IEventGUIMenuCallBack {
public:
    IEventGUIMenuCallBack() {}
    virtual ~IEventGUIMenuCallBack() {}

    virtual void OnEvent(IGUIMenu*, EMENU_EVENT, void*) = 0;
};

}

#endif
