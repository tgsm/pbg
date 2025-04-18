#include <string>
#include "entities/CEntityBhvTagData.h"
#include "engine/CChunkIterator.h"

class CEntityBhvTagAnim : public CEntityBhvTagData {
public:
    CEntityBhvTagAnim();
    virtual ~CEntityBhvTagAnim();

    virtual void Set(CEntity* entity); // entity actually be derived from CEntity
    virtual u32 GetType() {
        return 11;
    }

    void Parse(DkXmd::CChunkIterator);

private:
    std::string m_unkC;
};
