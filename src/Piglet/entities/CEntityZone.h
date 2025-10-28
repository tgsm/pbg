#ifndef PIGLET_ENTITIES_CENTITYZONE_H
#define PIGLET_ENTITIES_CENTITYZONE_H

#include "engine/display/CParticleEmitter.h"
#include "engine/wrap/DKW_V3d.h"
#include "entities/CEntityObject.h"
#include "CGame.h"

class CEntityZone : public CEntityObject {
public:
    struct SFX {
        DKDSP::CParticleEmitter* emitter;
    };

    CDKW_V3d m_position;
    F32 m_unk4C;
    char m_unk50[16][16];
    U8 m_unk150[16];
    U8 m_unk160[16];
    SFX m_unk170;
    F32 m_unk174;
    F32 m_unk178;
    U8 m_unk17C[0x188 - 0x17C];
    SFX m_unk188;
    F32 m_unk18C;
    F32 m_unk190;
    U8 m_unk194[0x1A0 - 0x194];
    F32 m_unk1A0;
    F32 m_unk1A4;
    CDKW_V3d m_unk1A8;
    F32 m_unk1B4;
    U32 m_unk1B8;

public:
    CEntityZone(CEntityManager* entity_manager, std::string name);
    virtual ~CEntityZone();

    virtual void AddFlag(U32 flag);
    virtual void DelFlag(U32 flag);
    virtual U32 GetType() { return ENTITY_ZONE; }
    virtual void Init();
    virtual void Reset();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior);
    virtual void ManageMessage(SDkMessage& message);
    virtual CDKW_V3d GetPosition() { return m_position; }
    virtual void SetPosition(CDKW_V3d& position) { m_position = position; }
    virtual CDKW_V3d GetOrientation() { return CDKW_V3d(0.0f, m_unk4C, 0.0f); }
    virtual void SetOrientation(CDKW_V3d& orientation) { m_unk4C = orientation.m_y; }
    virtual F32 GetYOrientation() { return m_unk4C; }
    virtual void SetYOrientation(F32 y) { m_unk4C = y; }
    virtual CDKW_Matrix GetMatrix();
    virtual void ResolveContact(const DkPh::Collider::Body&, int, int);
    virtual BOOL ShouldKick();

    // May be incomplete
    void ParseZoneFxOut(DkXmd::CChunkIterator iter) {
        char buf1[128];
        char buf2[128];
        DkXmd::CChunkIterator dest1, dest2;

        if (iter.GetChunk("TexDict", dest1)) {
            strcpy(buf1, dest1.GetStringValue());
            m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(6, buf1);
        }

        if (iter.GetFirstChildChunk(dest1) == TRUE) {
            do {
                strcpy(buf2, dest1.GetName());

                if (strcmp(buf2, "ParticleEmitter") == 0) {
                    DKDSP::CParticleEmitter* emitter = m_unk188.emitter;
                    if (emitter == NULL) {
                        m_unk188.emitter = m_entity_manager->GetGame()->GetScene()->LoadParticleEmitter(m_unk0, &dest1);
                    } else {
                        emitter->LoadParameters(&dest1);
                    }
                } else if (strcmp(buf2, "ParticleEmitterFile") == 0) {
                    strcpy(buf1, dest1.GetStringValue());
                    ParseParticleEmitter(buf1, &m_unk188);
                }
            } while (dest1.GetNextSiblingChunk(dest1) == TRUE);
        }
    }

    // May be incomplete
    void ParseZoneFxIn(DkXmd::CChunkIterator iter) {
        char buf1[128];
        char buf2[128];
        DkXmd::CChunkIterator dest1, dest2;

        if (iter.GetChunk("TexDict", dest1)) {
            strcpy(buf1, dest1.GetStringValue());
            m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(6, buf1);
        }

        if (iter.GetFirstChildChunk(dest1) == TRUE) {
            do {
                strcpy(buf2, dest1.GetName());

                if (strcmp(buf2, "ParticleEmitter") == 0) {
                    DKDSP::CParticleEmitter* emitter = m_unk188.emitter;
                    if (emitter == NULL) {
                        m_unk188.emitter = m_entity_manager->GetGame()->GetScene()->LoadParticleEmitter(m_unk0, &dest1);
                    } else {
                        emitter->LoadParameters(&dest1);
                    }
                } else if (strcmp(buf2, "ParticleEmitterFile") == 0) {
                    strcpy(buf1, dest1.GetStringValue());
                    ParseParticleEmitter(buf1, &m_unk170);
                }
            } while (dest1.GetNextSiblingChunk(dest1) == TRUE);
        }
    }

    // May be incomplete
    void ParseZoneFx(DkXmd::CChunkIterator iter) {
        char buf[128];
        DkXmd::CChunkIterator dest;

        if (iter.GetFirstChildChunk(dest) == TRUE) {
            do {
                strcpy(buf, dest.GetName());

                if (strcmp(buf, "ZoneFxOut") == 0) {
                    ParseZoneFxOut(dest);
                } else if (strcmp(buf, "ZoneFxIn") == 0) {
                    ParseZoneFxIn(dest);
                } else if (strcmp(buf, "RadialVelocity") == 0) {
                    m_unk1A4 = dest.GetFloatValue();
                } else if (strcmp(buf, "Radius") == 0) {
                    m_unk1A0 = dest.GetFloatValue();
                } else if (strcmp(buf, "Position") == 0) {
                    ParseXYZ(dest, &m_unk1A8.m_x, &m_unk1A8.m_y, &m_unk1A8.m_z);
                }
            } while (dest.GetNextSiblingChunk(dest) == TRUE);
        }
    }

    BOOL ParseParticleEmitter(std::string filename, SFX*);
};
REQUIRE_SIZE(CEntityZone, 0x1BC);

#endif
