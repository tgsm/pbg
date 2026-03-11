#ifndef PIGLET_CRESOURCEFACTORY_H
#define PIGLET_CRESOURCEFACTORY_H

#include <string>
#include <vector>
#include "engine/display/ITextureCallback.h"
#include "engine/filesys/DkFileSys.h"

// TODO
class CErrorCallback : public CDkFileSysErrorCallBack {
public:
    CErrorCallback() {
        m_unk8 = 0;
    }
    virtual U32 Error(U32);
};

// Unofficial name
enum EResType {
    RESOURCE_TYPE_CLUMP = 1,
    RESOURCE_TYPE_SPLINE,
    RESOURCE_TYPE_ANIMATION,
    RESOURCE_TYPE_DMORPH_ANIMATION,
    RESOURCE_TYPE_MATERIAL_ANIMATION,
    RESOURCE_TYPE_TEXTURE_DICTIONARY,
    RESOURCE_TYPE_TEXTURE,
    RESOURCE_TYPE_EVENT_ANIMATION,
    RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION,
    RESOURCE_TYPE_SAMPLE_BANK1, // "SndDict"?
    RESOURCE_TYPE_ANIMATION_STAR,
    RESOURCE_TYPE_SAMPLE_BANK2, // "StreamDict"?
};

// Unofficial name
enum EResLoadResult {
    RESOURCE_LOAD_RESULT_OK,
    RESOURCE_LOAD_RESULT_NOT_FOUND,
    RESOURCE_LOAD_RESULT_2,
    RESOURCE_LOAD_RESULT_INVALID_TYPE,
};

struct SResEntry {
    char name[0x60];
    int unk60;
    U32 type;
};
REQUIRE_SIZE(SResEntry, 0x68);

class CResourceFactory : public DKDSP::ITextureCallback {
public:
    int m_unk4;
    CGame* m_game;
    U32 m_unkC;
    std::vector<SResEntry> m_resource_entries;
    U8 m_unk1C[0x28 - 0x1C];

public:
    CResourceFactory(CGame* game);
    virtual ~CResourceFactory();

    virtual void AddTexture(DKDSP::ITexture* texture);
    virtual U32 LoadResource(U32 type, std::string& name);
    virtual U32 LoadResource(U32 type, const char* name);
    virtual void* LoadPureFile(std::string& filename, U32* size);
    virtual void* LoadPureFile(const char* filename, U32* size);

    BOOL AddEntryInline(U32 a1, U32 type, const char* name) {
        U32 found = FALSE;
        if (m_unk4 == 0) {
            return FALSE;
        }
        for (int i = 0; i < (int)m_resource_entries.size(); i++) {
            if (strcmp(m_resource_entries[i].name, name) == 0) {
                found = TRUE;
                SResEntry& entry = m_resource_entries[i];
                if (entry.unk60 < a1) {
                    entry.unk60 = a1;
                }
                break;
            }
        }
        if (!found) {
            SResEntry entry;
            strcpy(entry.name, name);
            entry.unk60 = a1;
            entry.type = type;
            m_resource_entries.push_back(entry);
            return TRUE;
        }

        return FALSE;
    }

    void UnloadResources(U32);
    void UnloadSpecificResource(std::string resource_name, BOOL);
};
REQUIRE_SIZE(CResourceFactory, 0x28);

#endif
