#ifndef ENGINE_PHYSICS_DKPH_COLLIDER_H
#define ENGINE_PHYSICS_DKPH_COLLIDER_H

#include <vector>
#include "engine/physics/DkPh_Primitives.h"

class CEntity;

namespace DkPh {

class Collider {
public:
    struct Body {
        U8 unk0[4];
        CEntity* entity4;
        CDKW_V3d unk8;
        U8 unk14[0x24 - 0x14];
        int unk24;
        int unk28;
        int unk2C;
        int unk30;
        U32 unk34;
    };

    struct Contact {

    };

    struct Mesh {

    };

    U32 m_unk0;
    std::vector<Mesh> m_meshes;
    std::vector<Body> m_bodies;
    std::vector<Contact> m_contacts;
    U8 m_unk28[0x10];

public:
    Collider() : m_unk0(0) {
        m_bodies.reserve(20);
        m_contacts.reserve(20);
    }

    Body& GetBodyRef(const BVolume*);
    void AddBVolume(BVolume*, void*);
};
REQUIRE_SIZE(Collider, 0x38);

}

#endif
