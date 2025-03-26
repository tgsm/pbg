#pragma once

namespace DkXmd {

// TODO
class CChunkIterator {
public:
    CChunkIterator();

    char* GetName() const;
    char* GetStringValue() const;
    s32 GetS32Value() const;
    BOOL GetFirstChildChunk(CChunkIterator& dest) const;
    BOOL GetNextSiblingChunk(CChunkIterator& dest) const;
};

}
