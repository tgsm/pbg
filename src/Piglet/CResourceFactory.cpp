#include "CResourceFactory.h"
#include "CGCNFont.h"
#include "CGame.h"
#include "engine/display/DkDisplay.h"
#include "engine/filesys/DkFileSys.h"
#include <iostream>

CResourceFactory::CResourceFactory(CGame* game) {
    m_game = game;
    m_resource_entries.reserve(0x400);
    m_unkC = 0;
    m_unk4 = 1;
}

// Equivalent: std::vector
CResourceFactory::~CResourceFactory() {
    m_resource_entries.clear();
}

U32 CResourceFactory::LoadResource(U32 type, std::string& name) {
    return LoadResource(type, name.c_str());
}

// Equivalent?
U32 CResourceFactory::LoadResource(U32 type, const char* name) {
    char common_buf[256];
    sprintf(common_buf, "./Data/Common/%s", name);
    char gcn_buf[256];
    sprintf(gcn_buf, "./Data/GCN/%s", name);

    switch (type) {
        case RESOURCE_TYPE_CLUMP:
            m_game->m_display_engine->SetImagePath("Data/NoTextures/");
            if (m_game->GetObjectDictionary()->FindClump(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->GetObjectDictionary()->LoadClumpFromFile(name, gcn_buf, 1)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
                m_game->m_texture_dictionary->Update();
            }
            break;
        case RESOURCE_TYPE_SPLINE:
            if (m_game->GetObjectDictionary()->FindSpline(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->GetObjectDictionary()->LoadSplineFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION:
            if (m_game->GetObjectDictionary()->FindParticleEmitterDefinition(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->GetObjectDictionary()->LoadParticleEmitterDefinitionFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_ANIMATION:
            if (m_game->m_anim_dictionary->FindAnimation(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->m_anim_dictionary->LoadAnimationFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_ANIMATION_STAR:
            if (m_game->m_anim_dictionary->FindAnimationStar(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->m_anim_dictionary->LoadAnimationStarFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_EVENT_ANIMATION:
            if (m_game->m_anim_dictionary->FindEventAnimation(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->m_anim_dictionary->LoadEventAnimationFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_DMORPH_ANIMATION:
            if (m_game->m_anim_dictionary->FindDMorphAnimation(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->m_anim_dictionary->LoadDMorphAnimationFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_MATERIAL_ANIMATION:
            if (m_game->m_anim_dictionary->FindMaterialAnimation(name)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            } else {
                if (!m_game->m_anim_dictionary->LoadMaterialAnimationFromFile(name, gcn_buf)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        case RESOURCE_TYPE_TEXTURE_DICTIONARY: {
            U32 unkC = m_unkC;
            if (AddEntryInline(unkC, type, name)) {
                if (!m_game->m_texture_dictionary->LoadTextureDictionaryFromFile(gcn_buf)) {
                    if (!m_game->m_texture_dictionary->LoadTextureDictionaryFromFile(common_buf)) {
                        return RESOURCE_LOAD_RESULT_NOT_FOUND;
                    }
                }
                m_game->m_texture_dictionary->Update();
            }
            break;
        }
        case RESOURCE_TYPE_SAMPLE_BANK1: {
            std::string name_ = name;
            std::string gcn_buf_ = gcn_buf;
            std::string common_buf_ = common_buf;
            if (m_game->m_sample_dictionary->FindSampleBank(&name_)) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
                break;
            } else {
                if (!m_game->m_sample_dictionary->LoadSampleBankFromFile(&name_, &gcn_buf_)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        }
        case RESOURCE_TYPE_SAMPLE_BANK2: {
            std::string name_ = name;
            std::string gcn_buf_ = gcn_buf;
            std::string common_buf_ = common_buf;
            if (m_game->m_sample_dictionary->FindSampleBank(&name_) != NULL) {
                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
                break;
            } else {
                if (!m_game->m_sample_dictionary->LoadStreamSampleBankFromFile(&name_, &gcn_buf_)) {
                    return RESOURCE_LOAD_RESULT_NOT_FOUND;
                }

                U32 unkC = m_unkC;
                AddEntryInline(unkC, type, name);
            }
            break;
        }
        default:
            return RESOURCE_LOAD_RESULT_INVALID_TYPE;
    }

    return RESOURCE_LOAD_RESULT_OK;
}

void* CResourceFactory::LoadPureFile(std::string& filename, U32* size) {
    return LoadPureFile(filename.c_str(), size);
}

void* CResourceFactory::LoadPureFile(const char* filename, U32* size) {
    char common_buf[256];
    sprintf(common_buf, "./Data/Common/%s", filename);
    char gcn_buf[256];
    sprintf(gcn_buf, "./Data/GCN/%s", filename);

    void* file = CDkFileSys::fopen(gcn_buf, "rb");
    if (file == NULL && file == NULL) {
        return NULL;
    }

    CDkFileSys::fseek(file, 0, 2); // SEEK_END
    U32 length = CDkFileSys::ftell(file);
    if (size != NULL) {
        *size = length;
    }

    void* ret = new U8[length];
    if (ret == NULL) {
        CDkFileSys::fclose(file);
        return NULL;
    }

    CDkFileSys::fseek(file, 0, 0); // SEEK_SET
    CDkFileSys::fread(ret, 1, length, file);
    CDkFileSys::fclose(file);

    return ret;
}

void CResourceFactory::UnloadResources(U32 a1) {
    std::string name;

    if (m_resource_entries.size() != 0) {
        std::vector<SResEntry>::iterator iter = m_resource_entries.end();
        iter--;
        for (; iter >= m_resource_entries.begin(); iter--) {
            if (iter->unk60 <= a1) {
                switch (iter->type) {
                    case RESOURCE_TYPE_CLUMP:
                        m_game->GetObjectDictionary()->RemoveClump(iter->name);
                        break;
                    case RESOURCE_TYPE_SPLINE:
                        m_game->GetObjectDictionary()->RemoveSpline(iter->name);
                        break;
                    case RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION:
                        m_game->GetObjectDictionary()->RemoveParticleEmitterDefinition(iter->name);
                        break;
                    case RESOURCE_TYPE_ANIMATION:
                        m_game->m_anim_dictionary->RemoveAnimation(iter->name);
                        break;
                    case RESOURCE_TYPE_EVENT_ANIMATION:
                        m_game->m_anim_dictionary->RemoveEventAnimation(iter->name);
                        break;
                    case RESOURCE_TYPE_DMORPH_ANIMATION:
                        m_game->m_anim_dictionary->RemoveDMorphAnimation(iter->name);
                        break;
                    case RESOURCE_TYPE_MATERIAL_ANIMATION:
                        m_game->m_anim_dictionary->RemoveMaterialAnimation(iter->name);
                        break;
                    case RESOURCE_TYPE_SAMPLE_BANK1:
                        name = iter->name;
                        m_game->m_sample_dictionary->RemoveSampleBank(&name);
                        break;
                    case RESOURCE_TYPE_SAMPLE_BANK2:
                        name = iter->name;
                        m_game->m_sample_dictionary->RemoveSampleBank(&name);
                        break;
                    case RESOURCE_TYPE_ANIMATION_STAR:
                        m_game->m_anim_dictionary->RemoveAnimationStar(iter->name);
                        break;
                }

                if (iter->type != RESOURCE_TYPE_TEXTURE) {
                    m_resource_entries.erase(iter);
                }
            }
        }
    }

    if (m_resource_entries.size() != 0) {
        std::vector<SResEntry>::iterator iter = m_resource_entries.end();
        iter--;
        for (; iter >= m_resource_entries.begin(); iter--) {
            if (iter->unk60 <= a1) {
                switch (iter->type) {
                    case RESOURCE_TYPE_CLUMP:
                    case RESOURCE_TYPE_SPLINE:
                    case RESOURCE_TYPE_ANIMATION:
                    case RESOURCE_TYPE_DMORPH_ANIMATION:
                    case RESOURCE_TYPE_MATERIAL_ANIMATION:
                    case RESOURCE_TYPE_TEXTURE_DICTIONARY:
                        break;
                    case RESOURCE_TYPE_TEXTURE: {
                        m_game->m_texture_dictionary->RemoveTexture(iter->name);
                        char buf[0x60];
                        strcpy(buf, iter->name);
                        m_resource_entries.erase(iter);
                        m_game->m_texture_dictionary->Update();
                        if (m_game->m_texture_dictionary->FindTexture(buf)) {
                            while (m_game->m_texture_dictionary->FindTexture(buf)) {
                                m_game->m_texture_dictionary->RemoveTexture(buf);
                            }
                        }
                        break;
                    }
                    case RESOURCE_TYPE_EVENT_ANIMATION:
                    case RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION:
                    case RESOURCE_TYPE_SAMPLE_BANK1:
                    case RESOURCE_TYPE_ANIMATION_STAR:
                    case RESOURCE_TYPE_SAMPLE_BANK2:
                        break;
                }
            }
        }
    }

    m_game->m_texture_dictionary->Update();
}

void CResourceFactory::UnloadSpecificResource(std::string resource_name, BOOL a2) {
    std::string name;

    if (m_resource_entries.size() != 0) {
        std::vector<SResEntry>::iterator iter = m_resource_entries.end();
        iter--;
        for (; iter >= m_resource_entries.begin(); iter--) {
            if (strcmp(iter->name, resource_name.c_str()) == 0) {
                if (a2 == TRUE) {
                    switch (iter->type) {
                        case RESOURCE_TYPE_TEXTURE:
                            m_game->m_texture_dictionary->RemoveTexture(iter->name);
                            break;
                        case RESOURCE_TYPE_CLUMP:
                            m_game->GetObjectDictionary()->RemoveClump(iter->name);
                            break;
                        case RESOURCE_TYPE_SPLINE:
                            m_game->GetObjectDictionary()->RemoveSpline(iter->name);
                            break;
                        case RESOURCE_TYPE_PARTICLE_EMITTER_DEFINITION:
                            m_game->GetObjectDictionary()->RemoveParticleEmitterDefinition(iter->name);
                            break;
                        case RESOURCE_TYPE_ANIMATION:
                            m_game->m_anim_dictionary->RemoveAnimation(iter->name);
                            break;
                        case RESOURCE_TYPE_EVENT_ANIMATION:
                            m_game->m_anim_dictionary->RemoveEventAnimation(iter->name);
                            break;
                        case RESOURCE_TYPE_DMORPH_ANIMATION:
                            m_game->m_anim_dictionary->RemoveDMorphAnimation(iter->name);
                            break;
                        case RESOURCE_TYPE_MATERIAL_ANIMATION:
                            m_game->m_anim_dictionary->RemoveMaterialAnimation(iter->name);
                            break;
                        case RESOURCE_TYPE_SAMPLE_BANK1: {
                            name = iter->name;
                            m_game->m_sample_dictionary->RemoveSampleBank(&name);
                            break;
                        }
                        case RESOURCE_TYPE_SAMPLE_BANK2: {
                            name = iter->name;
                            m_game->m_sample_dictionary->RemoveSampleBank(&name);
                            break;
                        }
                        case RESOURCE_TYPE_ANIMATION_STAR:
                            m_game->m_anim_dictionary->RemoveAnimationStar(iter->name);
                            break;
                    }
                }
                m_resource_entries.erase(iter);
                break;
            }
        }
    }

    m_game->m_texture_dictionary->Update();
}

void CResourceFactory::AddTexture(DKDSP::ITexture* texture) {
    U32 a1 = m_unkC;
    AddEntryInline(a1, RESOURCE_TYPE_TEXTURE, texture->GetName());
}

// Equivalent?
U32 CErrorCallback::Error(U32 unk) {
    switch (unk) {
        case 0:
            m_unk8 = 0;
            break;
        case 1:
            m_unk8 = 1;
            break;
        case 2:
            m_unk8 = 2;
            break;
        case 3:
            m_unk8 = 3;
            break;
        case 4:
            m_unk8 = 4;
            break;
        case 5:
            m_unk8 = 5;
            break;
        default:
            m_unk8 = 0;
            break;
    }

    if (m_unk8 != 0 && m_unk8 != 5 && m_unk8 != 2 && m_game != NULL && m_game->m_current_loading_callback != NULL) {
        m_game->m_current_loading_callback->Update();
        return 1;
    }

    if (CGCNFont::m_pImage != NULL) {
        if (unk == 2 || unk == 5) {
            if (m_game->GetScene() != NULL) {
                DkDisplayGetEngine()->Update();
                m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
                m_game->GetScene()->BeginRender();
            }

            if (unk == 2) {
                CGCNFont::PrintText(0, 220, "The Game Disc could not be read.");
                CGCNFont::PrintText(0, 250, "Please read the Nintendo GameCube Instruction Booklet");
                CGCNFont::PrintText(0, 280, "for more information.");
            }
            if (unk == 5) {
                CGCNFont::PrintText(0, 220, "An error has occurred. Turn the power off");
                CGCNFont::PrintText(0, 250, "and refer to the Nintendo GameCube Instruction Booklet");
                CGCNFont::PrintText(0, 280, "for further instructions.");
            }

            if (m_game->GetScene() != NULL) {
                m_game->GetScene()->EndRender();
                m_game->GetScene()->Flip(0);
            }
        }
        CGame::ManageReset();
        return 1;
    } else {
        F32 cam_buffer_width = m_game->GetCamera()->GetBuffer()->GetWidth();
        F32 cam_buffer_height = m_game->GetCamera()->GetBuffer()->GetHeight();

        if (unk == 2) {
            m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);

            F32 y = 224.0f;
            m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);
            F32 text_width = m_game->GetGuiEngine()->GetTextWidth("The Game Disc could not be read.", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
            m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "The Game Disc could not be read.", 16.0f / cam_buffer_height, NULL, 0.0f);

            text_width = m_game->GetGuiEngine()->GetTextWidth("Please read the Nintendo GameCube Instruction Booklet", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
            y += 16.0f;
            m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "Please read the Nintendo GameCube Instruction Booklet", 16.0f / cam_buffer_height, NULL, 0.0f);

            text_width = m_game->GetGuiEngine()->GetTextWidth("for more information.", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
            y += 16.0f;
            m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "for more information.", 16.0f / cam_buffer_height, NULL, 0.0f);

            m_game->GetScene()->SelectCamera(m_game->GetCamera());
            m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
            m_game->GetScene()->BeginRender();
            m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_game->GetCamera()->m_wrap_camera->m_rw_camera);
            m_game->GetScene()->EndRender();
            m_game->GetScene()->Flush();
            m_game->GetScene()->Flip(0);

            CGame::ManageReset();
        } else if (unk == 5) {
            while (TRUE) {
                F32 y = 224.0f;
                m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);
                F32 text_width = m_game->GetGuiEngine()->GetTextWidth("An error has occurred. Turn the power off", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
                m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "An error has occurred. Turn the power off", 16.0f / cam_buffer_height, NULL, 0.0f);
                text_width = m_game->GetGuiEngine()->GetTextWidth("and refer to the Nintendo GameCube Instruction Booklet", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
                y += 16.0f;
                m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "and refer to the Nintendo GameCube Instruction Booklet", 16.0f / cam_buffer_height, NULL, 0.0f);
                text_width = m_game->GetGuiEngine()->GetTextWidth("for further instructions.", 16.0f / cam_buffer_height, NULL) * cam_buffer_width;
                y += 16.0f;
                m_game->GetGuiEngine()->AddText(((cam_buffer_width - text_width) / 2) / cam_buffer_width, y / cam_buffer_height, "for further instructions.", 16.0f / cam_buffer_height, NULL, 0.0f);

                m_game->GetScene()->SelectCamera(m_game->GetCamera());
                m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);
                m_game->GetScene()->BeginRender();
                m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_game->GetCamera()->m_wrap_camera->m_rw_camera);
                m_game->GetScene()->EndRender();
                m_game->GetScene()->Flush();
                m_game->GetScene()->Flip(0);
            }
        }
    }

    return 1;
}
