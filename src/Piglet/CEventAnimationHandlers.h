#ifndef PIGLET_CEVENTANIMATIONHANDLERS_H
#define PIGLET_CEVENTANIMATIONHANDLERS_H

#include "engine/display/IEventAnimationCallback.h"

class CFXEventCallback : public DKDSP::IEventAnimationCallback {
public:
    CFXEventCallback() {}

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*);
    virtual void Start(void*);
    virtual void Stop(void*);
    virtual void Pause(void*);
    virtual void Resume(void*);
};

class CSNDEventCallback : public DKDSP::IEventAnimationCallback {
public:
    CSNDEventCallback() {}

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*);
    virtual void Start(void*);
    virtual void Stop(void*);
    virtual void Pause(void*);
    virtual void Resume(void*);
};

class CVIBEventCallback : public DKDSP::IEventAnimationCallback {
public:
    CVIBEventCallback() {}

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*);
    virtual void Start(void*);
    virtual void Stop(void*);
    virtual void Pause(void*);
    virtual void Resume(void*);
};

class CVIB2DEventCallback : public DKDSP::IEventAnimationCallback {
public:
    CVIB2DEventCallback() {}

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*);
    virtual void Start(void*);
    virtual void Stop(void*);
    virtual void Pause(void*);
    virtual void Resume(void*);
};

class CShakeEventCallback : public DKDSP::IEventAnimationCallback {
public:
    CShakeEventCallback() {}

    virtual void HandleEvent(F32, DkXmd::CChunkIterator*, void*);
    virtual void Start(void*);
    virtual void Stop(void*);
    virtual void Pause(void*);
    virtual void Resume(void*);
};

#endif
