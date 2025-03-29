#pragma once

#include "types.h"

class CEntity;

class CEntityBhvTagData {
public:
    CEntityBhvTagData* m_unk0;
    CEntityBhvTagData* m_unk4;

public:
    CEntityBhvTagData();
    virtual ~CEntityBhvTagData();

    virtual void Set(CEntity* entity) = 0;
    virtual u32 GetType() = 0;

    void AddBehavior(CEntityBhvTagData* unk);
    void AddData(CEntityBhvTagData* unk);
};
