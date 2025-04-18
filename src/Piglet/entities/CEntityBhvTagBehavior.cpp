#include "entities/CEntityBhvTagActivation.h"
#include "entities/CEntityBhvTagAnim.h"
#include "entities/CEntityBhvTagBehavior.h"
#include "entities/CEntityBhvTagCollision.h"
#include "entities/CEntityBhvTagMessage.h"
#include "entities/CEntityBhvTagVisibility.h"
#include <cstring>
#include <iostream>

CEntityBhvTagBehavior::CEntityBhvTagBehavior() {
    m_id = 0;
    m_unk4 = NULL;
}

CEntityBhvTagBehavior::~CEntityBhvTagBehavior() {

}

void CEntityBhvTagBehavior::Set(CEntity* entity) {
    for (CEntityBhvTagData* data = m_unk4; data != NULL; data = data->m_unk0) {
        switch (data->GetType()) {
            case 0:
            case 1:
            case 4:
            case 5:
            case 6:
            case 10:
            case 12:
            case 13:
            case 14:
            case 16:
            case 17:
                break;
            default:
                data->Set(entity);
                break;
        }
    }
}

void CEntityBhvTagBehavior::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) != TRUE) {
        return;
    }

    while (TRUE) {
        strcpy(buf, dest.GetName());
        if (strcmp(buf, "Id") == 0) {
            m_id = dest.GetS32Value();
        } else if (strcmp(buf, "Message") == 0) {
            CEntityBhvTagMessage* message = new CEntityBhvTagMessage;
            message->Parse(dest);
            AddData(message);
        } else if (strcmp(buf, "Visibility") == 0) {
            CEntityBhvTagVisibility* visibility = new CEntityBhvTagVisibility;
            visibility->Parse(dest);
            AddData(visibility);
        } else if (strcmp(buf, "Activation") == 0) {
            CEntityBhvTagActivation* activation = new CEntityBhvTagActivation;
            activation->Parse(dest);
            AddData(activation);
        } else if (strcmp(buf, "Collision") == 0) {
            CEntityBhvTagCollision* collision = new CEntityBhvTagCollision;
            collision->Parse(dest);
            AddData(collision);
        } else if (strcmp(buf, "Anim") == 0) {
            CEntityBhvTagAnim* anim = new CEntityBhvTagAnim;
            anim->Parse(dest);
            AddData(anim);
        }

        if (dest.GetNextSiblingChunk(dest) != TRUE) {
            break;
        }
    }
}

// This feels like a fakematch
CEntityBhvTagBehavior* CEntityBhvTagBehavior::GetBehavior(u32 id) {
    CEntityBhvTagBehavior* bhv = this;
    if (bhv != NULL) {
        do {
            if (bhv->m_id == id) {
                return bhv;
            }
            bhv = (CEntityBhvTagBehavior*)bhv->m_unk0;
        } while (bhv != NULL);
    }

    return NULL;
}
