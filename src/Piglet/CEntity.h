#pragma once on

#include <string>
#include "engine/CChunkIterator.h"
#include "CEntityManager.h"
#include "CEntityBhvTagBehavior.h"
#include "types.h"

// FIXME: This is part of the engine, but where do we put it?
struct SDkMessage {
    char unk0[32];
    char unk20[16];
    u32 unk30;
}; // size: 0x34

class CEntity {
public:
    std::string m_unk0;
    u32 m_flags;
    u32 m_unk8;
    u32 m_unkC;
    u32 m_unk10;
    u32 m_unk14;
    CEntityBhvTagBehavior* m_unk18;
    CEntityManager* m_entity_manager;

public:
    CEntity(CEntityManager* entity_manager, std::string name);
    virtual ~CEntity();

    virtual BOOL IsFlagged(u32);
    virtual void AddFlag(u32 flag);
    virtual void DelFlag(u32 flag);
    virtual void SetBehavior(u32 unk);
    virtual u32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void UpdateBehavior(f32 dt_maybe);
    virtual u32 GetSaveSize();
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ManageMessage(SDkMessage& message);

    void ParseXYZ(DkXmd::CChunkIterator iter, f32* x, f32* y, f32* z);
    void ProcessMessages();
};
