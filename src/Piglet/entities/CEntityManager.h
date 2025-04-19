#pragma once on

#include <string>
#include <vector>
#include "engine/CChunkIterator.h"
#include "CGame.h"
#include "types.h"

class CEntity;

class CEntityManager {
private:
    CGame* m_game;
    std::vector<unsigned long> m_entities;

    // Unknown types
    u32 m_unk10;
    u32 m_unk14;
    DkXmd::CChunkIterator* m_unk18;
    u32 m_unk1C;
    u32 m_unk20;
    u32 m_entity_light_01; // ?

public:
    // FIXME: Make an enum for entity types

    CEntityManager(CGame* game);

    CGame* GetGame() { return m_game; }

    u32 GetTypeFromModel(std::string);
    u32 GetTypeFromString(std::string type_str);
    u32 CreateEntity(std::string str1, std::string type, std::string str3); // unknown return type
    u32 CreateEntity(std::string str1, u32 type, std::string str3); // unknown return type

    BOOL FindChunkModel(std::string str, DkXmd::CChunkIterator& iter);
    CEntity* GetEntity(std::string name);
    u32 GetEntityLight01();
    CEntity* GetEntity(u32 n);
    u32 GetEntityCount();

    CEntity* GetHero();
};
