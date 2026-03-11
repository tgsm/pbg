#ifndef ENGINE_SOUND_CSAMPLEDICTIONARY_H
#define ENGINE_SOUND_CSAMPLEDICTIONARY_H

#include "engine/sound/ISampleDictionary.h"

namespace DKSND {

class CSampleDictionary : public ISampleDictionary {
public:
    virtual ~CSampleDictionary();

    virtual CSampleBank* LoadSampleBankFromFile(std::string* name, std::string* filename);
    virtual CSampleBank* LoadStreamSampleBankFromFile(std::string* name, std::string* filename);
    virtual CSampleBank* FindSampleBank(std::string* name);
    virtual void func4();
    virtual void RemoveSampleBank(std::string* name);
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
