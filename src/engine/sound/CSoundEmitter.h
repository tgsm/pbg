#ifndef ENGINE_SOUND_CSOUNDEMITTER_H
#define ENGINE_SOUND_CSOUNDEMITTER_H

#include "engine/sound/ISoundEmitter.h"

namespace DKSND {

class CSoundEmitter : public ISoundEmitter {
public:
    CSoundEmitter();
    virtual ~CSoundEmitter();

    virtual void SetPosition(CDKW_V3d* position);
    virtual void SetPosition(CDKW_V3d position);
    virtual CDKW_V3d* GetPosition();
    virtual void SetName(std::string* name);
    virtual std::string* GetName();
    virtual void AddSound(ISound* sound);
    virtual void RemoveSound(ISound* sound, int);
    virtual void RemoveAllSounds();
    virtual void Activate();
    virtual void Deactivate();
    virtual U32 GetNbSounds();
    virtual void Update(F32 dt);
    virtual void SetDestroyWhenNoSound();
    virtual CSound3D* PlaySound(ISample* sample, int);
    virtual CSound3D* PlaySound(std::string*, int);
    virtual CSound3D* PlaySound(std::string, int);
};

}

#endif
