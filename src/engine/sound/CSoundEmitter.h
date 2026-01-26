#ifndef ENGINE_SOUND_CSOUNDEMITTER_H
#define ENGINE_SOUND_CSOUNDEMITTER_H

#include <vector>
#include "engine/sound/ISoundEmitter.h"
#include "engine/wrap/DKW_V3d.h"

namespace DKSND {

class CSoundEmitter : public ISoundEmitter {
public:
    CDKW_V3d* m_pPosition;
    CDKW_V3d m_position;
    CDKW_V3d m_unk14;
    std::string m_name;
    std::vector<ISound*> m_sounds;
    U32 m_num_sounds;
    BOOL m_active;
    BOOL m_unk38;
    U8 m_unk3C[0x54 - 0x3C];

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
    virtual void Update();
    virtual void SetDestroyWhenNoSound();
    virtual CSound3D* PlaySound(ISample* sample, int);
    virtual CSound3D* PlaySound(std::string*, int);
    virtual CSound3D* PlaySound(std::string, int);

    void Clear();
    void Destroy();
    void UpdateSpecific();
};
REQUIRE_SIZE(CSoundEmitter, 0x54);

}

#endif
