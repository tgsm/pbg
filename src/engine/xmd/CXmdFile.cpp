#include "engine/xmd/CChunkIterator.h"
#include "engine/xmd/CXmdFile.h"
#include <cstring>

namespace DkXmd {

CXmdFile::CXmdFile() {
    m_chunk_names = NULL;
    m_xmd_data = NULL;
}

CXmdFile::~CXmdFile() {
    if (m_chunk_names != NULL) {
        delete[] m_chunk_names;
    }
}

// Very much incomplete
BOOL CXmdFile::Parse(void* xmd_data) {
    if (xmd_data == NULL) {
        return FALSE;
    }

    // Check the magic of the file: "XMD\x00\x01\x00"
    char magic[6];
    strcpy(magic, "XMD\0");
    *(U16*)(&magic[4]) = 1;
    // Byteswap
    U16 magic4 = *(U16*)(&magic[4]);
    *(U16*)(&magic[4]) = (magic4 << 8) | ((magic4 & 0xFF00) >> 8);

    if (memcmp(xmd_data, magic, sizeof(magic)) != 0) {
        return FALSE;
    }

    if (m_chunk_names != NULL) {
        delete[] m_chunk_names;
        m_chunk_names = NULL;
    }

    U8* xmd_data_cursor = (U8*)xmd_data;

    // Checks the lowest 8 bits of the *pointer* (xmd+6) instead of the value at (xmd+6).
    // This feels like a bug.
    m_unk0 = (U8)((int)xmd_data + 6) != 0 ? 1 : 0;

    xmd_data_cursor += 8;

    CChunk* chunk = reinterpret_cast<CChunk*>(xmd_data_cursor);
    if (chunk->GetType() != 0) {
        return FALSE;
    }

    int num_names_maybe;
    if (chunk->unk0_0) {
        xmd_data_cursor += 8;
    } else {
        xmd_data_cursor += 4;
    }

    int iVar1;
    if (chunk->unk0_0) {
        iVar1 = 8;
    } else {
        iVar1 = 4;
    }

    m_chunk_names = new char*[num_names_maybe];

    m_chunk_iterator.m_xmd = this;
    m_xmd_data = xmd_data;
    return TRUE;
}

}
