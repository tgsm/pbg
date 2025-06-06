#include "entities/CEntityNPCBasicWoozle.h"
#include <iostream>

CEntityNPCBasicWoozle::CEntityNPCBasicWoozle(CEntityManager* entity_manager, std::string name) : CEntityNPC(entity_manager, name) {
    SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
}

CEntityNPCBasicWoozle::~CEntityNPCBasicWoozle() {

}

void CEntityNPCBasicWoozle::Reset() {
    CEntityNPC::Reset();
}

void CEntityNPCBasicWoozle::UpdateDeathBehaviour(F32) {

}

void CEntityNPCBasicWoozle::Render(F32 dt_maybe) {
    if (IsFlagged(1 << 1) == TRUE) {
        CEntityNPC::Render(dt_maybe);
    }
}

void CEntityNPCBasicWoozle::Restore(void* unk) {
    CEntityNPC::Restore(unk);
}

void CEntityNPCBasicWoozle::Parse(DkXmd::CChunkIterator iter) {
    CEntityNPC::Parse(iter);
}

void CEntityNPCBasicWoozle::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityNPC::ParseBehavior(iter, behavior);
}
