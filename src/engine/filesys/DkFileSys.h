#ifndef ENGINE_FILESYS_DKFILESYS_H
#define ENGINE_FILESYS_DKFILESYS_H

#include <rwsdk/badevice.h>
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

// Not an official name
struct DkFile {
    U8 m_unk0[0x84];
}; // size: 0x84

class CDkFileSys {
public:
    static U32 m_FatSize;
    static U8* m_FatMemEntry;

public:
    static void SetErrorCallBack(CDkFileSysErrorCallBack*);
    static void UnSetErrorCallBack();
    static void SetCallBackOnLoad(CDkFileSysLoadCallBack*, int);
    static void UnSetCallBackOnLoad();

    static void init(char* xmd, int, int unused);
    static void exit();

    static BOOL loadfat(char* path, DkXmd::CXmdFile* xmd);
    static unsigned char* KeepOnlyFileName(char* path);
};

extern RwFileFunctions RWFileInterface;

#endif
