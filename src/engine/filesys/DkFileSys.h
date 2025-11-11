#ifndef ENGINE_FILESYS_DKFILESYS_H
#define ENGINE_FILESYS_DKFILESYS_H

#include <rwsdk/badevice.h>
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
    int m_unk0;
    U8 m_unk4[4];
    int m_unk8;
    U8 m_unkC[4];
    int m_unk10;
    int m_unk14;
    U8 m_unk18[8];
    void* m_unk20;
    U8 m_unk24[8];
    char* m_unk2C;
    U8 m_unk30[0x6C - 0x30];
    void* m_unk6C;
    U8 m_unk70[4];
    int m_unk74;
    U32 m_unk78;
    U8 m_unk7C[0x84 - 0x7C];
}; // size: 0x84

struct DkFileSysInfoFromFat {
    char* name;
    int offset;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    DkXmd::CChunkIterator unk18;
};

class CDkFileSys {
public:
    static DkXmd::CXmdFile* m_Fat;
    static U32 m_FatSize;
    static U8* m_FatMemEntry;
    static char* m_CurrentPackName;
    static int m_CurrentPackSize;
    static int m_Error;

    static std::vector<DkFileSysDescriptor*> m_pUnCompressedMemory;
    static DkXmd::CChunkIterator m_OnFatPosition;

public:
    static void SetErrorCallBack(CDkFileSysErrorCallBack*);
    static void UnSetErrorCallBack();
    static void SetCallBackOnLoad(CDkFileSysLoadCallBack*, int);
    static void UnSetCallBackOnLoad();

    static void init(char* xmd, int, int unused);
    static void exit();

    static BOOL loadfat(char* path, DkXmd::CXmdFile* xmd);
    static BOOL FindFileOnFatBegeningOn(DkXmd::CChunkIterator&, const char*, DkXmd::CChunkIterator&, DkFileSysInfoFromFat*);
    static BOOL FindFileOnFat(const char*, DkFileSysInfoFromFat*);
    static DkFileSysDescriptor* IsPackAllreadyOpened(char*);
    static unsigned char* KeepOnlyFileName(char* path);
    static int FREADInPackByBlock(void*, unsigned long, unsigned long, DkFileSysDescriptor*, int*);

    static int UncompressByBlock(U8*, U32*, U8*, U32, void*, int);
    static int UncompressNewBlock(DkFileSysDescriptor* file, int);
};

extern RwFileFunctions RWFileInterface;

#endif
