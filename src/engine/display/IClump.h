#ifndef ENGINE_DISPLAY_ICLUMP_H
#define ENGINE_DISPLAY_ICLUMP_H

#include "engine/display/CAtomic.h"
#include "engine/display/IFramableGeometry.h"
#include "engine/display/IController.h"

namespace DKDSP {

class IAnimation;

class IClump : public IFramableGeometry {
private:
public:
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual CAtomic* GetAtomic(int id) = 0;
    virtual void func10() = 0;
    virtual void func11() = 0;
    virtual void func12() = 0;
    virtual void func13() = 0;
    virtual void func14() = 0;
    virtual void func15() = 0;
    virtual int GetAtomicIndexFromID(int id) = 0;
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
    virtual void SetController(IController*) = 0;
    virtual void func28() = 0;
    virtual void func29() = 0;
    virtual void func30() = 0;
    virtual void Update(F32 dt_maybe) = 0;
    virtual void func31() = 0;
    virtual void SetAnimation(IAnimation*) = 0;
    virtual void func33() = 0;
    virtual void func34() = 0;
    virtual void func35() = 0;
    // TODO
};

}

#endif
