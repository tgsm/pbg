#ifndef ENGINE_BACKUP_IBAKENGINE_H
#define ENGINE_BACKUP_IBAKENGINE_H

#include <string>
#include "types.h"

class IEventBackupCB;

namespace DKBAK {

class DKBAK_DATE;

class IBAKEngine {
public:
    virtual void Initialize() = 0;
    virtual void SetGameTitle(std::string title) = 0;
    virtual void SetIconFile(std::string icon) = 0;
    virtual void SetEventCB(IEventBackupCB*) = 0;
    virtual void GetState() = 0;
    virtual U32 Format() = 0;
    virtual void FindFirst(std::string, std::string) = 0;
    virtual void FindNext() = 0;
    virtual void GetDate(std::string, std::string, DKBAK_DATE&) = 0;
    virtual void GetSize(std::string, std::string, int&) = 0;
    virtual U32 Delete(std::string, std::string) = 0;
    virtual BOOL IsSpaceAvailable(std::string, std::string, int) = 0;
    virtual int Load(void*, int, std::string, std::string) = 0;
    virtual int Save(void*, int, std::string, std::string) = 0;
};

}

#endif
