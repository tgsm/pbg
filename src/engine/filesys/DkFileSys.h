#ifndef ENGINE_FILESYS_DKFILESYS_H
#define ENGINE_FILESYS_DKFILESYS_H

#include <rwsdk/badevice.h>
#include <rwsdk/tool/png/zlib/zlib.h>
#include <vector>
#include "engine/xmd/CXmdFile.h"

// FIXME: Do these classes belong here?
class CGame;

class CDkFileSysErrorCallBack {
public:
    CDkFileSysErrorCallBack() {}
    virtual U32 Error(U32) = 0;
    virtual ~CDkFileSysErrorCallBack() {}

public:
    CGame* m_game;
    U32 m_unk8;
};

class CDkFileSysLoadCallBack {
public:
    CDkFileSysLoadCallBack() {}
    virtual void Update() = 0;
    virtual ~CDkFileSysLoadCallBack() {}
};

struct DkFileSysDescriptor {
    // FIXME: This is defined in DkFileSys.cpp on PAL
    DkFileSysDescriptor() {
        flags = 0;
        id = 0;
        position = 0;
        pack_offset = 0;
        compressed_size = 0;
        uncompressed_size = 0;
        unk18 = 0;
        filename = NULL;
        unk20 = NULL;
        unk24 = NULL;
        file = NULL;
        pack_name = NULL;
        unk68 = 0;
        unk6C = NULL;
        unk70 = 0;
        unk74 = 0;
        unk78 = 0;
        unk7C = NULL;
        unk80 = 0;
    }

    int flags;
    U32 id;
    int position;
    U32 pack_offset;
    int compressed_size;
    int uncompressed_size;
    U32 unk18;
    const char* filename;
    void* unk20;
    U8* unk24;
    void* file;
    char* pack_name;
    z_stream zstream;
    U32 unk68;
    void* unk6C;
    int unk70;
    int unk74;
    int unk78;
    U8* unk7C; // Unknown type
    U32 unk80;
}; // size: 0x84

struct DkFileSysInfoFromFat {
    char* name;
    int size;
    int unk8;
    int pack_offset;
    int compressed_size;
    int uncompressed_size;
    DkXmd::CChunkIterator unk18;
};

// Not in USA version
// TODO
struct SDkFileSysAlias {
    void* unk0;
    void* unk4;
};

class CDkFileSys {
public:
    static std::vector<DkFileSysDescriptor*> m_pUnCompressedMemory;
    static DkXmd::CChunkIterator m_OnFatPosition;
#ifdef VERSION_GPLP9G
    static std::vector<SDkFileSysAlias> m_vAliases;
#endif

    static int m_DescriptorIdCounter;
    static DkXmd::CXmdFile* m_Fat;
    static U32 m_FatSize;
    static U8* m_FatMemEntry;
    static char* m_CurrentPackName;
    static int m_CurrentPackSize;
    static int m_Flag;
    static int m_Error;
    static BOOL m_NoCompression;
    static CDkFileSysLoadCallBack* m_LoadingCB;
    static size_t m_SizeBetweenCBLaunch;
    static int m_LoadingCumulator;
    static CDkFileSysErrorCallBack* m_ErrorCallBack;
    static DkFileSysDescriptor* m_LastFileDescriptorUsed;
    static void* m_ReadBuffer; // Unknown type

    static int m_LastPointerOnPack;

public:
    static void SetErrorCallBack(CDkFileSysErrorCallBack*);
    static void UnSetErrorCallBack();
    static void SetCallBackOnLoad(CDkFileSysLoadCallBack*, int);
    static void UnSetCallBackOnLoad();

    static BOOL init(char* xmd_path, int flags, int unused);
    static BOOL exit();
    static int fexist(const char* filename);
    static void* fopen(const char* filename, const char* mode);
    static int fclose(void* file);
    static size_t fread(void* ptr, size_t size, size_t n, void* file);
    static size_t FREADOnDisk(void* ptr, size_t size, size_t n, DkFileSysDescriptor* file);
    static char* fgets(char* dest, int size, void* file);
    static int feof(void* file);
    static int fseek(void* file, long offset, int whence);
    static long ftell(void* file);
    static int fflush(void* file);
    static unsigned long fwrite(const void* ptr, unsigned long size, unsigned long n, void* file);
    static int fputs(const char* str, void* file);

    static BOOL loadfat(char* path, DkXmd::CXmdFile* xmd);
    static BOOL FindFileOnFatBegeningOn(DkXmd::CChunkIterator&, const char*, DkXmd::CChunkIterator&, DkFileSysInfoFromFat*);
    static BOOL FindFileOnFat(const char*, DkFileSysInfoFromFat*);
    static DkFileSysDescriptor* IsPackAllreadyOpened(char*);
    static char* KeepOnlyFileName(char* path);
    static int FREADInPackByBlock(void*, unsigned long, unsigned long, DkFileSysDescriptor*, int*);

    static int UncompressByBlock(U8*, U32*, U8*, U32, void*, int);
    static int UncompressNewBlock(DkFileSysDescriptor* file, int);
};

extern RwFileFunctions RWFileInterface;

#endif
