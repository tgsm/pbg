#ifndef ENGINE_SOUND_ISOUNDEMITTER_H
#define ENGINE_SOUND_ISOUNDEMITTER_H

#include <string>
#include "engine/sound/ISound.h"
#include "engine/wrap/DKW_V3d.h"

namespace DKSND {

class CSound3D;
class ISample;

class ISoundEmitter {
public:
    ISoundEmitter();
    virtual ~ISoundEmitter() {}

    virtual void SetPosition(CDKW_V3d* position) = 0;
    virtual void SetPosition(CDKW_V3d position) = 0;
    virtual CDKW_V3d* GetPosition() = 0;
    virtual void SetName(std::string* name) = 0;
    virtual std::string* GetName() = 0;
    virtual void AddSound(ISound* sound) = 0;
    virtual void RemoveSound(ISound* sound, int) = 0;
    virtual void RemoveAllSounds() = 0;
    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual U32 GetNbSounds() = 0;
    virtual void Update() = 0;
    virtual void SetDestroyWhenNoSound() = 0;
    virtual CSound3D* PlaySound(ISample* sample, int) = 0;
    virtual CSound3D* PlaySound(std::string*, int) = 0;
    virtual CSound3D* PlaySound(std::string, int) = 0;
};

}

#endif
