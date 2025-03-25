#pragma once

#include <string>
#include "types.h"

class CEntityManager;
class CEntityBhvTagBehavior;

class CEntity {
private:
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
    virtual void Parse(); // FIXME: Types
    virtual void ParseBehavior(); // FIXME: Types
    virtual void UpdateBehavior(f32 dt_maybe);
    virtual u32 GetSaveSize();
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ManageMessage(); // FIXME: Types
};
