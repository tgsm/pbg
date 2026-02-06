#ifndef ENGINE_BACKUP_IBAKENGINE_H
#define ENGINE_BACKUP_IBAKENGINE_H

#include <string>
#include "types.h"

class IEventBackupCB {
public:
    virtual ~IEventBackupCB() {}
    virtual void OnSave();
};

namespace DKBAK {

struct DKBAK_DATE {
    U8 unk0;
    U8 unk1;
    U8 unk2;
    U8 unk3;
    U8 unk4;
    U16 unk6;
};

class IBAKEngine {
public:
    virtual void Initialize() = 0;
    virtual void SetGameTitle(std::string title) = 0;
    virtual void SetIconFile(std::string icon) = 0;
    virtual void SetEventCB(IEventBackupCB*) = 0;
    virtual U32 GetState() = 0;
    virtual U32 Format() = 0;
    virtual std::string FindFirst(std::string, std::string) = 0;
    virtual std::string FindNext() = 0;
    virtual int GetDate(std::string, std::string, DKBAK_DATE&) = 0;
    virtual void GetSize(std::string, std::string, int&) = 0;
    virtual U32 Delete(std::string, std::string) = 0;
    virtual BOOL IsSpaceAvailable(std::string, std::string, int) = 0;
    virtual int Load(void*, int, std::string, std::string) = 0;
    virtual int Save(void*, int, std::string, std::string) = 0;
};

}

#endif
