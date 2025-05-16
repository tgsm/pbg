#ifndef ENGINE_XMD_CCHUNKITERATOR_H
#define ENGINE_XMD_CCHUNKITERATOR_H

#include "types.h"

namespace DkXmd {

class CChunk {
public:
    u32 unk0_0 : 1;
    u32 type : 5;
    u32 unk0_6 : 2;

    void* unk4;
    void* unk8;

public:
    u32 GetType() {
        return type;
    }

    void** GetData();
};

class CChunkInvert {
public:
    union {
        u32 yeah;
        struct {
            u32 unk0_0 : 8;
            u32 unk0_8 : 8;
            u32 unk0_16 : 8;
            u32 unk0_30 : 2;
            u32 type : 5;
            u32 fdsfds : 1;
        } flags;
    };
    // Unknown types
    void* unk4;
    void* unk8;

    // inline volatile CChunkInvert& byteswapped() {
    //     volatile CChunkInvert result;
    //     result.yeah = yeah;
    //     result.yeah =
    //         ((yeah & 0x000000FF) << 24) |
    //         ((yeah & 0x0000FF00) << 8) |
    //         ((yeah & 0x00FF0000) >> 8) |
    //         ((yeah & 0xFF000000) >> 24);
    //     return result;
    // }

    // u32 GetType() {
    //     return byteswapped().flags.type;
    // }

public:
    void** GetData();
};

struct CCI_UnkSubstruct {
    BOOL m_inverted;
    char** unk4;
};

// TODO
class CChunkIterator {
private:
    CCI_UnkSubstruct* m_unk0;
    union {
        CChunk* m_non_inverted;
        CChunkInvert* m_inverted;
        void* ptr;
    } m_chunk;
    u32 m_unk8;

public:
    CChunkIterator();

    s32 GetType() const;
    char* GetName() const;
    u32 GetDataSize() const;
    char* GetStringValue() const;
    s32 GetS32Value() const;
    f32 GetFloatValue() const;
    BOOL GetFirstChildChunk(CChunkIterator& dest) const;
    BOOL GetNextSiblingChunk(CChunkIterator& dest) const;
    BOOL GetChunk(const char* name, CChunkIterator& dest) const;

    // void* GetData() const {
    //     if (!m_unk0->m_inverted) {
    //         return m_chunk.m_non_inverted->GetData();
    //     } else {
    //         return m_chunk.m_inverted->GetData();
    //     }
    // }
};

}

#endif
