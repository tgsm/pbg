#ifndef ENGINE_GUI_IGUIENGINE_H
#define ENGINE_GUI_IGUIENGINE_H

#include "engine/input/IInput.h"

struct RwCamera;

namespace DKGUI {

class IGUIEngine {
public:
    virtual ~IGUIEngine() {}

    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
    virtual void SetButtonUpAction(DKI::IInput*) = 0;
    virtual void SetButtonDownAction(DKI::IInput*) = 0;
    virtual void SetButtonLeftAction(DKI::IInput*) = 0;
    virtual void SetButtonRightAction(DKI::IInput*) = 0;
    virtual void SetButtonOkAction(DKI::IInput*) = 0;
    virtual void SetButtonCancelAction(DKI::IInput*) = 0;
    virtual void SetButtonStartAction(DKI::IInput*) = 0;
    virtual void func17() = 0;
    virtual void func18() = 0;
    virtual void func19() = 0;
    virtual void func20() = 0;
    virtual void func21() = 0;
    virtual void func22() = 0;
    virtual void func23() = 0;
    virtual void UpdateAndRenderOnlyTexts(RwCamera* camera) = 0;
};

}

#endif
