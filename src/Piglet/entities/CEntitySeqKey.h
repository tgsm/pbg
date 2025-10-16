#ifndef PIGLET_ENTITIES_CENTITY_SEQKEY_H
#define PIGLET_ENTITIES_CENTITY_SEQKEY_H

#include "engine/display/IParticleEmitter.h"
#include "entities/CEntityMesh.h"
#include "entities/CEntityObject.h"

class CEntitySeqKey : public CEntityObject {
public:
    U8 m_unk40;
    U8 m_unk41[0x54 - 0x41];
    F32 m_unk54;
    U8 m_unk58[0x148E4 - 0x58];
    F32 m_unk148E4;
    int m_unk148E8;
    U8 m_unk148EC[0x14908 - 0x148EC];

public:
    CEntitySeqKey(CEntityManager* entity_manager, std::string name);

    void CreateRoadSweeperArrivingFX(CEntityMesh*, DKDSP::IParticleEmitter*);
    void CreateRoadSweeperDustFX(CEntityMesh*, DKDSP::IParticleEmitter*);
    void StopRoadSweeperFX();
    void RotateOneRandomKey();
    void Explode();
    void UnExplode();
};
REQUIRE_SIZE(CEntitySeqKey, 0x14908);

#endif
