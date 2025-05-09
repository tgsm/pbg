#pragma once on

#include "types.h"

namespace DKGUI {

class IGUIMenu;

enum EMENU_EVENT {
    EVENT_3 = 3,
};

class IEventGUIMenuCallBack {
public:
    IEventGUIMenuCallBack() {}
    virtual ~IEventGUIMenuCallBack() {}

    virtual void OnEvent(IGUIMenu*, EMENU_EVENT, void*) = 0;
};

}
