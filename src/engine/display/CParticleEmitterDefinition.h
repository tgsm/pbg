#ifndef ENGINE_DISPLAY_CPARTICLEEMITTERDEFINITION_H
#define ENGINE_DISPLAY_CPARTICLEEMITTERDEFINITION_H

#include <string>
#include "engine/display/CRWStream.h"
#include "engine/display/IParticleEmitterDefinition.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "engine/wrap/DKW_V2d.h"
#include "engine/wrap/DKW_V3d.h"
#include "engine/xmd/CXmdFile.h"
#include "engine/xmd/CChunkIterator.h"

extern int dkdsp_stricmp(const char* a, const char* b);

namespace DKDSP {

struct PED_UnkSubstruct {
    U32 m_flags;
    U32 m_max_particles;
    U32 m_seed;
    F32 m_particle_emission_gap;
    F32 m_particle_emission_gap_bias;
    U32 m_particle_emission;
    U32 m_particle_emission_bias;
    U32 m_src_blend;
    U32 m_dest_blend;
    CDKW_V3d m_initial_direction;
    CDKW_V3d m_initial_direction_bias;
    CDKW_RGBAReal m_unk3C;
    CDKW_V3d m_emitter_size;
    char m_texture_id[32];
    CDKW_V2d m_unk78;
    CDKW_V3d m_force;
    F32 m_life_time;
    F32 m_life_time_bias;
    F32 m_initial_velocity;
    F32 m_initial_velocity_bias;
    CDKW_RGBAReal m_start_tex_coords;
    CDKW_RGBAReal m_unkAC; // start texcoords bias?
    CDKW_RGBAReal m_end_tex_coords;
    CDKW_RGBAReal m_unkCC; // end texcoords bias?
    CDKW_V2d m_start_size;
    CDKW_V2d m_start_size_bias;
    CDKW_V2d m_end_size;
    CDKW_V2d m_end_size_bias;
    CDKW_RGBAReal m_start_color;
    CDKW_RGBAReal m_start_color_bias;
    CDKW_RGBAReal m_end_color;
    CDKW_RGBAReal m_end_color_bias;
};

// TODO
class CParticleEmitterDefinition : public IParticleEmitterDefinition {
public:
    std::string m_name;
    DkXmd::CXmdFile* m_xmd;
    void* m_unkC;
    PED_UnkSubstruct m_unk10;

public:
    CParticleEmitterDefinition();
    ~CParticleEmitterDefinition();

    virtual std::string* GetName();
    virtual U32 GetSize();

    void Release();
    void SetName(std::string name);
    BOOL Load(IRWStream*);
    BOOL Parse();
    void SetupDefault();

    // Incomplete
    CDKW_V3d ParseVector3d(DkXmd::CChunkIterator* iter) {
        if (iter == NULL) {
            // return;
        }

        CDKW_V3d vec;

        DkXmd::CChunkIterator dest;
        if (iter->GetFirstChildChunk(dest)) {
            do {
                char* name_ = dest.GetName();

                if (dkdsp_stricmp(name_, "X") == 0) {
                    vec.m_x = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "Y") == 0) {
                    vec.m_y = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "Z") == 0) {
                    vec.m_z = dest.GetFloatValue();
                }
            } while (dest.GetNextSiblingChunk(dest));
        }
        return vec;
    }

    void ParseColor(DkXmd::CChunkIterator* iter, CDKW_RGBAReal* dest_color) {
        if (iter == NULL) {
            return;
        }

        if (dest_color == NULL) {
            return;
        }

        DkXmd::CChunkIterator dest;
        if (iter->GetFirstChildChunk(dest)) {
            do {
                char* name_ = dest.GetName();

                if (dkdsp_stricmp(name_, "R") == 0) {
                    dest_color->m_r = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "G") == 0) {
                    dest_color->m_g = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "B") == 0) {
                    dest_color->m_b = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "A") == 0) {
                    dest_color->m_a = dest.GetFloatValue();
                }
            } while (dest.GetNextSiblingChunk(dest));
        }
    }

    void ParseTexCoords(DkXmd::CChunkIterator* iter, CDKW_RGBAReal* dest_coords) {
        if (iter == NULL) {
            return;
        }

        if (dest_coords == NULL) {
            return;
        }

        DkXmd::CChunkIterator dest;
        if (iter->GetFirstChildChunk(dest)) {
            do {
                char* name_ = dest.GetName();

                if (dkdsp_stricmp(name_, "U1") == 0) {
                    dest_coords->m_r = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "V1") == 0) {
                    dest_coords->m_g = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "U2") == 0) {
                    dest_coords->m_b = dest.GetFloatValue();
                } else if (dkdsp_stricmp(name_, "V2") == 0) {
                    dest_coords->m_a = dest.GetFloatValue();
                }
            } while (dest.GetNextSiblingChunk(dest));
        }
    }
};
REQUIRE_SIZE(CParticleEmitterDefinition, 0x14C);

}

#endif
