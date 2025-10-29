#ifndef ENGINE_DISPLAY_ICONTROLLER_H
#define ENGINE_DISPLAY_ICONTROLLER_H

#include "types.h"

namespace DKDSP {

enum EANIMATIONTYPE {
    ANIMATION_TYPE_ANM,
    ANIMATION_TYPE_DMA,
    ANIMATION_TYPE_TAN,
    ANIMATION_TYPE_EVN,
    ANIMATION_TYPE_4,
};

enum EANIMATIONLOOP {
    ANIMATION_LOOP_3 = 3,
};

class IGenericAnimation;

// TODO
class IController {
public:
    virtual void func0() = 0;
    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void Resume() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
    virtual void func10() = 0;
    virtual void func11() = 0;
    virtual void func12() = 0;
    virtual void func13() = 0;
    virtual void func14() = 0;
    virtual void Update(F32 dt) = 0;
    virtual void func16() = 0;
    virtual void func17() = 0;
    virtual void func18() = 0;
    virtual void func19() = 0;
    virtual int PlayAnimation(IGenericAnimation* animation, int, F32, F32, F32, F32) = 0;
    virtual void func21() = 0;
    virtual void func22() = 0;
    virtual void func23() = 0;
    virtual void StopAllAnimations(EANIMATIONTYPE, int) = 0;
    virtual void func25() = 0;
    virtual void func26() = 0;
    virtual void func27() = 0;
    virtual void func28() = 0;
    virtual void func29() = 0;
    virtual void func30() = 0;
    virtual void func31() = 0;
    virtual void func32() = 0;
    virtual void func33() = 0;
    virtual void func34() = 0;
    virtual void func35() = 0;
    virtual void func36() = 0;
    virtual void func37() = 0;
    virtual void func38() = 0;
    virtual void func39() = 0;
    virtual void func40() = 0;
    virtual void func41() = 0;
    virtual void func42() = 0;
    virtual void func43() = 0;
    virtual void func44() = 0;
    virtual void func45() = 0;
    virtual void func46() = 0;
    virtual int FindAnimationFromUID(EANIMATIONTYPE type, int uid) = 0;
    virtual void func48() = 0;
    virtual void func49() = 0;
    virtual void func50() = 0;
    virtual void func51() = 0;
    virtual void func52() = 0;
    virtual void func53() = 0;
    virtual void func54() = 0;
    virtual void func55() = 0;
    virtual void func56() = 0;
    virtual void func57() = 0;
    virtual void SetAnimationLoopMode(EANIMATIONTYPE type, int, EANIMATIONLOOP, int) = 0;
    // TODO
};

}

#endif
