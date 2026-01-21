#ifndef PIGLET_CLOADING_H
#define PIGLET_CLOADING_H

#include "engine/display/CCamera.h"
#include "engine/display/CClump.h"
#include "engine/display/CController.h"
#include "engine/display/CLight.h"
#include "engine/display/CScene.h"
#include "engine/display/CShadowMap.h"
#include "engine/filesys/DkFileSys.h"
#include "engine/wrap/DKW_RGBA.h"

class CGame;

class CBaseLoadingCallback : public CDkFileSysLoadCallBack {
public:
    CGame* m_game;

public:
    CBaseLoadingCallback(CGame* game) : m_game(game) {}
    virtual ~CBaseLoadingCallback() {}

    virtual void Update() = 0;
    virtual void Create() = 0;
    virtual void Destroy() = 0;
};

class CLoadingCallback : public CBaseLoadingCallback {
public:
    DKDSP::CScene* m_scene;
    DKDSP::CCamera* m_camera;
    DKDSP::CLight* m_light;
    DKDSP::CShadowMap* m_shadow_map;
    DKDSP::CLight* m_light2;
    DKDSP::CClump* m_clump1C;
    DKDSP::CController* m_controller;
    DKDSP::CClump* m_clump24;
    F32 m_time;
    F32 m_unk2C;
    CDKW_RGBA m_unk30;
    CDKW_RGBA m_unk34;
    RwTexture* m_unk38;
    U32 m_unk3C;
    U32 m_unk40;

public:
    CLoadingCallback(CGame* game);
    virtual ~CLoadingCallback();

    virtual void Update();
    virtual void Create();
    virtual void Destroy();
    virtual F32 UpdateTimer();
    virtual void UpdateObjects(F32 dt);
    virtual void RenderObjects();

    inline void LookAtInline(DKDSP::IFramable* framable, CDKW_V3d vec1, CDKW_V3d vec2, CDKW_V3d vec3) {
        ((CDKW_Matrix*)(&framable->GetFrame()->m_rwframe->modelling))->LookAt(vec1, vec2, vec3);
        RwFrameUpdateObjects(framable->GetFrame()->m_rwframe);
    }

    inline void UpdateModelPos(DKDSP::IFramable* framable, CDKW_V3d pos) {
        RwMatrix* model = &framable->GetFrame()->m_rwframe->modelling;
        model->pos = pos;
        RwMatrixUpdate(model);
        RwFrameUpdateObjects(framable->GetFrame()->m_rwframe);
    }
};
REQUIRE_SIZE(CLoadingCallback, 0x44);

class CInGameLoadingCallback : public CBaseLoadingCallback {
public:
    CGame* m_game; // Overrides base m_game
    U32 m_unkC;
    F32 m_unk10;
    U8 m_unk14[4];

public:
    CInGameLoadingCallback(CGame* game);
    virtual ~CInGameLoadingCallback();

    virtual void Destroy();
    virtual void Update();
    virtual void Create();
};
REQUIRE_SIZE(CInGameLoadingCallback, 0x18);

class CBootUpLoadingCallback : public CBaseLoadingCallback {
public:
    CGame* m_game; // Overrides base m_game
    F32 m_unkC;
    U8 m_unk10[4];

public:
    CBootUpLoadingCallback(CGame* game);
    virtual ~CBootUpLoadingCallback();

    virtual void Destroy();
    virtual void Update();
    virtual void Create();
};
REQUIRE_SIZE(CBootUpLoadingCallback, 0x14);

class CPreBootUpLoadingCallback : public CBaseLoadingCallback {
public:
    CGame* m_game; // Overrides base m_game

public:
    CPreBootUpLoadingCallback(CGame* game);
    virtual ~CPreBootUpLoadingCallback() {}

    virtual void Destroy();
    virtual void Update();
    virtual void Create();
};
REQUIRE_SIZE(CPreBootUpLoadingCallback, 0xC);

class CVideoLoadingCallback : public CBaseLoadingCallback {
public:
    CGame* m_game; // Overrides base m_game

public:
    CVideoLoadingCallback(CGame* game);
    virtual ~CVideoLoadingCallback() {}

    virtual void Create();
    virtual void Destroy();
    virtual void Update();
};
REQUIRE_SIZE(CVideoLoadingCallback, 0xC);

#endif
