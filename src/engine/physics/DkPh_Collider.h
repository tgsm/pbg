#ifndef ENGINE_PHYSICS_DKPH_COLLIDER_H
#define ENGINE_PHYSICS_DKPH_COLLIDER_H

#include "engine/physics/DkPh_Primitives.h"

class CEntity;

namespace DkPh {

class Collider {
public:
    struct Body {
        U8 unk0[4];
        CEntity* entity4;
        U8 unk8[0x34 - 0x8];
        U32 unk34;
    };

    Body& GetBodyRef(const BVolume*);
};

}

#endif
