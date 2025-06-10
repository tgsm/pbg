#ifndef PIGLET_ENTITIES_CENTITYMANAGER_H
#define PIGLET_ENTITIES_CENTITYMANAGER_H

#include <string>
#include <vector>
#include "engine/xmd/CChunkIterator.h"
#include "engine/xmd/CXmdFile.h"
#include "CGame.h"
#include "types.h"

class CEntity;
class CEntityHero;

namespace DkPh {
class Collider;
}

class CEntityManager {
private:
    CGame* m_game;
    std::vector<unsigned long> m_entities;

    void* m_model_xmd_data;
    DkXmd::CXmdFile* m_model_xmd;
    DkXmd::CChunkIterator* m_model_xmd_iter;
    // Unknown types
    DkPh::Collider* m_unk1C;
    U32 m_unk20;
    CEntity* m_entity_light_01;

public:
    CEntityManager(CGame* game);
    ~CEntityManager();

    CGame* GetGame() { return m_game; }
    DkPh::Collider* GetUnkCollider() { return m_unk1C; }

    U32 GetTypeFromModel(std::string);
    U32 GetTypeFromString(std::string type_str);
    BOOL CreateEntity(std::string name, std::string type, std::string model);
    BOOL CreateEntity(std::string name, U32 type, std::string model);
    BOOL DestroyEntity(std::string name);
    BOOL DestroyEntity(U32 index);
    BOOL SetModelFile(std::string model);
    BOOL FindChunkModel(std::string str, DkXmd::CChunkIterator& iter);
    BOOL Parse(DkXmd::CChunkIterator iter);
    CEntity* GetEntity(std::string name);
    CEntity* GetEntityLight01();
    CEntity* GetEntity(U32 n);
    U32 GetEntityCount();

    CEntityHero* GetHero();
};
REQUIRE_SIZE(CEntityManager, 0x28);

#endif
