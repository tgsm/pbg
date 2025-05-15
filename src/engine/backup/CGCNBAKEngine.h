#pragma once

#include <string>
#include "types.h"

namespace DKBAK {

class IEventBackupCB;
class DKBAK_DATE;

// FIXME: This has a parent class
class CGCNBAKEngine {
public:
    virtual void Initialize();
    virtual void SetGameTitle(std::string title);
    virtual void SetIconFile(std::string icon);
    virtual void SetEventCB(IEventBackupCB*);
    virtual u32 GetState();
    virtual u32 Format();
    virtual void FindFirst(std::string, std::string);
    virtual void FindNext();
    virtual void GetDate(std::string, std::string, DKBAK_DATE&);
    virtual void GetSize(std::string, std::string, int&);
    virtual u32 Delete(std::string, std::string);
    virtual BOOL IsSpaceAvailable(std::string, std::string, int);
    virtual int Load(void*, int, std::string, std::string);
    virtual int Save(void*, int, std::string, std::string);
};

}
