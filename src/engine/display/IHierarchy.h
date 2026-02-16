#ifndef ENGINE_DISPLAY_IHIERARCHY_H
#define ENGINE_DISPLAY_IHIERARCHY_H

#include "types.h"

namespace DKDSP {

// TODO
class IHierarchy {
public:
    IHierarchy();

    virtual int GetSize() = 0;
    virtual int GetNumberOfNodes() = 0;
    virtual void func10() = 0;
    virtual void func14() = 0;
    virtual void func18() = 0;
    virtual void func1C() = 0;
    virtual void func20() = 0;
    virtual void func24() = 0;
    virtual void func28() = 0;
    virtual void func2C() = 0;
    virtual void Update() = 0;
    virtual void func34() = 0;
    virtual void func38() = 0;
    virtual void BlendSubHierarchyFast(IHierarchy*, IHierarchy*, F32) = 0;
    virtual void func40() = 0;
    virtual void func44() = 0;
    virtual IHierarchy* Copy(IHierarchy*) = 0;
    virtual void func4C() = 0;
    virtual IHierarchy* Clone(U32) = 0;
    virtual void func54() = 0;
    virtual void func58() = 0;
    virtual void func5C() = 0;
    virtual int GetIndexFromID(int id) = 0;
    virtual void func64() = 0;
    virtual void func68() = 0;
    virtual void func6C() = 0;
    virtual void func70() = 0;
    virtual void func74() = 0;
    virtual void func78() = 0;
    virtual void func7C() = 0;
    virtual void func80() = 0;
    virtual void func84() = 0;
};

}

#endif
