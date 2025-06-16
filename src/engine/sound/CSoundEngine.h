#ifndef ENGINE_SOUND_CSOUNDENGINE_H
#define ENGINE_SOUND_CSOUNDENGINE_H

#include "engine/sound/ISoundEngine.h"

namespace DKSND {

class CSoundEngine : public ISoundEngine {
public:
    U8 m_unk4[0x2D0 - 4];

public:
    CSoundEngine();
    virtual ~CSoundEngine();

    virtual void func1();
    virtual void Release();
    virtual void BeginUpdate();
    virtual void EndUpdate();
    virtual void SetGlobalVolume(F32 volume);
    virtual void func6();
    virtual void func7();
    virtual void func8();
    virtual void func9();
    virtual void func10();
    virtual void func11();
    virtual void func12();
    virtual void func13();
    virtual void RemoveSound(ISound* sound);
    virtual CSampleDictionary* GetSampleDictionary();
    virtual void func16();
    virtual void func17();
    virtual void func18();
    virtual void func19();
    virtual void func20();
    virtual void func21();
    virtual void func22();
    virtual CSoundEmitter* CreateEmitter();
    virtual void func24();
    virtual void func25();
    virtual void func26();
    virtual void RemoveEmitter(ISoundEmitter*);
    virtual void func28();
    virtual void func29();
    virtual void func30();
    virtual CSound2D* CreateSound2D(std::string*, int);
    virtual CSound3D* CreateSound3D(ISample* sample, int);
    virtual CSound3D* CreateSound3D(std::string* name, int);
    virtual void func34();
    virtual void func35();
    virtual CSound2D* PlaySound2D(std::string*, int);
    virtual CSound2D* PlaySound2D(std::string, int);
    virtual void func38();
    virtual void func39();
    virtual void func40();
    virtual void func41();
    virtual void func42();
    virtual void func43();
    virtual void func44();
    virtual void func45();
    virtual void func46();
    virtual void func47();
    virtual void func48();
    virtual CSound2D* PlayStreamedSound(std::string*, int);
    virtual void func50();
    virtual void func51();
    virtual void func52();
    virtual void func53();
    virtual void StopStreamedSound();
    virtual void func55();
    virtual void func56();
    virtual void DeleteAllSounds();
    virtual void SetStereoMode(int mode);
    // TODO

    void AddLayer(int);
};
REQUIRE_SIZE(CSoundEngine, 0x2D0);

}

#endif
