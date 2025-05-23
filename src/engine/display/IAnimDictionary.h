#ifndef ENGINE_DISPLAY_IANIMDICTIONARY_H
#define ENGINE_DISPLAY_IANIMDICTIONARY_H

#include <string>

namespace DKDSP {

class CAnimation;

// TODO
class IAnimDictionary {
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
    virtual void func13() = 0;
    virtual void func14() = 0;
    virtual void func15() = 0;
    virtual CAnimation* FindAnimation(std::string name) = 0;
};

}

#endif
