#include <cstring>
#include <rwsdk/badevice.h>
#include "engine/display/CParticleEmitterDefinition.h"
#include "wrap/DKW_V3d.h"
#include "xmd/CChunkIterator.h"

namespace DKDSP {

CParticleEmitterDefinition::CParticleEmitterDefinition() : m_name("") {
    m_xmd = NULL;
    m_unkC = NULL;

    SetupDefault();
}

CParticleEmitterDefinition::~CParticleEmitterDefinition() {
    Release();
}

void CParticleEmitterDefinition::Release() {
    if (m_xmd != NULL) {
        delete m_xmd;
    }
    m_xmd = NULL;

    if (m_unkC != NULL) {
        RwEngineInstance->unk134(m_unkC);
    }
    m_unkC = NULL;

    SetupDefault();
}

std::string* CParticleEmitterDefinition::GetName() {
    return &m_name;
}

void CParticleEmitterDefinition::SetName(std::string name) {
    m_name.assign(name, 0);
}

U32 CParticleEmitterDefinition::GetSize() {
    return sizeof(CParticleEmitterDefinition) + 0x18; // is this right?
}

BOOL CParticleEmitterDefinition::Load(IRWStream* stream) {
    if (stream == NULL) {
        return FALSE;
    }

    Release();

    U32 unk;
    U32 iVar3 = 0;
    do {
        m_unkC = RwEngineInstance->unk138(m_unkC, iVar3 + 0x200);
        if (m_unkC == NULL) {
            return FALSE;
        }
        unk = stream->Read((void*)(*(int*)&m_unkC + iVar3), 0x200);
        iVar3 += unk;
    } while (unk == 0x200);


    m_xmd = new DkXmd::CXmdFile;
    if (m_xmd == NULL) {
        RwEngineInstance->unk134(m_unkC);
        m_unkC = NULL;
        return FALSE;
    }

    m_xmd->Parse(m_unkC);
    Parse();

    delete m_xmd;
    m_xmd = NULL;
    RwEngineInstance->unk134(m_unkC);
    m_unkC = NULL;

    return TRUE;
}

// Incomplete
BOOL CParticleEmitterDefinition::Parse() {
    if (m_xmd == NULL) {
        return FALSE;
    }

    DkXmd::CChunkIterator* iter = &m_xmd->m_chunk_iterator;
    SetupDefault();

    if (dkdsp_stricmp(iter->GetName(), "ParticleEmitter") != 0) {
        return FALSE;
    }

    DkXmd::CChunkIterator dest;
    if (!iter->GetFirstChildChunk(dest)) {
        return FALSE;
    }

    m_unk10.m_max_particles = 100;
    m_unk10.m_flags = 0;

    do {
        char* name = dest.GetName();

        if (dkdsp_stricmp(name, "MaximumParticles") == 0) {
            m_unk10.m_max_particles = dest.GetS32Value();
            if (m_unk10.m_max_particles == 0) {
                m_unk10.m_max_particles = 1;
            }
        } else if (dkdsp_stricmp(name, "AnimatedSize") == 0) {
            if (dest.GetS32Value() != 0) {
                m_unk10.m_flags |= (1 << 2);
            }
        } else if (dkdsp_stricmp(name, "AnimatedColor") == 0) {
            if (dest.GetS32Value() != 0) {
                m_unk10.m_flags |= (1 << 0);
            }
        } else if (dkdsp_stricmp(name, "AnimatedTexCoords") == 0) {
            if (dest.GetS32Value() != 0) {
                // unused, they may have compared to 0 here?
            }
        } else if (dkdsp_stricmp(name, "EmitterSize") == 0) {
            m_unk10.m_emitter_size = ParseVector3d(&dest);
        } else if (dkdsp_stricmp(name, "ParticleEmission") == 0) {
            m_unk10.m_particle_emission = dest.GetS32Value();
        } else if (dkdsp_stricmp(name, "ParticleEmissionBias") == 0) {
            m_unk10.m_particle_emission_bias = dest.GetS32Value();
        } else if (dkdsp_stricmp(name, "ParticleEmissionGap") == 0) {
            m_unk10.m_particle_emission_gap = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "ParticleEmissionGapBias") == 0) {
            m_unk10.m_particle_emission_gap_bias = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "Seed") == 0) {
            m_unk10.m_seed = dest.GetS32Value();
        } else if (dkdsp_stricmp(name, "LifeTime") == 0) {
            m_unk10.m_life_time = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "LifeTimeBias") == 0) {
            m_unk10.m_life_time_bias = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "TextureId") == 0) {
            char* id = dest.GetStringValue();
            if (id != NULL) {
                strcpy(m_unk10.m_texture_id, id);
            } else {
                m_unk10.m_texture_id[0] = '\0';
            }
        } else if (dkdsp_stricmp(name, "InitialVelocity") == 0) {
            m_unk10.m_initial_velocity = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "InitialVelocityBias") == 0) {
            m_unk10.m_initial_velocity_bias = dest.GetFloatValue();
        } else if (dkdsp_stricmp(name, "InitialDirection") == 0) {
            m_unk10.m_initial_direction = ParseVector3d(&dest);
        } else if (dkdsp_stricmp(name, "InitialDirectionBias") == 0) {
            m_unk10.m_initial_direction_bias = ParseVector3d(&dest);
        } else if (dkdsp_stricmp(name, "Force") == 0) {
            m_unk10.m_force = ParseVector3d(&dest);
        } else if (dkdsp_stricmp(name, "StartColor") == 0) {
            ParseColor(&dest, &m_unk10.m_start_color);
            m_unk10.m_unk3C = m_unk10.m_start_color;
        } else if (dkdsp_stricmp(name, "StartColorBias") == 0) {
            ParseColor(&dest, &m_unk10.m_start_color_bias);
        } else if (dkdsp_stricmp(name, "EndColor") == 0) {
            ParseColor(&dest, &m_unk10.m_end_color);
        } else if (dkdsp_stricmp(name, "EndColorBias") == 0) {
            ParseColor(&dest, &m_unk10.m_end_color_bias);
        } else if (dkdsp_stricmp(name, "StartSize") == 0) {
            CDKW_V3d size = ParseVector3d(&dest);
            m_unk10.m_start_size.m_x = size.m_x;
            m_unk10.m_start_size.m_y = size.m_y;
            m_unk10.m_unk78.m_x = size.m_x;
            m_unk10.m_unk78.m_y = size.m_y;
        } else if (dkdsp_stricmp(name, "StartSizeBias") == 0) {
            CDKW_V3d size = ParseVector3d(&dest);
            m_unk10.m_start_size_bias.m_x = size.m_x;
            m_unk10.m_start_size_bias.m_y = size.m_y;
        } else if (dkdsp_stricmp(name, "EndSize") == 0) {
            CDKW_V3d size = ParseVector3d(&dest);
            m_unk10.m_end_size.m_x = size.m_x;
            m_unk10.m_end_size.m_y = size.m_y;
        } else if (dkdsp_stricmp(name, "EndSizeBias") == 0) {
            CDKW_V3d size = ParseVector3d(&dest);
            m_unk10.m_end_size_bias.m_x = size.m_x;
            m_unk10.m_end_size_bias.m_y = size.m_y;
        } else if (dkdsp_stricmp(name, "StartTexCoords") == 0) {
            ParseTexCoords(&dest, &m_unk10.m_start_tex_coords);
        } else if (dkdsp_stricmp(name, "EndTexCoords") == 0) {
            ParseTexCoords(&dest, &m_unk10.m_end_tex_coords);
        } else if (dkdsp_stricmp(name, "SRCBLEND") == 0) {
            U32 blend;
            char* str = dest.GetStringValue();
            if (str == NULL) {
                blend = 1;
            } else if (dkdsp_stricmp(str, "ZERO") == 0) {
                blend = 1;
            } else if (dkdsp_stricmp(str, "ONE") == 0) {
                blend = 2;
            } else if (dkdsp_stricmp(str, "SRCALPHA") == 0) {
                blend = 5;
            } else if (dkdsp_stricmp(str, "INVSRCALPHA") == 0) {
                blend = 6;
            } else {
                blend = 1;
            }
            m_unk10.m_src_blend = blend;
        } else if (dkdsp_stricmp(name, "DESTBLEND") == 0) {
            U32 blend;
            char* str = dest.GetStringValue();
            if (str == NULL) {
                blend = 1;
            } else if (dkdsp_stricmp(str, "ZERO") == 0) {
                blend = 1;
            } else if (dkdsp_stricmp(str, "ONE") == 0) {
                blend = 2;
            } else if (dkdsp_stricmp(str, "SRCALPHA") == 0) {
                blend = 5;
            } else if (dkdsp_stricmp(str, "INVSRCALPHA") == 0) {
                blend = 6;
            } else {
                blend = 1;
            }
            m_unk10.m_dest_blend = blend;
        }
    } while (dest.GetNextSiblingChunk(dest));

    return TRUE;
}

void CParticleEmitterDefinition::SetupDefault() {
    memset(&m_unk10, 0, sizeof(PED_UnkSubstruct));

    m_unk10.m_flags = 0;
    m_unk10.m_max_particles = 0;
    m_unk10.m_seed = 0;
    m_unk10.m_particle_emission_gap = 0.0f;
    m_unk10.m_particle_emission_gap_bias = 0.0f;
    m_unk10.m_particle_emission = 0;
    m_unk10.m_particle_emission_bias = 0;
    m_unk10.m_src_blend = 2;
    m_unk10.m_dest_blend = 2;
    m_unk10.m_initial_direction.m_x = 0.0f;
    m_unk10.m_initial_direction.m_y = 1.0f;
    m_unk10.m_initial_direction.m_z = 0.0f;
    m_unk10.m_initial_direction_bias.m_x = 0.0f;
    m_unk10.m_initial_direction_bias.m_y = 0.0f;
    m_unk10.m_initial_direction_bias.m_z = 0.0f;

    m_unk10.m_unk3C.m_r = 255.0f;
    m_unk10.m_unk3C.m_g = 255.0f;
    m_unk10.m_unk3C.m_b = 255.0f;
    m_unk10.m_unk3C.m_a = 255.0f;

    m_unk10.m_emitter_size.m_x = 0.0f;
    m_unk10.m_emitter_size.m_y = 0.0f;
    m_unk10.m_emitter_size.m_z = 0.0f;
    m_unk10.m_texture_id[0] = '\0';

    m_unk10.m_unk78.m_x = 1.0f;
    m_unk10.m_unk78.m_y = 1.0f;

    m_unk10.m_force.m_x = 0.0f;
    m_unk10.m_force.m_y = 0.0f;
    m_unk10.m_force.m_z = 0.0f;
    m_unk10.m_life_time = 1.0f;
    m_unk10.m_life_time_bias = 0.0f;
    m_unk10.m_initial_velocity = 1.0f;
    m_unk10.m_initial_velocity_bias = 0.0f;

    m_unk10.m_start_tex_coords.m_r = 0.0f;
    m_unk10.m_start_tex_coords.m_g = 0.0f;
    m_unk10.m_start_tex_coords.m_b = 1.0f;
    m_unk10.m_start_tex_coords.m_a = 1.0f;
    m_unk10.m_unkAC.m_r = 0.0f;
    m_unk10.m_unkAC.m_g = 0.0f;
    m_unk10.m_unkAC.m_b = 0.0f;
    m_unk10.m_unkAC.m_a = 0.0f;
    m_unk10.m_end_tex_coords.m_r = 0.0f;
    m_unk10.m_end_tex_coords.m_g = 0.0f;
    m_unk10.m_end_tex_coords.m_b = 1.0f;
    m_unk10.m_end_tex_coords.m_a = 1.0f;
    m_unk10.m_unkCC.m_r = 0.0f;
    m_unk10.m_unkCC.m_g = 0.0f;
    m_unk10.m_unkCC.m_b = 0.0f;
    m_unk10.m_unkCC.m_a = 0.0f;

    m_unk10.m_start_size.m_x = 1.0f;
    m_unk10.m_start_size.m_y = 1.0f;
    m_unk10.m_start_size_bias.m_x = 0.0f;
    m_unk10.m_start_size_bias.m_y = 0.0f;
    m_unk10.m_end_size.m_x = 1.0f;
    m_unk10.m_end_size.m_y = 1.0f;
    m_unk10.m_end_size_bias.m_x = 0.0f;
    m_unk10.m_end_size_bias.m_y = 0.0f;

    m_unk10.m_start_color.m_r = 255.0f;
    m_unk10.m_start_color.m_g = 255.0f;
    m_unk10.m_start_color.m_b = 255.0f;
    m_unk10.m_start_color.m_a = 255.0f;
    m_unk10.m_start_color_bias.m_r = 0.0f;
    m_unk10.m_start_color_bias.m_g = 0.0f;
    m_unk10.m_start_color_bias.m_b = 0.0f;
    m_unk10.m_start_color_bias.m_a = 0.0f;
    m_unk10.m_end_color.m_r = 255.0f;
    m_unk10.m_end_color.m_g = 255.0f;
    m_unk10.m_end_color.m_b = 255.0f;
    m_unk10.m_end_color.m_a = 255.0f;
    m_unk10.m_end_color_bias.m_r = 0.0f;
    m_unk10.m_end_color_bias.m_g = 0.0f;
    m_unk10.m_end_color_bias.m_b = 0.0f;
    m_unk10.m_end_color_bias.m_a = 0.0f;
}

}
