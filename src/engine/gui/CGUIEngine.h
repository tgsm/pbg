#ifndef ENGINE_GUI_CGUIENGINE_H
#define ENGINE_GUI_CGUIENGINE_H

#include "engine/gui/IGUIEngine.h"

namespace DKGUI {

class CGUIEngine : public IGUIEngine {
public:
    U8 m_unk4[0x70 - 0x4];

public:
    CGUIEngine();
    virtual ~CGUIEngine();

    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void func14();
    virtual void func15();
    virtual void func16();
    virtual void func17();
    virtual void func18();
    virtual void func19();
    virtual void func20();
    virtual void func21();
    virtual void func22();
    virtual void func23();
    virtual void UpdateAndRenderOnlyTexts(RwCamera* camera);
};
REQUIRE_SIZE(CGUIEngine, 0x70);

}

#endif
