#ifndef PIGLET_CPATHFINDER_H
#define PIGLET_CPATHFINDER_H

#include <vector>
#include "engine/wrap/DKW_World.h"
#include "types.h"

class CEntityManager;

// TODO
class CPathFinder {
public:
    struct SDynamicBodies {

    };

    struct Link {

    };

    struct Node {

    };

    std::vector<Node> m_nodes;
    std::vector<Link> m_links;
    U8 m_unk18[0x24 - 0x18];
    U32 m_unk24;
    U8 m_unk28[0x4B64 - 0x28];
    U32 m_unk4B64;
    U8 m_unk4B68[4];
    std::vector<SDynamicBodies> m_unk4B6C;

public:
    CPathFinder() : m_unk24(0) {
        m_unk4B64 = 0;
    }

    CPathFinder* Init(CDKW_World*, F32, CEntityManager* manager);
    void Update(F32 dt);
};
REQUIRE_SIZE(CPathFinder, 0x4B78);

#endif
