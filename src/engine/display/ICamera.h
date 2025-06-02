#ifndef ENGINE_DISPLAY_ICAMERA_H
#define ENGINE_DISPLAY_ICAMERA_H

#include "engine/display/CRaster.h"

namespace DKDSP {

// TODO
class ICamera {
private:
    u8 m_unk0[4]; // FIXME: This belongs to IFramable

public:
    virtual void func0() = 0;
    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void SetBuffer(IRaster*) = 0;
    virtual IRaster* GetBuffer() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void SetViewWindow(f32, f32) = 0;
    virtual void func10() = 0;
    virtual void func11() = 0;
    virtual void func12() = 0;
    virtual void func13() = 0;
    virtual void func14() = 0;
    virtual void func15() = 0;
    virtual void func16() = 0;
    virtual void func17() = 0;
    virtual void func18() = 0;
    virtual void func19() = 0;
    virtual void func20() = 0;
    virtual void func21() = 0;
    virtual void func22() = 0;
    virtual void func23() = 0;
    virtual void func24() = 0;
    virtual void func25() = 0;
    virtual void func26() = 0;
    virtual void func27() = 0;
    virtual void func28() = 0;
};

}

#endif
