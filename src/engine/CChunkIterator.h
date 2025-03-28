#ifndef ENGINE_CCHUNKITERATOR_H
#define ENGINE_CCHUNKITERATOR_H

#include "types.h"

namespace DkXmd {

// TODO
class CChunkIterator {
private:
    u8 m_unk0[12];

public:
    CChunkIterator();

    char* GetName() const;
    char* GetStringValue() const;
    s32 GetS32Value() const;
    f32 GetFloatValue() const;
    BOOL GetFirstChildChunk(CChunkIterator& dest) const;
    BOOL GetNextSiblingChunk(CChunkIterator& dest) const;
};

}

#endif
