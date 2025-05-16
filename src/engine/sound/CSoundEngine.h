#ifndef ENGINE_SOUND_CSOUNDENGINE_H
#define ENGINE_SOUND_CSOUNDENGINE_H

#include "engine/sound/ISoundEngine.h"
#include "engine/sound/CSound2D.h"

namespace DKSND {

class CSoundEngine : public ISoundEngine {
public:
    virtual ~CSoundEngine();

    virtual void func1();
    virtual void func2();
    virtual void func3();
    virtual void func4();
    virtual void func5();
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void func14();
    virtual void func15();
    virtual void func16();
    virtual void func17();
    virtual void func18();
    virtual void func19();
    virtual void func20();
    virtual void func21();
    virtual void func22();
    virtual void func23();
    virtual void func24();
    virtual void func25();
    virtual void func26();
    virtual void func27();
    virtual void func28();
    virtual void func29();
    virtual void func30();
    virtual CSound2D* CreateSound2D(std::string*, int);
    virtual void func32();
    virtual void func33();
    virtual void func34();
    virtual void func35();
    virtual CSound2D* PlaySound2D(std::string*, int);
    virtual CSound2D* PlaySound2D(std::string, int);
    // TODO
};

}

#endif
