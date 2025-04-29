#pragma once on

#include "types.h"

class CEntity;

enum EBehaviorTagType {
    BEHAVIOR_TAG_0 = 0,
    BEHAVIOR_TAG_BEHAVIOR = 1,
    BEHAVIOR_TAG_POSITION = 2,
    BEHAVIOR_TAG_ORIENTATION = 3,
    BEHAVIOR_TAG_MESSAGE = 4,
    BEHAVIOR_TAG_RESPONSE = 5,
    BEHAVIOR_TAG_SEND_MESSAGE = 6,
    BEHAVIOR_TAG_VISIBILITY = 7,
    BEHAVIOR_TAG_COLLISION = 8,
    BEHAVIOR_TAG_ACTIVATION = 9,
    BEHAVIOR_TAG_TELEPORT = 10,
    BEHAVIOR_TAG_ANIM = 11,
    BEHAVIOR_TAG_CAMERA_SYSTEM = 12,
    BEHAVIOR_TAG_GUI_CONTROL = 13,
    BEHAVIOR_TAG_SETTINGS = 14,
    BEHAVIOR_TAG_BOX_POSITION = 15,
    BEHAVIOR_TAG_FINISH_MISSION = 16,
    BEHAVIOR_TAG_NARRATOR = 17,
    BEHAVIOR_TAG_RECEIVE_SHADOW = 18,
};

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
REQUIRE_SIZE(CEntityBhvTagData, 0xC);
