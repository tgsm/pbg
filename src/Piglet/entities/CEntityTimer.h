#ifndef PIGLET_ENTITIES_CENTITYTIMER_H
#define PIGLET_ENTITIES_CENTITYTIMER_H

#include "entities/CEntity.h"
#include "CGamePartIngame.h"

class CEntityTimer : public CEntity {
private:
    f32 m_duration;
    f32 m_elapsed;

public:
    CEntityTimer(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityTimer();

    virtual u32 GetType() {
        return ENTITY_TIMER;
    }
    virtual void Update(f32 dt_maybe);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void Save(void*);
    virtual void ManageMessage(SDkMessage& message);
};
REQUIRE_SIZE(CEntityTimer, 0x2C);

#endif
