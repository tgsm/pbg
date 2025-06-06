#include <string>
#include "entities/CEntityBhvTagData.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagAnim : public CEntityBhvTagData {
private:
    std::string m_animation_name;

public:
    CEntityBhvTagAnim();
    virtual ~CEntityBhvTagAnim();

    virtual void Set(CEntity* entity); // entity actually be derived from CEntity
    virtual U32 GetType() {
        return BEHAVIOR_TAG_ANIM;
    }

    void Parse(DkXmd::CChunkIterator);
};
REQUIRE_SIZE(CEntityBhvTagAnim, 0x10);
