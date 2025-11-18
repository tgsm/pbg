#include "engine/input/IInputEngine.h"
#include "engine/sound/CSample.h"
#include "engine/sound/CSound3D.h"
#include "entities/CEntityHero.h"
#include "entities/CEntityMesh.h"
#include "CEventAnimationHandlers.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <cstdlib>
#include <iostream>

void CFXEventCallback::HandleEvent(F32, DkXmd::CChunkIterator* iter, void* arg) {
    DkXmd::CChunkIterator dest;
    CEntityMesh* mesh = (CEntityMesh*)arg;

    if (mesh != NULL && !(mesh->m_entity_manager->GetGame()->m_unk8 & (1 << 8))) {
        if (iter->GetChunk("Id", dest)) {
            int id = dest.GetS32Value();
            if (iter->GetChunk("START", dest)) {
                mesh->StartAttachedFX(id);
            } else {
                mesh->StopAttachedFX(id);
            }
        }
    }
}

void CFXEventCallback::Start(void* arg) {

}

void CFXEventCallback::Stop(void* arg) {
    CEntityMesh* mesh = (CEntityMesh*)arg;
    if (mesh != NULL) {
        mesh->StopAllAttachedFX();
    }
}

void CFXEventCallback::Pause(void* arg) {
    CEntityMesh* mesh = (CEntityMesh*)arg;
    if (mesh != NULL) {
        mesh->PauseAllAttachedFX();
    }
}

void CFXEventCallback::Resume(void* arg) {
    CEntityMesh* mesh = (CEntityMesh*)arg;
    if (mesh != NULL) {
        mesh->ResumeAllAttachedFX();
    }
}

// Equivalent: regalloc, rand()
static inline int check_value(int value) {
    // volatile is used to force the rand() call after the first variable
    volatile U8 gofirstdammit = ((value << 8) / 100);
    U8 golastdammit = rand() & 0xFF;
    return golastdammit <= gofirstdammit;
}
void CSNDEventCallback::HandleEvent(F32, DkXmd::CChunkIterator* iter, void* arg) {
    DkXmd::CChunkIterator dest;
    CEntityMesh* mesh = (CEntityMesh*)arg;
    DKSND::CSample* sample;
    DKSND::CSound3D* _3d_sound;
    DKSND::CSound2D* _2d_sound;

    if (mesh != NULL && !(mesh->m_entity_manager->GetGame()->m_unk8 & (1 << 8))) {
        if (!iter->GetChunk("Random", dest)) {
            goto good;
        }
        int value = dest.GetS32Value();
        if (value > 0 && (value > 100 || check_value(value))) {
good:
            if (iter->GetChunk("Id", dest)) {
                int id = dest.GetS32Value();
                _3d_sound = NULL;
                _2d_sound = NULL;
                sample = mesh->GetSampleById(id);
                F32 variation = mesh->GetPitchVariationById(id);
                if (sample != NULL) {
                    if (sample->Is3D()) {
                        _3d_sound = mesh->m_sound_emitter->PlaySound(sample, 0);
                    } else {
                        _2d_sound = mesh->m_entity_manager->GetGame()->m_sound_engine->PlaySound2D(sample, 1);
                    }

                    if (_3d_sound != NULL) {
                        if (variation > 0.0f) {
                            F32 pitch = ((((rand() / 32767.0f) * 2.0f) - 1.0f));
                            pitch *= variation;
                            _3d_sound->SetPitch(pitch + 1.0f);
                        }

                        _3d_sound->SetMinDistance(8.0f);

                        if (mesh->m_entity_manager->GetGame()->GetIngameGamePart() != NULL && mesh->m_entity_manager->GetGame()->GetIngameGamePart()->m_unk0 == 7) {
                            if (mesh->m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->IsOnFight()) {
                                _3d_sound->SetLayer(4);
                            } else {
                                _3d_sound->SetLayer(1);
                            }
                        } else {
                            _3d_sound->SetLayer(1);
                        }
                        _3d_sound->SetVolume(1.0f);
                    }

                    if (_2d_sound != NULL) {
                        if (variation > 0.0f) {
                            F32 pitch = ((((rand() / 32767.0f) * 2.0f) - 1.0f));
                            pitch *= variation;
                            _2d_sound->SetPitch(pitch + 1.0f);
                        }

                        if (mesh->m_entity_manager->GetGame()->GetIngameGamePart() != NULL && mesh->m_entity_manager->GetGame()->GetIngameGamePart()->m_unk0 == 7) {
                            if (mesh->m_entity_manager->GetGame()->GetIngameGamePart()->GetGameRoomManager()->IsOnFight()) {
                                _2d_sound->SetLayer(4);
                            } else {
                                _2d_sound->SetLayer(1);
                            }
                        } else {
                            _2d_sound->SetLayer(1);
                        }

                        _2d_sound->SetVolume(1.0f);
                    }
                }
            }
        }
    }
}

void CSNDEventCallback::Start(void*) {

}

void CSNDEventCallback::Stop(void*) {

}

void CSNDEventCallback::Pause(void*) {

}

void CSNDEventCallback::Resume(void*) {

}

// TODO
void CVIBEventCallback::HandleEvent(F32, DkXmd::CChunkIterator*, void*) {

}

void CVIBEventCallback::Start(void*) {

}

void CVIBEventCallback::Stop(void*) {

}

void CVIBEventCallback::Pause(void*) {

}

void CVIBEventCallback::Resume(void*) {

}

void CVIB2DEventCallback::HandleEvent(F32, DkXmd::CChunkIterator* iter, void* arg) {
    DkXmd::CChunkIterator dest;
    CEntityMesh* mesh = (CEntityMesh*)arg;

    if (iter->GetChunk("Value", dest)) {
        if (!(mesh->m_entity_manager->GetGame()->m_unk8 & (1 << 8))) {
            int value = dest.GetS32Value();
            DKI::IInputEngine::GetDevice(0)->SendVibration(value);
        }
    }
}

void CVIB2DEventCallback::Start(void*) {

}

void CVIB2DEventCallback::Stop(void*) {

}

void CVIB2DEventCallback::Pause(void*) {

}

void CVIB2DEventCallback::Resume(void*) {

}

// TODO
void CShakeEventCallback::HandleEvent(F32, DkXmd::CChunkIterator* iter, void* arg) {

}

void CShakeEventCallback::Start(void*) {

}

void CShakeEventCallback::Stop(void*) {

}

void CShakeEventCallback::Pause(void*) {

}

void CShakeEventCallback::Resume(void*) {

}
