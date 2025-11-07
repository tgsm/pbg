#ifndef ENGINE_SOUND_ISOUNDENGINE_H
#define ENGINE_SOUND_ISOUNDENGINE_H

#include <string>
#include "engine/sound/CSampleDictionary.h"
#include "engine/sound/CSound2D.h"
#include "engine/sound/CSoundEmitter.h"

namespace DKSND {

class ISoundEngine {
public:
    virtual ~ISoundEngine();

    virtual void Initialize(void*) = 0;
    virtual void Release() = 0;
    virtual void BeginUpdate() = 0;
    virtual void EndUpdate() = 0;
    virtual void SetGlobalVolume(F32 volume) = 0;
    virtual F32 GetGlobalVolume() = 0;
    virtual void func7() = 0;
    virtual void func8() = 0;
    virtual void func9() = 0;
    virtual void func10() = 0;
    virtual void func11() = 0;
    virtual void PauseLayer(int layer) = 0;
    virtual void ResumeLayer(int layer) = 0;
    virtual void RemoveSound(ISound* sound) = 0;
    virtual CSampleDictionary* GetSampleDictionary() = 0;
    virtual void func16() = 0;
    virtual void func17() = 0;
    virtual void func18() = 0;
    virtual void func19() = 0;
    virtual void func20() = 0;
    virtual void func21() = 0;
    virtual void func22() = 0;
    virtual CSoundEmitter* CreateEmitter() = 0;
    virtual void func24() = 0;
    virtual void func25() = 0;
    virtual void func26() = 0;
    virtual void RemoveEmitter(ISoundEmitter*) = 0;
    virtual void func28() = 0;
    virtual void func29() = 0;
    virtual void func30() = 0;
    virtual CSound2D* CreateSound2D(std::string*, int) = 0;
    virtual CSound3D* CreateSound3D(ISample* sample, int) = 0;
    virtual CSound3D* CreateSound3D(std::string* name, int) = 0;
    virtual void func34() = 0;
    virtual void func35() = 0;
    virtual CSound2D* PlaySound2D(std::string*, int) = 0;
    virtual CSound2D* PlaySound2D(std::string, int) = 0;
    virtual CSound2D* PlaySound2DLooped(std::string*, int) = 0;
    virtual CSound2D* PlaySound2DLooped(std::string, int) = 0;
    virtual void func40() = 0;
    virtual void func41() = 0;
    virtual void func42() = 0;
    virtual void func43() = 0;
    virtual void func44() = 0;
    virtual void func45() = 0;
    virtual void func46() = 0;
    virtual void func47() = 0;
    virtual void func48() = 0;
    virtual CSound2D* PlayStreamedSound(std::string*, int) = 0;
    virtual void func50() = 0;
    virtual void func51() = 0;
    virtual void func52() = 0;
    virtual ISound* GetStreamedSound() = 0;
    virtual void StopStreamedSound() = 0;
    virtual void func55() = 0;
    virtual void func56() = 0;
    virtual void DeleteAllSounds() = 0;
    virtual void SetStereoMode(int mode) = 0;
    virtual int RWAGetNbFreeVoice() = 0;
    virtual F32 RWAGetCurrentStreamPosition() = 0;
    virtual void RWASetCurrentStreamPosition(F32) = 0;
};

}

#endif
