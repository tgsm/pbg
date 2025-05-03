#pragma once on

#include <string>
#include "engine/xmd/CChunkIterator.h"
#include "entities/CEntityManager.h"
#include "entities/CEntityBhvTagBehavior.h"
#include "types.h"

// FIXME: This is part of the engine, but where do we put it?
struct SDkMessage {
    char unk0[32];
    char unk20[16];
    u32 unk30;
};
REQUIRE_SIZE(SDkMessage, 0x34);

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
REQUIRE_SIZE(CEntity, 0x24);
