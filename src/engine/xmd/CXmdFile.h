#pragma once on

#include "types.h"
#include "engine/xmd/CChunkIterator.h"

namespace DkXmd {

class CXmdFile {
public:
    u32 m_unk0; // unknown return type
    u32* m_unk4; // unknown return type
    CChunkIterator m_chunk_iterator;
    u32 m_unk14; // unknown return type

public:
    CXmdFile();
    ~CXmdFile();

    BOOL Parse(void* unk);
};
REQUIRE_SIZE(CXmdFile, 0x18);

}
