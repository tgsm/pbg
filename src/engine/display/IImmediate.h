#ifndef ENGINE_DISPLAY_IIMMEDIATE_H
#define ENGINE_DISPLAY_IIMMEDIATE_H

#include "engine/display/CIm3DBatch.h"
#include "types.h"

namespace DKDSP {

class IIm2DBatch;
class CIm2DBatch;

class IImmediate {
public:
    virtual void func0() = 0;
    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
    virtual void func10() = 0;
    virtual void func11() = 0;
    virtual void func12() = 0;
    virtual CIm3DBatch* CreateBatch3D(int, int) = 0;
    virtual void func14() = 0;
    virtual void RemoveBatch3D(IIm3DBatch*) = 0;
    virtual CIm2DBatch* CreateBatch2D(int, int) = 0;
    virtual void func17() = 0;
    virtual void RemoveBatch2D(IIm2DBatch*) = 0;
    virtual void func19() = 0;
    virtual void func20() = 0;
    virtual void func21() = 0;
};

}

#endif
