#include "engine/filesys/DkFileSys.h"
#include <cstring>

BOOL CDkFileSys::loadfat(char* path, DkXmd::CXmdFile* xmd) {
    DkFileSysDescriptor* file = (DkFileSysDescriptor*)RWFileInterface.rwfopen(path, "rb");
    if (file == NULL) {
        return FALSE;
    }

    RWFileInterface.rwfseek(file, 0, 2); // SEEK_END
    m_FatSize = RWFileInterface.rwftell(file);
    m_FatMemEntry = new U8[m_FatSize];
    if (m_FatMemEntry == NULL) {
        return FALSE;
    }

    RWFileInterface.rwfseek(file, 0, 0); // SEEK_SET
    RWFileInterface.rwfread(m_FatMemEntry, 1, m_FatSize, file);
    RWFileInterface.rwfclose(file);

    return xmd->Parse(m_FatMemEntry) != FALSE;
}

extern U8 __lower_map[256];
inline int tolower(int c) { return (c == -1 ? -1 : (int)__lower_map[(U8)c]); }

static inline int string_comp(const char* a, const char* b) {
    if (a == NULL && b == NULL) {
        return 0;
    }

    if (a == NULL) {
        return -1;
    } else if (b == NULL) {
        return 1;
    }

    char a_, b_;
    do {
        a_ = tolower(*(a++));
        b_ = tolower(*(b++));

        if (a_ < b_) {
            return -1;
        } else if (a_ > b_) {
            return 1;
        }
    } while (a_ != 0);

    return 0;
}

BOOL CDkFileSys::FindFileOnFatBegeningOn(DkXmd::CChunkIterator& iter, const char* path, DkXmd::CChunkIterator& iter2, DkFileSysInfoFromFat* info) {
    DkXmd::CChunkIterator dest;

    int type = iter.GetType();
    if (type == 4) {
        char* string_value = iter.GetStringValue();
        if (string_value != NULL) {
            if (string_comp(string_value, path) == 0) {
                info->unk8 = 1;
                info->unk18 = iter;

                if (iter.GetNextSiblingChunk(dest)) {
                    char* yeah = dest.GetName();
                    if (yeah != NULL && yeah != NULL) {
                        if (string_comp(dest.GetName(), "FATFDesc") == 0) {
                            info->unk8 = 2;

                            DkXmd::CChunkIterator dest3, unused;

                            dest.GetFirstChildChunk(dest3);
                            dest3.GetName();
                            info->unkC = dest3.GetS32Value();

                            dest3.GetNextSiblingChunk(dest3);
                            dest3.GetName();
                            info->unk10 = dest3.GetS32Value();

                            dest3.GetNextSiblingChunk(dest3);
                            dest3.GetName();
                            info->unk14 = dest3.GetS32Value();
                        }
                    }
                }
                return TRUE;
            }
        }
    }

    if (string_comp(iter.GetName(), "FATPack") == 0) {
        DkXmd::CChunkIterator dest2;
        if (!iter.GetFirstChildChunk(dest2)) {
            m_Error = 5;
            return 0;
        }
        info->name = dest2.GetStringValue();

        if (!dest2.GetNextSiblingChunk(dest2)) {
            m_Error = 5;
            return 0;
        }
        info->offset = dest2.GetS32Value();
    }

    if (iter.m_chunk.ptr == iter2.m_chunk.ptr || !iter.GetFirstChildChunk(dest)) {
        return FALSE;
    }

    do {
        dest.GetName();
        if (FindFileOnFatBegeningOn(dest, path, iter2, info)) {
            return TRUE;
        }
        dest.GetName();
    } while (dest.GetNextSiblingChunk(dest));

    return FALSE;
}

BOOL CDkFileSys::FindFileOnFat(const char* path, DkFileSysInfoFromFat* info) {
    DkXmd::CChunkIterator dest;
    info->unk8 = 0;
    info->name = m_CurrentPackName;
    info->offset = m_CurrentPackSize;

    if (!m_OnFatPosition.GetFirstChildChunk(dest) && !m_OnFatPosition.GetNextSiblingChunk(dest)) {
        return FindFileOnFatBegeningOn(m_Fat->m_chunk_iterator, path, m_OnFatPosition, info);
    }

    do {
        if (FindFileOnFatBegeningOn(dest, path, m_OnFatPosition, info)) {
            return info->unk8;
        }
    } while (dest.GetNextSiblingChunk(dest));

    return FindFileOnFatBegeningOn(m_Fat->m_chunk_iterator, path, m_OnFatPosition, info);
}

DkFileSysDescriptor* CDkFileSys::IsPackAllreadyOpened(char* a0) {
    for (U32 i = 0; i < m_pUnCompressedMemory.size(); i++) {
        DkFileSysDescriptor* desc = m_pUnCompressedMemory[i];
        if (desc != NULL && desc->m_unk2C != NULL) {
            if (string_comp(desc->m_unk2C, a0) == 0 && (desc->m_unk0 & 0x40)) {
                return desc;
            }
        }
    }

    return NULL;
}

unsigned char* CDkFileSys::KeepOnlyFileName(char* path) {
    unsigned int ch;
    unsigned char* ret = (U8*)path;
    for (; path != NULL && (ch = *(U8*)path, ch != '\0'); path++) {
        if (ch == '/' || ch == '\\') {
            ret = (U8*)path + 1;
        }
    }
    return ret;
}

// Incomplete: regalloc
int CDkFileSys::FREADInPackByBlock(void* ptr, size_t size, size_t n, DkFileSysDescriptor* file, int* a4) {
    U32 yeah;
    int unk;

    *a4 = 0;
    int ret = 0;
    U8* ptr_ = (U8*)ptr;
    U32 local_28 = size * n;
    if ((int)(file->m_unk8 + local_28) > file->m_unk14) {
        local_28 = file->m_unk14 - file->m_unk8;
    }

    if ((int)local_28 < file->m_unk14) {
        if (file->m_unk6C == NULL) {
            file->m_unk6C = new U8[0x10000];
            UncompressNewBlock(file, 0x10000);
        }

        while (ret < local_28) {
            unk = local_28 - ret;
            int iVar2 = file->m_unk74;
            yeah = file->m_unk78 - file->m_unk74;
            if (unk < yeah) {
                memcpy(ptr_, (void*)((int)file->m_unk6C + iVar2), unk);
                file->m_unk74 += unk;
                ret += unk;
                file->m_unk8 += unk;

                return ret / size;
            } else {
                memcpy(ptr_, (void*)((int)file->m_unk6C + iVar2), yeah);
                ptr_ = (U8*)((int)ptr_ + yeah);
                ret += (file->m_unk78 - file->m_unk74);
                file->m_unk8 += yeah;
                file->m_unk74 = file->m_unk78;
                if (file->m_unk8 >= file->m_unk14) {
                    return ret / size;
                }

                UncompressNewBlock(file, 0x10000);
            }
        }

        return ret / size;
    } else {
        U32 iVar2 = UncompressByBlock(ptr_, &local_28, NULL, file->m_unk10, file, 1);
        if (file->m_unk20 != NULL) {
            delete[] file->m_unk20;
            file->m_unk20 = NULL;
        }
        *a4 = 1;

        if (iVar2 != 0) {
            return 0;
        } else {
            return local_28 / size;
        }
    }

    return ret;
}
