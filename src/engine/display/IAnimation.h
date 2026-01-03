#ifndef ENGINE_DISPLAY_IANIMATION_H
#define ENGINE_DISPLAY_IANIMATION_H

#include "engine/display/IGenericAnimation.h"

namespace DKDSP {

class IAnimation : public IGenericAnimation {
public:
    IAnimation();

    virtual EANIMATIONTYPE GetType() = 0;
    virtual F32 GetDuration() = 0;
    virtual std::string* GetName() = 0;
    virtual U32 GetSize() = 0;
    virtual F32 MakeDelta(int, F32) = 0;
    virtual int OptimizeSmoothAnim(int, int, F32, F32, F32) = 0;
    virtual int OptimizeSmoothAnim(int a1) { return OptimizeSmoothAnim(a1, 0, 0.001f, 0.0001f, 0.00001f); }
    virtual int OptimizeSmoothAnim() { return OptimizeSmoothAnim(2); }
    virtual int ConvertToStdAnim() = 0;
    virtual BOOL IsDelta() = 0;
    virtual void SetDuration(F32 duration) = 0;
    virtual int GetNumberOfKeyframes() = 0;
    virtual int GetFlags() = 0;
    virtual void SetFlags(int flags) = 0;
};

}

#endif
