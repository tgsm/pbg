#include <string>
#include "entities/CEntityBhvTagData.h"
#include "engine/xmd/CChunkIterator.h"

class CEntityBhvTagAnim : public CEntityBhvTagData {
private:
    std::string m_unkC;

public:
    CEntityBhvTagAnim();
    virtual ~CEntityBhvTagAnim();

    virtual void Set(CEntity* entity); // entity actually be derived from CEntity
    virtual u32 GetType() {
        return 11;
    }

    void Parse(DkXmd::CChunkIterator);
};
REQUIRE_SIZE(CEntityBhvTagAnim, 0x10);
