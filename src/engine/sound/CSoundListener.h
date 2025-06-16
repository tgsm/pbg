#ifndef ENGINE_SOUND_CSOUNDLISTENER_H
#define ENGINE_SOUND_CSOUNDLISTENER_H

#include "engine/sound/ISoundListener.h"

namespace DKSND {

class CSoundListener : public ISoundListener {
public:
    CSoundListener();
    virtual ~CSoundListener();

    virtual void SetPosition(CDKW_V3d* position);
    virtual void SetPosition(CDKW_V3d position);
    virtual CDKW_V3d* GetPosition();
    virtual void Update();
    virtual void SetUpVector(CDKW_V3d* up);
    virtual void SetUpVector(CDKW_V3d up);
    virtual CDKW_V3d* GetUpVector();
    virtual void SetFrontVector(CDKW_V3d* front);
    virtual void SetFrontVector(CDKW_V3d front);
    virtual CDKW_V3d* GetFrontVector();
    virtual void SetAttenuation(F32 attenuation);
    virtual F32 GetAttenuation();
};

}

#endif
