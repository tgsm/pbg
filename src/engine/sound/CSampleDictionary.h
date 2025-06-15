#ifndef ENGINE_SOUND_CSAMPLEDICTIONARY_H
#define ENGINE_SOUND_CSAMPLEDICTIONARY_H

#include "engine/sound/ISampleDictionary.h"

namespace DKSND {

class CSampleDictionary : public ISampleDictionary {
public:
    virtual ~CSampleDictionary();

    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
    virtual CSample* FindSample(std::string*, int); // does this return a CSample?
    virtual void func11();
    virtual void func12();
};

}

#endif
