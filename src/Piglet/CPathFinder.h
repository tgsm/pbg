#ifndef PIGLET_CPATHFINDER_H
#define PIGLET_CPATHFINDER_H

#include <vector>
#include "types.h"

// TODO
class CPathFinder {
public:
    struct SDynamicBodies {

    };

    U8 m_unk0[0x4B6C - 0x0];
    std::vector<SDynamicBodies> m_unk4B6C;
    U8 m_unk4B78[0x4F78 - 0x4B78];

public:
    void Update(F32 dt);
};
REQUIRE_SIZE(CPathFinder, 0x4F78);

#endif
