#ifndef PIGLET_ENTITIES_CENTITY_H
#define PIGLET_ENTITIES_CENTITY_H

#include <string>
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityManager.h"
#include "entities/CEntityBhvTagBehavior.h"
#include "CMailBox.h"
#include "types.h"

#define ENTITY_FLAG_ACTIVE        (1 << 0)
#define ENTITY_FLAG_VISIBLE       (1 << 1)
#define ENTITY_FLAG_HAS_COLLISION (1 << 2)
#define ENTITY_FLAG_UNK3          (1 << 3)
#define ENTITY_FLAG_UNK4          (1 << 4)
#define ENTITY_FLAG_UNK5          (1 << 5)
#define ENTITY_FLAG_UNK6          (1 << 6)
#define ENTITY_FLAG_UNK7          (1 << 7)
#define ENTITY_FLAG_UNK8          (1 << 8) // Related to visibility

enum EEntityType {
    ENTITY_INVALID = 0,
    ENTITY_BASE = 1,
    ENTITY_OBJECT = 2,
    ENTITY_WORLD = 3,
    ENTITY_WARP = 4,
    ENTITY_UNK5 = 5,
    ENTITY_ZONE = 6,
    ENTITY_SEARCHABLE_ZONE = 7,
    ENTITY_SCARY_FACE_BOX = 8,
    ENTITY_UNK9 = 9,
    ENTITY_LIGHT = 10,
    ENTITY_TIMER = 11,
    ENTITY_RTC_PLAYER = 12,
    ENTITY_FX = 13,
    ENTITY_MESH = 14,
    ENTITY_PUSHING_BOX = 15,
    ENTITY_COOKIE = 16,
    ENTITY_HERO = 17,
    ENTITY_PIGLET = 18,
    ENTITY_WINNIE = 19,
    ENTITY_TIGGER = 20,
    ENTITY_NPC21 = 21, // Returned by CEntityManager::GetTypeFromString when you ask for "NPCEntity"
    ENTITY_NPC23 = 23, // Returned by CEntityNPC::GetType
    ENTITY_NPC_BASIC_EFFALUMP = 24,
    ENTITY_NPC_BASIC_WOOZLE = 25,
    ENTITY_NPC_TREE = 26,
    ENTITY_NPC_BEE_EFFALUMP = 27,
    ENTITY_NPC_HNS_WOOZLE = 28,
    ENTITY_NPC_MIRROR_WOOZLE = 29,
    ENTITY_NPC_ROAD_SWEEPER_EFFALUMP = 30,
    ENTITY_NPC_TUBA_EFFALUMP = 31,
    ENTITY_NPC_JACKPOT_EFFALUMP = 32,
    ENTITY_NPC_TENNIS_WOOZLE = 33,
    ENTITY_NPC_DOOR = 34,
    ENTITY_NPC_GRALOSAURUS = 35,
    ENTITY_UNK36 = 36,
    ENTITY_ESCAPING_OBJECT = 39,
    ENTITY_NPC_ARROW = 42,
    ENTITY_SEQ_KEY = 43,
    ENTITY_PHIAL = 44,
    ENTITY_PATH_FINDER = 45,
    ENTITY_3D_SOUND = 46,
};

class CEntity {
public:
    std::string m_unk0;
    U32 m_flags;
    U32 m_unk8;
    U32 m_unkC;
    U32 m_unk10;
    U32 m_unk14;
    CEntityBhvTagBehavior* m_unk18;
    CEntityManager* m_entity_manager;

public:
    CEntity(CEntityManager* entity_manager, std::string name);
    virtual ~CEntity();

    virtual BOOL IsFlagged(U32);
    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual void SetBehavior(U32 unk);
    virtual U32 GetType();
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt_maybe);
    virtual void Render(F32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void UpdateBehavior(F32 dt_maybe);
    virtual U32 GetSaveSize();
    virtual void Save(void*);
    virtual void Restore(void*);
    virtual void ManageMessage(SDkMessage& message);

    void ParseXYZ(DkXmd::CChunkIterator iter, F32* x, F32* y, F32* z);
    void ProcessMessages();
};
REQUIRE_SIZE(CEntity, 0x24);

#endif
