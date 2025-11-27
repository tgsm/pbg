#ifndef ENGINE_XMD_CXMDFILE_H
#define ENGINE_XMD_CXMDFILE_H

#include "types.h"
#include "engine/xmd/CChunkIterator.h"

namespace DkXmd {

class CXmdFile {
public:
    U32 m_unk0; // unknown return type
    char** m_chunk_names;
    CChunkIterator m_chunk_iterator;
    void* m_xmd_data;

public:
    CXmdFile();
    ~CXmdFile();

    BOOL Parse(void* xmd_data);
};
REQUIRE_SIZE(CXmdFile, 0x18);

}

#endif
