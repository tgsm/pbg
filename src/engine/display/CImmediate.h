#ifndef ENGINE_DISPLAY_CIMMEDIATE_H
#define ENGINE_DISPLAY_CIMMEDIATE_H

#include "engine/display/IImmediate.h"

namespace DKDSP {

class CImmediate : public IImmediate {
public:
    U8 m_unk4[0x1C - 0x4];

public:
    CImmediate();
    ~CImmediate();

    virtual void func0();
    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual U32 GetSize();
    virtual void func8();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual CIm3DBatch* CreateBatch3D(int, int);
    virtual void func14();
    virtual void RemoveBatch3D(IIm3DBatch*);
    virtual CIm2DBatch* CreateBatch2D(int, int);
    virtual void func17();
    virtual void RemoveBatch2D(IIm2DBatch*);
    virtual void func19();
    virtual void func20();
    virtual void func21();

    void Release();
};
REQUIRE_SIZE(CImmediate, 0x1C);

}

#endif
