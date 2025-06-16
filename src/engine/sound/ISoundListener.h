#ifndef ENGINE_SOUND_ISOUNDLISTENER_H
#define ENGINE_SOUND_ISOUNDLISTENER_H

#include "engine/wrap/DKW_V3d.h"

namespace DKSND {

class ISoundListener {
public:
    ISoundListener();
    virtual ~ISoundListener() {}

    virtual void SetPosition(CDKW_V3d* position) = 0;
    virtual void SetPosition(CDKW_V3d position) = 0;
    virtual CDKW_V3d* GetPosition() = 0;
    virtual void Update() = 0;
    virtual void SetUpVector(CDKW_V3d* up) = 0;
    virtual void SetUpVector(CDKW_V3d up) = 0;
    virtual CDKW_V3d* GetUpVector() = 0;
    virtual void SetFrontVector(CDKW_V3d* front) = 0;
    virtual void SetFrontVector(CDKW_V3d front) = 0;
    virtual CDKW_V3d* GetFrontVector() = 0;
    virtual void SetAttenuation(F32 attenuation) = 0;
    virtual F32 GetAttenuation() = 0;
};

}

#endif
