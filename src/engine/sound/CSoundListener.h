#ifndef ENGINE_SOUND_CSOUNDLISTENER_H
#define ENGINE_SOUND_CSOUNDLISTENER_H

#include "engine/sound/ISoundListener.h"

namespace DKSND {

class CSoundListener : public ISoundListener {
public:
    CDKW_V3d* m_pPosition;
    CDKW_V3d* m_pUp;
    CDKW_V3d* m_pFront;
    CDKW_V3d m_position;
    CDKW_V3d m_up;
    CDKW_V3d m_front;
    F32 m_attenuation;
    U8 m_unk38[0x84 - 0x38];

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

    void Clear();
    void Destroy();

    void CreateListenerSpecific();
    void UpdateListenerSpecific();
    void ReleaseListenerSpecific();
};
REQUIRE_SIZE(CSoundListener, 0x84);

}

#endif
