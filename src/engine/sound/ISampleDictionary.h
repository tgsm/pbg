#ifndef ENGINE_SOUND_ISAMPLEDICTIONARY_H
#define ENGINE_SOUND_ISAMPLEDICTIONARY_H

#include <string>
#include "types.h"

namespace DKSND {

class CSample;

class ISampleDictionary {
public:
    virtual ~ISampleDictionary();

    virtual void func1() = 0;
    virtual void func2() = 0;
    virtual void func3() = 0;
    virtual void func4() = 0;
    virtual void func5() = 0;
    virtual void func6() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
    virtual CSample* FindSample(std::string*, int) = 0; // does this return a CSample?
    virtual void func11() = 0;
    virtual void func12() = 0;
};

}

#endif
