#ifndef ENGINE_SOUND_CSOUND2D_H
#define ENGINE_SOUND_CSOUND2D_H

#include "engine/sound/ISound2D.h"

namespace DKSND {

class CSound2D : public ISound2D {
public:
    virtual ~CSound2D();

    virtual void SetVolume(f32 volume);
    virtual f32 GetVolume();
    virtual void SetPitch(f32 pitch);
    virtual f32 GetPitch();
    virtual void* GetSample(); // unknown return type
    virtual void SetLayer(u32 layer);
    virtual u32 GetLayer();
    // TODO
};

}

#endif
