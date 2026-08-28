#include "engine/filesys/DkFileSys.h"
#include <cstring>

std::vector<DkFileSysDescriptor*> CDkFileSys::m_pUnCompressedMemory;
DkXmd::CChunkIterator CDkFileSys::m_OnFatPosition;
#ifdef VERSION_GPLP9G
std::vector<SDkFileSysAlias> CDkFileSys::m_vAliases;
#endif
RwFileFunctions RWFileInterface;

int CDkFileSys::m_DescriptorIdCounter = 0;
DkXmd::CXmdFile* CDkFileSys::m_Fat = NULL;
U32 CDkFileSys::m_FatSize = 0;
U8* CDkFileSys::m_FatMemEntry = NULL;
char* CDkFileSys::m_CurrentPackName = NULL;
int CDkFileSys::m_CurrentPackSize = 0;
int CDkFileSys::m_Flag = 0;
int CDkFileSys::m_Error = 0;
BOOL CDkFileSys::m_NoCompression = FALSE;
CDkFileSysLoadCallBack* CDkFileSys::m_LoadingCB = NULL;
size_t CDkFileSys::m_SizeBetweenCBLaunch = 0;
int CDkFileSys::m_LoadingCumulator = 0;
CDkFileSysErrorCallBack* CDkFileSys::m_ErrorCallBack = NULL;
DkFileSysDescriptor* CDkFileSys::m_LastFileDescriptorUsed = NULL;
void* CDkFileSys::m_ReadBuffer = NULL; // Unknown type

int CDkFileSys::m_LastPointerOnPack = -1;

// FIXME: This needs a home.
extern U8 __lower_map[256];
inline int tolower(int c) { return (c == -1 ? -1 : (int)__lower_map[(U8)c]); }

// Copied over from DKDSP (with the added check at the top), name unknown, inlined and deadstripped.
static int dkfilesys_stricmp(const char* a, const char* b) {
    if (a == NULL && b == NULL) {
        return 0;
    } else if (a == NULL) {
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

BOOL CDkFileSys::init(char* xmd_path, int flags, int) {
    RwFileFunctions* interface = RwOsGetFileInterface();
    if (interface == NULL) {
        m_Error = 4;
        return FALSE;
    }

    RWFileInterface = *interface;

    if (!(m_Flag & (1 << 1)) || flags & (1 << 2)) {
        interface->rwfexist = fexist;
        interface->rwfopen = fopen;
        interface->rwfclose = fclose;
        interface->rwfread = fread;
        interface->rwfgets = fgets;
        interface->rwfeof = feof;
        interface->rwfseek = fseek;
        interface->rwftell = ftell;
        interface->rwfflush = fflush;
        interface->rwfwrite = fwrite;
        interface->rwfputs = fputs;
    }

    m_Fat = new DkXmd::CXmdFile;
    if (xmd_path != NULL) {
        if (!loadfat(xmd_path, m_Fat)) {
            m_Error = 3;
            return FALSE;
        }
#ifdef VERSION_GPLE9G
        m_OnFatPosition = m_Fat->m_chunk_iterator;

        DkXmd::CChunkIterator iter;
        if (m_OnFatPosition.GetFirstChildChunk(iter)) {
            if (dkfilesys_stricmp(iter.GetName(), "FATNoCompression") == 0) {
                m_NoCompression = TRUE;
            }
        }
#endif
    } else {
        if (loadfat("DkFat.xmd", m_Fat)) {
            flags |= (1 << 2);
#ifdef VERSION_GPLE9G
            m_OnFatPosition = m_Fat->m_chunk_iterator;

            DkXmd::CChunkIterator iter;
            if (m_OnFatPosition.GetFirstChildChunk(iter)) {
                if (dkfilesys_stricmp(iter.GetName(), "FATNoCompression") == 0) {
                    m_NoCompression = TRUE;
                }
            }
#endif
        } else if (flags & (1 << 2)) {
            m_Error = 3;
            return FALSE;
        }
    }

    m_Flag = flags;
    m_LoadingCB = NULL;
    m_ErrorCallBack = NULL;
#ifdef VERSION_GPLP9G
    m_vAliases.reserve(5);
#endif

    return TRUE;
}

// Equivalent (USA): regalloc
// Incomplete (PAL)
BOOL CDkFileSys::exit() {
#ifdef VERSION_GPLE9G
    if (m_Fat == NULL) {
        return FALSE;
    }
#endif

    RwFileFunctions* interface = RwOsGetFileInterface();
    *interface = RWFileInterface;

    if (m_Fat != NULL) {
        delete m_Fat;
        m_Fat = NULL;
    }

    if (m_FatMemEntry != NULL) {
        delete m_FatMemEntry;
        m_FatMemEntry = NULL;
    }

    while (m_pUnCompressedMemory.size() != 0) {
        DkFileSysDescriptor* fd = m_pUnCompressedMemory[0];
        if (fd->unk20 != NULL) {
            delete fd->unk20;
            fd->unk20 = NULL;
        }
        if (fd->unk7C != NULL) {
            delete[] fd->unk7C;
            fd->unk7C = NULL;
        }
        if (fd->unk24 != NULL) {
            delete[] fd->unk24;
            fd->unk24 = NULL;
        }

        if (m_Flag & (1 << 0) && !(fd->flags & (1 << 6))) {
            inflateEnd(&fd->zstream);
        }

        AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).erase(reinterpret_cast<unsigned long*>(fd));

        if (fd->flags & (1 << 4)) {
            RWFileInterface.rwfclose(fd->file);
        }
        delete fd;
    }

    if (m_ReadBuffer != NULL) {
        delete[] m_ReadBuffer;
        m_ReadBuffer = NULL;
    }

#ifdef VERSION_GPLP9G
    for (int i = 0; i < (int)m_vAliases.size(); i++) {
        SDkFileSysAlias& alias = m_vAliases[i];
        void* unk0 = alias.unk0;
        delete[] alias.unk4;
        delete[] unk0;
    }
    m_vAliases.clear();
#endif

    return TRUE;
}

// Used by zlib. Should we use U8 here?
static void* MyAlloc_func(void*, U32 items, U32 size) {
    return new U8[items * size];
}

// Used by zlib.
static void MyFree_func(void*, void* ptr) {
    delete[] ptr;
}

int CDkFileSys::fexist(const char* filename) {
    void* fd = fopen(filename, "r");
    if (fd != NULL) {
        fclose(fd);
        return TRUE;
    }

    return FALSE;
}

static int unkinline(const char* mode) {
    int ret = 1;
    for (; mode != NULL && *mode != '\0'; mode++) {
        if (*mode == 'r') {
            return ret;
        }
        ret++;
    }
    return 0;
}

// Equivalent(?) (USA): volatile shenanigans
// Incomplete (PAL)
void* CDkFileSys::fopen(const char* filename, const char* mode) {
    DkFileSysDescriptor* volatile fd;
    m_Error = 0;

    if (unkinline(mode) == 0) {
        fd = new DkFileSysDescriptor;
        fd->file = RWFileInterface.rwfopen(filename, mode);
        if (fd->file == NULL) {
            delete fd;
            m_Error = 6;
#ifdef VERSION_GPLP9G
            delete[] filename;
#endif
            return NULL;
        }

        fd->flags = (1 << 4) | (1 << 3);
        fd->filename = filename;
        fd->id = ++m_DescriptorIdCounter;
        AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).push_back(reinterpret_cast<unsigned long>(fd));
#ifdef VERSION_GPLP9G
        delete[] filename;
#endif
        return fd;
    } else {
        m_LastFileDescriptorUsed = NULL;
        const char* filename_ = (m_Flag & (1 << 0)) ? KeepOnlyFileName((char*)filename) : filename;
        if (m_Flag & (1 << 0)) {
            DkFileSysInfoFromFat info;
            FindFileOnFat(filename_, &info);
            if (info.unk8 != 0) {
                m_CurrentPackName = info.name;
                m_CurrentPackSize = info.size;
                m_OnFatPosition = info.unk18;

                fd = new DkFileSysDescriptor;

                DkFileSysDescriptor* unk = IsPackAllreadyOpened(m_CurrentPackName);
#ifdef VERSION_GPLE9G
                if (unk == NULL) {
                    unk = new DkFileSysDescriptor;
                    if (unk != NULL) {
                        unk->file = RWFileInterface.rwfopen(m_CurrentPackName, "rb");
                        unk->flags = (1 << 6) | (1 << 4) | (1 << 3);
                        unk->filename = filename_;
                        unk->pack_name = m_CurrentPackName;
                        unk->id = ++m_DescriptorIdCounter;

                        AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).push_back(reinterpret_cast<unsigned long>(unk));
                    }
                }

                if (unk == NULL || unk->file == NULL) {
                    delete fd;
                    if (unk != NULL) {
                        delete unk;
                    }
                    return NULL;
                }
#endif

                fd->file = unk->file;
                fd->flags = (1 << 7);
                fd->pack_offset = info.pack_offset;
                fd->compressed_size = info.compressed_size;
                fd->uncompressed_size = info.uncompressed_size;
                fd->filename = filename_;
                fd->pack_name = m_CurrentPackName;
                fd->zstream.zalloc = MyAlloc_func;
                fd->zstream.zfree = MyFree_func;
                fd->zstream.opaque = NULL;
                fd->zstream.total_out = 0;
                fd->zstream.next_out = NULL;
                fd->zstream.avail_out = 0;
                fd->zstream.next_in = NULL;
                fd->zstream.avail_in = 0;
                fd->unk68 = 0;
                // FIXME: need zlib 1.1.4
                inflateInit(&fd->zstream);
                fd->unk6C = NULL;
                fd->unk70 = 0;
                fd->unk74 = 0;
                fd->unk78 = 0;
                fd->id = ++m_DescriptorIdCounter;

                AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).push_back(reinterpret_cast<unsigned long>(unk));
            } else {
                m_Error = 8;
                fd = new DkFileSysDescriptor;
                fd->file = RWFileInterface.rwfopen(filename, "rb");
                if (fd->file == NULL) {
                    delete fd;
#ifdef VERSION_GPLP9G
                    delete[] filename;
#endif
                    return NULL;
                }

                fd->flags = (1 << 4) | (1 << 3);
                fd->filename = filename;
                fd->id = ++m_DescriptorIdCounter;

                AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).push_back(reinterpret_cast<unsigned long>(fd));
            }
        } else {
            fd = new DkFileSysDescriptor;
            fd->file = RWFileInterface.rwfopen(filename, "rb");
            if (fd->file == NULL) {
                delete fd;
#ifdef VERSION_GPLP9G
                delete[] filename;
#endif
                return NULL;
            }

            fd->flags = (1 << 4) | (1 << 3);
            fd->filename = filename;
            fd->id = ++m_DescriptorIdCounter;

            AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).push_back(reinterpret_cast<unsigned long>(fd));
#ifdef VERSION_GPLP9G
            delete[] filename;
#endif
        }
    }

    return fd;
}

int CDkFileSys::fclose(void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;

    if (fd->unk20 != NULL) {
        delete[] fd->unk20;
    }
    fd->unk20 = NULL;

    if (fd->unk24 != NULL) {
        delete[] fd->unk24;
    }
    fd->unk24 = NULL;

    if (m_Flag & (1 << 0) && fd->flags & (1 << 7)) {
        inflateEnd(&fd->zstream);
    }

    if (fd->unk6C != NULL) {
        delete[] fd->unk6C;
    }
    fd->unk6C = NULL;

    if (fd->unk7C != NULL) {
        delete[] fd->unk7C;
    }
    fd->unk7C = NULL;

    for (std::vector<DkFileSysDescriptor*>::iterator iter = m_pUnCompressedMemory.begin(); iter != m_pUnCompressedMemory.end(); iter++) {
        if (*iter == fd) {
            AS_ULONG_VECTOR_HACK(m_pUnCompressedMemory).erase(reinterpret_cast<unsigned long*>(iter));
            break;
        }
    }

    if (fd->flags & (1 << 4)) {
        RWFileInterface.rwfclose(fd->file);
    }

    delete fd;
    return 0;
}

size_t CDkFileSys::fread(void* ptr, size_t size, size_t n, void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;

    if (fd->flags & (1 << 3)) {
        return FREADOnDisk(ptr, size, n, fd);
    }

    if (fd->flags & (1 << 7)) {
        BOOL local_28;
        U32 unk = FREADInPackByBlock(ptr, size, n, fd, &local_28);
        if (local_28) {
            return unk;
        }
        return unk;
    } else {
        int position = fd->position;
        int size_in_bytes = size * n;
        int read_len;
        if (position + size_in_bytes > fd->uncompressed_size) {
            read_len = fd->uncompressed_size - position;
        } else {
            read_len = size_in_bytes;
        };
        memcpy(ptr, (void*)((int)fd->unk20 + position), read_len);
        fd->position += read_len;
        return read_len / size;
    }
}

size_t CDkFileSys::FREADOnDisk(void* ptr, size_t size, size_t n, DkFileSysDescriptor* fd) {
    if (m_LoadingCB != NULL) {
        U32 ret = 0;
        U32 uVar4 = m_SizeBetweenCBLaunch;
        U32 size_in_bytes = n * size;
        for (; ret < size_in_bytes; ret += uVar4) {
            U32 uVar1 = size_in_bytes - ret;
            if (uVar4 > uVar1) {
                uVar4 = uVar1;
            }
            uVar1 = RWFileInterface.rwfread((void*)((int)ptr + ret), 1, uVar4, fd->file);
            if (uVar1 < uVar4) {
                return (ret + uVar1) / size;
            }
        }
        return ret / size;
    } else {
        return RWFileInterface.rwfread(ptr, size, n, fd->file);
    }
}

char* CDkFileSys::fgets(char* dest, int size, void* fd_) {
    int i;
    char* ptr;
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;

    if (fd->flags & (1 << 3)) {
        return RWFileInterface.rwfgets(dest, size, fd->file);
    }

    i = 0;
    ptr = dest;
    do {
        if (fread(ptr, 1, 1, fd) == 0) {
            break;
        }

        ptr++;
        i++;
        if (ptr[-1] == '\n') {
            break;
        }
    } while (i + 1 < size);

    *ptr = '\0';
    return dest;
}

// Equivalent: unneeded downcast
int CDkFileSys::feof(void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;

    if (fd->flags & (1 << 3)) {
        return RWFileInterface.rwfeof(fd->file);
    }

    return fd->position >= fd->uncompressed_size;
}

int CDkFileSys::fseek(void* fd_, long offset, int whence) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;
    m_LastFileDescriptorUsed = NULL;

    if (fd->flags & (1 << 3)) {
        return RWFileInterface.rwfseek(fd->file, offset, whence);
    }

    switch (whence) {
        case SEEK_SET:
            if (offset > fd->uncompressed_size) {
                fd->position = fd->uncompressed_size;
                fd->unk78 = 0;
                return -1;
            }
            if (offset < 0) {
                fd->position = 0;
                if (fd->unk70 == 0) {
                    fd->unk74 = 0;
                } else {
                    fd->unk78 = 0;
                }
                return -1;
            }

            fd->position = offset;
            if (fd->unk6C != NULL) {
                if (fd->position >= fd->unk70 && fd->position < fd->unk70 + fd->unk78) {
                    fd->unk74 = fd->position - fd->unk70;
                } else {
                    fd->unk78 = 0;
                }
            }
            return 0;

        case SEEK_END:
            if (offset > fd->uncompressed_size) {
                fd->position = 0;
                if (fd->unk70 == 0) {
                    fd->unk74 = 0;
                } else {
                    fd->unk78 = 0;
                }
                return -1;
            }
            if (offset < 0) {
                fd->position = fd->uncompressed_size;
                fd->unk78 = 0;
                return -1;
            }

            fd->position = fd->uncompressed_size - offset;
            if (fd->unk6C != NULL) {
                if (fd->position >= fd->unk70 && fd->position < fd->unk70 + fd->unk78) {
                    fd->unk74 = fd->position - fd->unk70;
                } else {
                    fd->unk78 = 0;
                }
            }
            return 0;

        case SEEK_CUR:
            if (fd->position + offset > fd->uncompressed_size) {
                fd->position = fd->uncompressed_size;
                fd->unk78 = 0;
                return -1;
            }
            if (fd->position + offset < 0) {
                fd->position = 0;
                if (fd->unk70 == 0) {
                    fd->unk74 = 0;
                } else {
                    fd->unk78 = 0;
                }
                return -1;
            }

            fd->position += offset;
            if (fd->unk6C != NULL) {
                if (fd->position >= fd->unk70 && fd->position < fd->unk70 + fd->unk78) {
                    fd->unk74 = fd->position - fd->unk70;
                } else {
                    fd->unk78 = 0;
                }
            }
            return 0;
    }

    return -1;
}

long CDkFileSys::ftell(void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;

    if (fd->flags & (1 << 3)) {
        return RWFileInterface.rwftell(fd->file);
    }

    return fd->position;
}

int CDkFileSys::fflush(void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;
    return RWFileInterface.rwfflush(fd->file);
}

size_t CDkFileSys::fwrite(const void* ptr, unsigned long size, unsigned long n, void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;
    return RWFileInterface.rwfwrite(ptr, size, n, fd->file);
}

int CDkFileSys::fputs(const char* str, void* fd_) {
    DkFileSysDescriptor* fd = (DkFileSysDescriptor*)fd_;
    return RWFileInterface.rwfputs(str, fd->file);
}

void CDkFileSys::SetCallBackOnLoad(CDkFileSysLoadCallBack* callback, int size_between_load) {
    m_LoadingCB = callback;
    m_SizeBetweenCBLaunch = size_between_load;
    m_LoadingCumulator = 0;
}

void CDkFileSys::UnSetCallBackOnLoad() {
    m_LoadingCB = NULL;
}

void CDkFileSys::SetErrorCallBack(CDkFileSysErrorCallBack* callback) {
    m_ErrorCallBack = callback;
}

void CDkFileSys::UnSetErrorCallBack() {
    m_ErrorCallBack = NULL;
}
