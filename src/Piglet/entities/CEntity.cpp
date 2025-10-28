#include "entities/CEntity.h"
#include "entities/CEntityBhvTagMessage.h"
#include "CGame.h"
#include "CMailBox.h"
#include <cstring>
#include <iostream>

CEntity::CEntity(CEntityManager* entity_manager, std::string name) {
    m_unk0.assign(name, 0);
    m_flags = 3;
    m_unk10 = 0;
    m_unk14 = 0;
    m_unkC = 0;
    m_unk8 = 0;
    m_unk18 = NULL;
    m_entity_manager = entity_manager;
}

CEntity::~CEntity() {
    if (m_unk18 != NULL) {
        delete m_unk18;
        m_unk18 = NULL;
    }
}

void CEntity::Init() {

}

void CEntity::Reset() {

}

void CEntity::Update(F32 dt_maybe) {
    ProcessMessages();
}

void CEntity::Render(F32 dt_maybe) {

}

void CEntity::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "StartBehavior") == 0) {
                m_unk10 = dest.GetS32Value();
                m_unk8 = m_unk10;
                m_unkC = m_unk10;
                m_unk14 = m_unk10;
            } else if (strcmp(buf, "Behavior") == 0) {
                CEntityBhvTagBehavior* behavior = new CEntityBhvTagBehavior;
                ParseBehavior(dest, behavior);
                if (m_unk18 == NULL) {
                    m_unk18 = behavior;
                } else {
                    m_unk18->AddBehavior(behavior);
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntity::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    behavior->Parse(iter);
}

void CEntity::ParseXYZ(DkXmd::CChunkIterator iter, F32* x, F32* y, F32* z) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    *x = 0.0f;
    *y = 0.0f;
    *z = 0.0f;

    if (iter.GetFirstChildChunk(dest)) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "X") == 0) {
                *x = dest.GetFloatValue();
            } else if (strcmp(buf, "Y") == 0) {
                *y = dest.GetFloatValue();
            } else if (strcmp(buf, "Z") == 0) {
                *z = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest));
    }
}

U32 CEntity::GetSaveSize() {
    return 12; // FIXME: This is probably calculated
}

void CEntity::SetBehavior(U32 unk) {
    if (m_unk18 != NULL && m_unk18->GetBehavior(unk) != NULL) {
        m_unk10 = unk;
        m_unk18->GetBehavior(m_unk10)->Set(this);
    }
}

// Equivalent: regalloc, std::basic_string::operator== fixme
void CEntity::ProcessMessages() {
    CMailBox* mailbox = m_entity_manager->GetGame()->GetMailbox();

    SDkMessage message;
    while (mailbox->GetMessage(&message, m_unk0, 1) != 0) {
        CEntityBhvTagBehavior* behavior = m_unk18->GetBehavior(m_unk10);
        if (behavior != NULL) {
            for (CEntityBhvTagBehavior* bhv = (CEntityBhvTagBehavior*)behavior->m_unk4; bhv != NULL; bhv = (CEntityBhvTagBehavior*)bhv->m_unk0) {
                if (bhv->GetType() == BEHAVIOR_TAG_MESSAGE) {
                    CEntityBhvTagMessage* bhv_message = ((CEntityBhvTagMessage*)bhv);
                    if (bhv_message->m_unkC == message.unk0 || (message.unk0 == "" && m_unk0 == bhv_message->m_unkC)) {
                        if (bhv_message->m_unk10 == message.unk20) {
                            bhv_message->Set(this);
                        }
                    }
                }
            }
        }
        ManageMessage(message);
    }

    if (m_unk14 != m_unk10) {
        SetBehavior(m_unk14);
    }
}

void CEntity::ManageMessage(SDkMessage& message) {
    if (strcmp(message.unk20, "VISIBILITY") == 0) {
        if (!message.unk30) {
            DelFlag(1 << 1);
        } else {
            AddFlag(1 << 1);
        }
    } else if (strcmp(message.unk20, "ACTIVATION") == 0) {
        if (!message.unk30) {
            DelFlag(1 << 0);
        } else {
            AddFlag(1 << 0);
        }
    } else if (strcmp(message.unk20, "COLLISION") == 0) {
        if (!message.unk30) {
            DelFlag(1 << 2);
        } else {
            AddFlag(1 << 2);
        }
    } else if (strcmp(message.unk20, "SET_BEHAVIOR") == 0) {
        m_unk14 = message.unk30;
    } else if (strcmp(message.unk20, "SAVE_BEHAVIOR") == 0) {
        m_unkC = message.unk30;
    } else if (strcmp(message.unk20, "FIX_BEHAVIOR") == 0) {
        m_unk14 = message.unk30;
        m_unkC = message.unk30;
    }
}

void CEntity::UpdateBehavior(F32 dt_maybe) {

}
