#include "entities/CEntityObject.h"
#include "entities/CEntityBhvTagPosition.h"
#include "entities/CEntityBhvTagOrientation.h"
#include <cstring>
#include <iostream>

CEntityObject::CEntityObject(CEntityManager* entity_manager, std::string name) : CEntity(entity_manager, name) {
    m_unk24 = 0;
    m_unk28 = 0;
    m_unk2C = 0;

    AddFlag(1 << 2);

    m_unk30 = 3.4028235e38f;
    m_unk34 = 3.4028235e38f;
    m_unk38 = 3.4028235e38f;
    m_unk3C = 3.4028235e38f;
}

// Incomplete
CEntityObject::~CEntityObject() {
    m_unk28 = 0;
    if (m_unk2C != NULL) {
        delete m_unk2C;
    }
    m_unk2C = NULL;
    m_unk24 = 0;

    if (m_unk2C != NULL) {
        delete m_unk2C;
    }
}

void CEntityObject::Reset() {
    CEntity::Reset();
}

void CEntityObject::Update(f32 dt_maybe) {
    CEntity::Update(dt_maybe);
    if (IsFlagged(1 << 0) && IsFlagged(1 << 7) == TRUE) {
        // FIXME: Fakematch
        asm { cmpwi r3, 1 }
    }
}

void CEntityObject::Init() {
    CEntity::Init();
    UpdateVolumes();
}

void CEntityObject::Render(f32 dt_maybe) {
    if (!IsFlagged(1 << 8) && !IsFlagged(1 << 1)) {
        // FIXME: Fakematch
        asm { cmpwi r3, 0 }
    }
}

void CEntityObject::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    CEntity::Parse(iter);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "Collisions") == 0) {
                ParseCollisions(dest);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityObject::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    CEntity::ParseBehavior(iter, behavior);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "Position") == 0) {
                CEntityBhvTagPosition* position = new CEntityBhvTagPosition;
                position->Parse(dest);
                behavior->AddData(position);
            } else if (strcmp(buf, "Orientation") == 0) {
                CEntityBhvTagOrientation* orientation = new CEntityBhvTagOrientation;
                orientation->Parse(dest);
                behavior->AddData(orientation);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}
