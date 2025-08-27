#ifndef ENGINE_FILESYS_DKFILESYS_H
#define ENGINE_FILESYS_DKFILESYS_H

#include "types.h"

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

class CDkFileSys {
public:
    static void SetErrorCallBack(CDkFileSysErrorCallBack*);
    static void UnSetErrorCallBack();
    static void SetCallBackOnLoad(CDkFileSysLoadCallBack*, int);
    static void UnSetCallBackOnLoad();

    static void init(char* xmd, int, int unused);
    static void exit();
};

#endif
