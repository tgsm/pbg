#ifndef ENGINE_BACKUP_CGCNBAKENGINE_H
#define ENGINE_BACKUP_CGCNBAKENGINE_H

#include "engine/backup/IBAKEngine.h"

namespace DKBAK {

class CGCNBAKEngine : public IBAKEngine {
public:
    int m_channel;
    int m_unk8;
    long m_memory_size;
    long m_sector_size;
    U8 m_unk14[0xA0 - 0x14];
    char m_game_title[32];
    U8 m_unkC0[0x3B00 - 0xC0];

public:
    CGCNBAKEngine();
    virtual ~CGCNBAKEngine();

    virtual void Initialize();
    virtual void SetGameTitle(std::string title);
    virtual void SetIconFile(std::string icon);
    virtual void SetEventCB(IEventBackupCB*);
    virtual U32 GetState();
    virtual U32 Format();
    virtual std::string FindFirst(std::string, std::string);
    virtual void FindNext();
    virtual void GetDate(std::string, std::string, DKBAK_DATE&);
    virtual void GetSize(std::string, std::string, int&);
    virtual U32 Delete(std::string, std::string);
    virtual BOOL IsSpaceAvailable(std::string, std::string, int);
    virtual int Load(void*, int, std::string, std::string);
    virtual int Save(void*, int, std::string, std::string);

    U32 CheckState();
    void GetCheckSum(void*, int);
};
REQUIRE_SIZE(CGCNBAKEngine, 0x3B00);

}

#endif
